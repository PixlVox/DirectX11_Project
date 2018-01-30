#include"Pixel.h"

Pixel::Pixel() {

	this->shader = nullptr;

}

Pixel::~Pixel() {

	this->shader->Release();

}

void Pixel::createShader(ID3D11Device* device) {

	ID3DBlob* pPS = nullptr;

	D3DCompileFromFile(
		L"PixelShader.hlsl",	// filename
		nullptr,				//macros
		nullptr,				//include files
		"PS_main",					// entry point
		"ps_5_0",				// shader model
		0,						// shader compile options
		0,						// effect compile options
		&pPS,					// double pointer to ID3DBlob		
		nullptr);				// pointer for Error Blob messages.

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->shader);

	pPS->Release();

}

ID3D11PixelShader* Pixel::getShader() {

	return this->shader;

}