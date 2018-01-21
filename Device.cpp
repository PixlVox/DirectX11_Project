#include"Device.h"

Device::Device() {

	this->swapChain = nullptr;
	this->deviceContext = nullptr;
	this->device = nullptr;
	this->backBufferRTV = nullptr;

}

Device::~Device() {

	this->swapChain->Release();
	this->device->Release();
	this->deviceContext->Release();
	this->backBufferRTV->Release();

}

HRESULT Device::createContext(HWND wHandle) {

	//Create swap chain desc
	DXGI_SWAP_CHAIN_DESC scD;

	//Clear desc struct
	ZeroMemory(&scD, sizeof(DXGI_SWAP_CHAIN_DESC));

	//Set info
	scD.BufferCount = 1;
	scD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//32-bit color
	scD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//Swap chain usage
	scD.OutputWindow = wHandle;							//Set window to use
	scD.SampleDesc.Count = 1;							//Nr of multisamples
	scD.Windowed = TRUE;								//Windowed/Full screen

	//Create device, deviceContext & swap chain using scD
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scD,
		&this->swapChain,
		&this->device,
		NULL,
		&this->deviceContext);

	//If successful create back buffer render view
	if (SUCCEEDED(hr)) {

		//Get address of back buffer
		ID3D11Texture2D* t_backBuffer = nullptr;
		this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t_backBuffer);

		//Use back buffer adress to create RTV
		this->device->CreateRenderTargetView(t_backBuffer, NULL, &this->backBufferRTV);
		t_backBuffer->Release();

		//Set the RTV as the back buffer
		this->deviceContext->OMSetRenderTargets(1, &this->backBufferRTV, NULL);

	}

	return hr;

}

ID3D11DeviceContext* Device::getDeviceContext(void) {

	return this->deviceContext;

}

IDXGISwapChain* Device::getSwapChain(void) {

	return this->swapChain;

}

ID3D11Device* Device::getDevice(void) {

	return this->device;

}

ID3D11RenderTargetView* Device::getBackBufferRTV(void) {

	return this->backBufferRTV;

}