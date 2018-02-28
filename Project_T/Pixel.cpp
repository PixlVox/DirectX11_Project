#include"Pixel.h"

Pixel::Pixel() {

	this->shader = nullptr;
	this->colorShader = nullptr;
	this->blendShader = nullptr;

}

Pixel::~Pixel() {

	this->shader->Release();
	this->colorShader->Release();
	this->blendShader->Release();

}

void Pixel::createShader(ID3D11Device* device) {

	//Shader
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

	//Shader Color
	ID3DBlob* pPS_Color = nullptr;

	D3DCompileFromFile(
		L"PixelShader.hlsl",	// filename
		nullptr,				//macros
		nullptr,				//include files
		"PS_main_Color",		// entry point
		"ps_5_0",				// shader model
		0,						// shader compile options
		0,						// effect compile options
		&pPS_Color,				// double pointer to ID3DBlob		
		nullptr);				// pointer for Error Blob messages.

	device->CreatePixelShader(pPS_Color->GetBufferPointer(), pPS_Color->GetBufferSize(), nullptr, &this->colorShader);

	pPS_Color->Release();

	//Blend Shader
	ID3DBlob* pPS_Blend = nullptr;

	D3DCompileFromFile(
		L"PixelShader.hlsl",	// filename
		nullptr,				//macros
		nullptr,				//include files
		"PS_main_Blend",		// entry point
		"ps_5_0",				// shader model
		0,						// shader compile options
		0,						// effect compile options
		&pPS_Blend,				// double pointer to ID3DBlob		
		nullptr);				// pointer for Error Blob messages.

	device->CreatePixelShader(pPS_Blend->GetBufferPointer(), pPS_Blend->GetBufferSize(), nullptr, &this->blendShader);

	pPS_Blend->Release();

}

ID3D11PixelShader* Pixel::getShader() {

	return this->shader;

}

ID3D11PixelShader* Pixel::getColorShader() {

	return this->colorShader;

}

ID3D11PixelShader* Pixel::getBlendShader(void) {

	return this->blendShader;

}