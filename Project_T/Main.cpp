#include <windows.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include"Device.h"
#include"Vertex.h"
#include"Geometry.h"
#include"Pixel.h"
#include"Camera.h"

const LONG WIDTH = 1200;
const LONG HEIGHT = 700;

HWND initWindow(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetViewport(ID3D11DeviceContext* dContext);
void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel, Camera* cam);

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

		//Create triangle vertices
		vertex.createTriangleData(device.getDevice());

		//Create constant buffer
		geometry.createConstBuffer(device.getDevice());

		//Create camera
		cam.initDI(hInst, wHandle);

		//Set values to WVP matrix(Pass window width/height)
		geometry.setMatrixValues(cam.getView(), (int)WIDTH, (int)HEIGHT);

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
				Render(&device, &vertex, &geometry, &pixel, &cam);

				//Swap front & back buffers
				device.getSwapChain()->Present(0, 0);

			}

		}
	}

		return (int)msg.wParam;

}

void Render(Device* device, Vertex* vertex, Geometry* geo, Pixel* pixel, Camera* cam) {

	//Set a default color
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//Set rendering state
	device->getDeviceContext()->VSSetShader(vertex->getShader(), nullptr, 0);
	device->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	device->getDeviceContext()->GSSetShader(geo->getShader(), nullptr, 0);
	device->getDeviceContext()->PSSetShader(pixel->getShader(), nullptr, 0);

	//Set vertex size and offset
	UINT32 vSize = sizeof(float) * 6;
	UINT32 offset = 0;

	//Update camera(view matrix)
	cam->getInput(0.0f);
	cam->update();

	//Update wvp matrix
	geo->updateMatrixValues(cam->getView(), (int)WIDTH, (int)HEIGHT);

	//Set vertex & index buffer
	//Create temp buffer for setting vertex buffer(cant pass it directly from object to functions for some reason)
	ID3D11Buffer* tempBuffer = vertex->getVertexBuffer();
	device->getDeviceContext()->IASetVertexBuffers(0, 1, &tempBuffer, &vSize, &offset);
	device->getDeviceContext()->IASetIndexBuffer(vertex->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	device->getDeviceContext()->IASetInputLayout(vertex->getInputLayout());

	//Map constant buffer
	geo->mapConstBuffer(device->getDeviceContext());

	//Set primitive type
	device->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Clear screen
	device->getDeviceContext()->ClearDepthStencilView(device->getDepthStencil(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device->getDeviceContext()->ClearRenderTargetView(device->getBackBufferRTV(), clearColor);

	//Draw
	device->getDeviceContext()->DrawIndexed((vertex->getNrOfFaces() * 3), 0, 0);

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