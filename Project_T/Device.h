#ifndef DEVICE_H
#define DEVICE_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Device {

private:

	ID3D11Device* device;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* dContext;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilV;

public:

	Device();
	~Device();

	HRESULT createContext(HWND wHandle, int wWidth, int wHeight);
	void createDepthStencil(int wWIDTH, int wHeight);

	//Get
	ID3D11DeviceContext* getDeviceContext(void);
	IDXGISwapChain* getSwapChain(void);
	ID3D11Device* getDevice(void);
	ID3D11RenderTargetView* getBackBufferRTV(void);
	ID3D11DepthStencilView* getDepthStencil(void);
};

#endif // !DEVICE_H

