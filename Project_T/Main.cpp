#include <windows.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include"Device.h"
#include"Vertex.h"
#include"Geometry.h"
#include"Pixel.h"
#include"Compute.h"
#include"Camera.h"
#include"BlendQuad.h"

const LONG WIDTH = 1200;
const LONG HEIGHT = 700;

HWND initWindow(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetViewport(ID3D11DeviceContext* dContext);
void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel, Compute* compute, Camera* cam, BlendQuad* blendQ);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {

	MSG msg = { 0 };

	//Create window
	HWND wHandle = initWindow(hInst);

	if (wHandle) {

		//Create objects
		Device device;

		Vertex vertex;
		Geometry geometry;
		Pixel pixel;
		Compute compute;

		BlendQuad blendQ;
		Camera cam;

		HRESULT hr = NULL;

		//Create device object(device, deviceContext, swapChain, backBufferRVT)
		hr = device.createContext(wHandle, (int)WIDTH, (int)HEIGHT);

		if (FAILED(hr)) {

			exit(-1);

		}

		//Set the viewport
		SetViewport(device.getDeviceContext());

		//Create shaders
		vertex.createShader(device.getDevice());
		geometry.createShader(device.getDevice());
		pixel.createShader(device.getDevice());
		compute.createShader(device.getDevice());

		//Create buffers
		vertex.createTriangleData(device.getDevice());
		blendQ.createBuffer(device.getDevice());
		compute.createBuffers(device.getDevice(), device.getDeviceContext());

		//Create textures
		vertex.createTextures(device.getDevice());
		blendQ.createTexture(device.getDevice());
		vertex.createSamplerState(device.getDevice());
		vertex.createBlendState(device.getDevice());

		//Create constant buffer
		geometry.createConstBuffer(device.getDevice());
		blendQ.createConstBuffer(device.getDevice());

		//Create camera
		cam.initDI(hInst, wHandle);

		//Sets state of window
		ShowWindow(wHandle, nCmdShow);

		//Updates the program
		while (WM_QUIT != msg.message) {

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			else {

				//Render
				Render(&device, &vertex, &geometry, &pixel, &compute, &cam, &blendQ);

				//Swap front & back buffers
				device.getSwapChain()->Present(0, 0);

			}

		}
	}

		return (int)msg.wParam;

}

void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel, Compute* compute, Camera* cam, BlendQuad* blendQ) {

	//Set a default color
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//Clear screen
	device->getDeviceContext()->ClearDepthStencilView(device->getDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device->getDeviceContext()->ClearRenderTargetView(device->getBackBufferRTV(), clearColor);

	//Update camera and matrices
	cam->getInput(0.0f);
	cam->update(vertex->getHeightValueAtPos(cam->getX(), cam->getZ()));
	geo->updateMatrixValues(cam->getView(), (int)WIDTH, (int)HEIGHT);
	blendQ->updateMatrix(cam->getView(), (int)WIDTH, (int)HEIGHT);

	//////////////////////////////////Terrain///////////////////////////////////

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShader(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShader(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getShader(), nullptr, 0);

	//Send grass texture view to PS
	ID3D11ShaderResourceView* grassView = vertex->getGrassView();
	device->getDeviceContext()->PSSetShaderResources(0, 1, &grassView);

	//Send stone texture view to PS
	ID3D11ShaderResourceView* stoneView = vertex->getStoneView();
	device->getDeviceContext()->PSSetShaderResources(1, 1, &stoneView);

	//Send Terrain's texture samplerState to PS
	ID3D11SamplerState* terrainSampler = vertex->getSamplerState();
	device->getDeviceContext()->PSSetSamplers(0, 1, &terrainSampler);

	//Set vertex size and offset
	UINT32 vSize = sizeof(float) * vertex->getValuesPerVertex();
	UINT32 offset = 0;

	//Set vertex & index buffer
	ID3D11Buffer* tempBuffer = vertex->getVertexBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);
	device->getDeviceContext()->IASetIndexBuffer(vertex->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayout());

	//Map constant buffer
	geo->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set blend state
	float blendValues[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device->getDeviceContext()->OMSetBlendState(nullptr, blendValues, 0xffffffff);

	//Draw Terrain
	device->getDeviceContext()->DrawIndexed((vertex->getNrOfFaces() * 3), 0, 0);

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
	device->getDeviceContext()->OMSetBlendState(vertex->getBlendState(), blendValues, 0xffffffff);

	//Draw BlendQuad
	device->getDeviceContext()->Draw(blendQ->getNrOfVertices(), 0);

	///////////////////////////////Compute Shader/////////////////////////////

	//Set compute shader
	device->getDeviceContext()->CSSetShader(compute->getShader(), NULL, 0);

	//Set input glow buffer for CS
	ID3D11ShaderResourceView* tempView = compute->getGlowView();
	device->getDeviceContext()->CSSetShaderResources(0, 1, &tempView);

	//Set input glowMap buffer for CS
	tempView = compute->getGlowMapView();
	device->getDeviceContext()->CSSetShaderResources(1, 1, &tempView);

	//Set output buffer for CS
	ID3D11UnorderedAccessView* tempUnorderedView = compute->getOutputView();
	device->getDeviceContext()->CSSetUnorderedAccessViews(0, 1,
		&tempUnorderedView, (UINT*)&tempUnorderedView);

	//Run CS(Image is 800x800, runs one thread per pixel)
	device->getDeviceContext()->Dispatch(800, 800, 1);

	//Save the result of CS in a accesible resourceView
	compute->saveResult(device->getDevice(), device->getDeviceContext());

	//Store the output from CS in a buffer for VS
	compute->createVertexBuffer(device->getDevice(), device->getDeviceContext());

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShader(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShader(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getGlowShader(), nullptr, 0);

	//Set glow result texture to pixel shader
	ID3D11ShaderResourceView* glowResultView = compute->getResultView();
	device->getDeviceContext()->PSSetShaderResources(0, 1, &glowResultView);

	//Set buffer
	vSize = sizeof(float) * compute->getValuesPerVertex();
	offset = 0;
	tempBuffer = compute->getVertexBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);

	//Set inputLayout
	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayout());

	//Map constant buffer
	blendQ->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set blend state
	device->getDeviceContext()->OMSetBlendState(nullptr, blendValues, 0xffffffff);

	//Draw BlendQuad
	device->getDeviceContext()->Draw(compute->getNrOfVertex(), 0);

}

void SetViewport(ID3D11DeviceContext* dContext) {

	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	dContext->RSSetViewports(1, &vp);

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