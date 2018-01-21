#include <windows.h>
#include<iostream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include"Device.h"
#include"Vertex.h"
#include"Geometry.h"
#include"Pixel.h"

const LONG WIDTH = 1200;
const LONG HEIGHT = 700;

HWND initWindow(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetViewport(ID3D11DeviceContext* dContext);
void Render(ID3D11DeviceContext* dContext, ID3D11VertexShader* vS, int nrOfVertex,
	ID3D11Buffer* vBuffer, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* backBufferRTV,
	ID3D11GeometryShader* gS, ID3D11PixelShader* pS);

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {

	//Detect memoryleaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	MSG msg = { 0 };

	//Create window
	HWND wndHandle = initWindow(hInst);

	if (wndHandle) {

		//Create objects
		Device device;
		Vertex vertex;
		Geometry geometry;
		Pixel pixel;
		HRESULT hr = NULL;

		//Create device object(device, deviceContext, swapChain, backBufferRVT)
		hr = device.createContext(wndHandle);

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

		//Sets state of window
		ShowWindow(wndHandle, nCmdShow);

		//Updates the program
		while (WM_QUIT != msg.message) {

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			}
			else {

				//Render
				Render(device.getDeviceContext(),
					vertex.getShader(),
					vertex.getNrOfVertex(),
					vertex.getBuffer(),
					vertex.getInputLayout(),
					device.getBackBufferRTV(),
					geometry.getShader(),
					pixel.getShader());

				//Swap front & back buffers
				device.getSwapChain()->Present(0, 0);

			}
		}
	}

		return (int)msg.wParam;

}

void Render(ID3D11DeviceContext* dContext, ID3D11VertexShader* vS, int nrOfVertex,
	ID3D11Buffer* vBuffer, ID3D11InputLayout* inputLayout, ID3D11RenderTargetView* backBufferRTV,
	ID3D11GeometryShader* gS, ID3D11PixelShader* pS) {

	//Set a default color
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//Set rendering state
	dContext->VSSetShader(vS, nullptr, 0);
	dContext->HSSetShader(nullptr, nullptr, 0);
	dContext->DSSetShader(nullptr, nullptr, 0);
	dContext->GSSetShader(gS, nullptr, 0);
	dContext->PSSetShader(pS, nullptr, 0);

	//Set vertex size and offset
	UINT32 vSize = sizeof(float) * nrOfVertex;
	UINT32 offset = 0;

	//Set vertex buffer and inputLayout
	dContext->IASetVertexBuffers(0, 1, &vBuffer, &vSize, &offset);
	dContext->IASetInputLayout(inputLayout);

	//Set primitive type
	dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Clear screen
	dContext->ClearRenderTargetView(backBufferRTV, clearColor);

	//Draw
	dContext->Draw(nrOfVertex, 0);

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