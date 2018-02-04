#include"Device.h"

Device::Device() {

	this->swapChain = nullptr;
	this->dContext = nullptr;
	this->device = nullptr;
	this->backBufferRTV = nullptr;
	this->depthStencilV = nullptr;

}

Device::~Device() {

	this->swapChain->Release();
	this->device->Release();
	this->dContext->Release();
	this->backBufferRTV->Release();
	this->depthStencilV->Release();

}

HRESULT Device::createContext(HWND wHandle, int wWidth, int wHeight) {

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
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scD,
		&this->swapChain,
		&this->device,
		NULL,
		&this->dContext);

	//If successful create back buffer render view
	if (SUCCEEDED(hr)) {

		//Create depthStencil
		this->createDepthStencil(wWidth, wHeight);

		//Get address of back buffer
		ID3D11Texture2D* t_backBuffer = nullptr;
		this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t_backBuffer);

		//Use back buffer adress to create RTV
		this->device->CreateRenderTargetView(t_backBuffer, NULL, &this->backBufferRTV);
		t_backBuffer->Release();

		//Set the RTV as the back buffer
		//this->dContext->OMSetRenderTargets(1, &this->backBufferRTV, nullptr);
		this->dContext->OMSetRenderTargets(1, &this->backBufferRTV, this->depthStencilV);

	}

	return hr;

}

void Device::createDepthStencil(int wWidth, int wHeight) {

	HRESULT hr = 0;

	//DepthStencil Desc
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//Stencil flags
	depthDesc.StencilEnable = false;
	depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//frontface flags
	depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//backface flags
	depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create depth stencil
	ID3D11DepthStencilState* depthState = nullptr;
	hr = this->device->CreateDepthStencilState(&depthDesc, &depthState);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Bind depth stencil state
	this->dContext->OMSetDepthStencilState(depthState, 1);

	depthState->Release();

	//Stencil texture and texture desc
	ID3D11Texture2D* pDepthStencil = nullptr;

	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.Width = wWidth;
	depthTextureDesc.Height = wHeight;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.MipLevels = 0;

	//Create depthStencil texture
	hr = this->device->CreateTexture2D(&depthTextureDesc, NULL, &pDepthStencil);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Depth stencil view flags
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.Flags = 0;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	//Create stencil view
	hr = this->device->CreateDepthStencilView(pDepthStencil, &depthViewDesc, &this->depthStencilV);

	if (FAILED(hr)) {

		exit(-1);

	}

	//Release depthStencil texture
	pDepthStencil->Release();

}

ID3D11DeviceContext* Device::getDeviceContext(void) {

	return this->dContext;

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

ID3D11DepthStencilView* Device::getDepthStencil(void) {

	return this->depthStencilV;

}