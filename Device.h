#ifndef DEVICE_H
#define DEVICE_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Device {

private:

	ID3D11Device* device;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* backBufferRTV;

public:

	Device();
	~Device();

	HRESULT createContext(HWND wHandle);

	//Get
	ID3D11DeviceContext* getDeviceContext(void);
	IDXGISwapChain* getSwapChain(void);
	ID3D11Device* getDevice(void);
	ID3D11RenderTargetView* getBackBufferRTV(void);
};

#endif // !DEVICE_H

