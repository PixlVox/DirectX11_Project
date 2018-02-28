#include <windows.h>
#include <ctime>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include"Device.h"

#include"Vertex.h"
#include"Geometry.h"
#include"Pixel.h"

#include"Terrain.h"
#include"Camera.h"
#include"BlendQuad.h"
#include"PickQuad.h"

const LONG WIDTH = 1200;
const LONG HEIGHT = 700;

HWND initWindow(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
D3D11_VIEWPORT SetViewport(ID3D11DeviceContext* dContext);
void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel,
	Terrain* terrain, Camera* cam, BlendQuad* blendQ,
	PickQuad* pQ, D3D11_VIEWPORT vp, float& timer);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {

	MSG msg = { 0 };

	//Init to be able to load textures
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//Create window
	HWND wHandle = initWindow(hInst);

	if (wHandle) {

		//Create objects
		Device device;

		Vertex vertex;
		Geometry geometry;
		Pixel pixel;

		Terrain terrain;
		BlendQuad blendQ;
		PickQuad pQ;
		Camera cam(WIDTH, HEIGHT);
		
		D3D11_VIEWPORT vpCopy;
		HRESULT hr = NULL;

		//Deltatime
		float deltaTime = 0.0f;
		float timer = 0.0f;
		std::clock_t deltaClock;

		//Create device object(device, deviceContext, swapChain, backBufferRVT)
		hr = device.createContext(wHandle, (int)WIDTH, (int)HEIGHT);

		if (FAILED(hr)) {

			exit(-1);

		}

		//Set the viewport
		vpCopy = SetViewport(device.getDeviceContext());

		//Create shaders
		vertex.createShader(device.getDevice());
		geometry.createShader(device.getDevice());
		pixel.createShader(device.getDevice());

		//Create buffers
		terrain.initHeightMap(device.getDevice());
		blendQ.createBuffer(device.getDevice());
		pQ.createBuffer(device.getDevice());

		//Create textures and states
		terrain.createTexture(device.getDevice());
		blendQ.createTexture(device.getDevice());
		terrain.createSamplerState(device.getDevice());
		blendQ.createBlendState(device.getDevice());

		//Create constant buffer
		terrain.createConstBuffer(device.getDevice());
		blendQ.createConstBuffer(device.getDevice());
		pQ.createConstBuffer(device.getDevice());

		//Create camera
		cam.initDI(hInst, wHandle);

		//Sets state of window
		ShowWindow(wHandle, nCmdShow);

		//Updates the program
		while (WM_QUIT != msg.message) {

			//Start timer
			deltaClock = std::clock();

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			else {

				//Render
				Render(&device, &vertex, &geometry, &pixel, &terrain,
					&cam, &blendQ, &pQ, vpCopy, timer);

				//Swap front & back buffers
				device.getSwapChain()->Present(0, 0);

			}

			deltaTime = (float)(std::clock() - deltaClock) / (float)CLOCKS_PER_SEC;
			timer += deltaTime;

		}
	}

		return (int)msg.wParam;

}

void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel,
	Terrain* terrain, Camera* cam, BlendQuad* blendQ,
	PickQuad* pQ, D3D11_VIEWPORT vp, float& timer) {

	//Set a default color
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//Clear screen
	device->getDeviceContext()->ClearDepthStencilView(device->getDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device->getDeviceContext()->ClearRenderTargetView(device->getBackBufferRTV(), clearColor);

	//Update camera
	cam->getInput();
	cam->update(terrain->getHeightValueAtPos(cam->getX(), cam->getZ()));
	DirectX::XMVECTOR mouseDir = cam->pickMouse(vp);

	//UpdateMatrices
	terrain->updateMatrixValues(cam->getView(), cam->getProj(), WIDTH, HEIGHT);
	blendQ->updateMatrix(cam->getView(), cam->getProj(), WIDTH, HEIGHT);
	pQ->updateMatrix(cam->getView(), cam->getProj(), WIDTH, HEIGHT);

	//Check if pickQuad was clicked on
	if (cam->getPick()) {

		if (timer >= 0.16f) {

			if (pQ->intersect(mouseDir, cam->getPosition())) {

				//Update color of quad
				pQ->updateBuffer(device->getDevice());
				timer = 0.0f;

			}

		}

		cam->setPick(false);

	}

	//////////////////////////////////Terrain///////////////////////////////////

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShader(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShader(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getShader(), nullptr, 0);

	//Send grass texture view to PS
	ID3D11ShaderResourceView* grassView = terrain->getGrassView();
	device->getDeviceContext()->PSSetShaderResources(0, 1, &grassView);

	//Send stone texture view to PS
	ID3D11ShaderResourceView* stoneView = terrain->getStoneView();
	device->getDeviceContext()->PSSetShaderResources(1, 1, &stoneView);

	//Send Terrain's texture samplerState to PS
	ID3D11SamplerState* terrainSampler = terrain->getSamplerState();
	device->getDeviceContext()->PSSetSamplers(0, 1, &terrainSampler);

	//Set vertex size and offset
	UINT32 vSize = sizeof(float) * terrain->getValuesPerVertex();
	UINT32 offset = 0;

	//Set vertex & index buffer
	ID3D11Buffer* tempBuffer = terrain->getVertexBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);
	device->getDeviceContext()->IASetIndexBuffer(terrain->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayout());

	//Map constant buffer
	terrain->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set blend state
	float blendValues[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device->getDeviceContext()->OMSetBlendState(nullptr, blendValues, 0xffffffff);

	//Draw Terrain
	device->getDeviceContext()->DrawIndexed((terrain->getNrOfFaces() * 3), 0, 0);

	///////////////////////////////////Draw PickQuad//////////////////////////

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShaderColor(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShaderColor(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getColorShader(), nullptr, 0);

	//Set buffer for GameArea room
	vSize = sizeof(float) * pQ->getValuesPerVertex();
	offset = 0;
	tempBuffer = pQ->getBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);

	//Set inputLayout
	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayoutColor());

	//Map constant buffer
	pQ->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set blend state
	device->getDeviceContext()->OMSetBlendState(nullptr, blendValues, 0xffffffff);

	//Draw BlendQuad
	device->getDeviceContext()->Draw(pQ->getNrOfVertices(), 0);

	/////////////////////////////////BlendQuad///////////////////////////////////////

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShader(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShader(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getBlendShader(), nullptr, 0);

	//Send window texture view to PS
	ID3D11ShaderResourceView* windowView = blendQ->getWindowView();
	device->getDeviceContext()->PSSetShaderResources(0, 1, &windowView);

	//Set buffer for BlendQuad
	vSize = sizeof(float) * blendQ->getValuesPerVertex();
	offset = 0;
	tempBuffer = blendQ->getBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);

	//Set inputLayout
	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayout());

	//Map constant buffer
	blendQ->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set blend state
	device->getDeviceContext()->OMSetBlendState(blendQ->getBlendState(), blendValues, 0xffffffff);

	//Draw BlendQuad
	device->getDeviceContext()->Draw(blendQ->getNrOfVertices(), 0);

}

D3D11_VIEWPORT SetViewport(ID3D11DeviceContext* dContext) {
	
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	dContext->RSSetViewports(1, &vp);

	return vp;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message){

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);

}

HWND initWindow(HINSTANCE hInst) {

	//Create window class
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInst;
	wcex.lpszClassName = L"Project";

	if (!RegisterClassEx(&wcex)) {

		return false;

	}

	//Set window size
	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//Create window handle
	HWND handle = CreateWindow(
		L"Project",
		L"DirectX 11 - Project",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInst,
		nullptr);

	return handle;

}