#include"Geometry.h"

Geometry::Geometry() {

	this->shader = nullptr;
	this->shaderColor = nullptr;

}

Geometry::~Geometry() {

	this->shader->Release();
	this->shaderColor->Release();

}

void Geometry::createShader(ID3D11Device* device) {

	//Create shader
	ID3DBlob* pGS = nullptr;

	D3DCompileFromFile(
		L"GeometryShader.hlsl",	// filename
		nullptr,				//macros
		nullptr,				//include files
		"GS_main",				// entry point
		"gs_5_0",				// shader model
		0,						// shader compile options
		0,						// effect compile options
		&pGS,					// double pointer to ID3DBlob		
		nullptr);				// pointer for Error Blob messages.

	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &this->shader);

	pGS->Release();

	//Create shader Color
	ID3DBlob* pGS_Color = nullptr;

	D3DCompileFromFile(
		L"GeometryShader.hlsl",	// filename
		nullptr,				//macros
		nullptr,				//include files
		"GS_main_Color",		// entry point
		"gs_5_0",				// shader model
		0,						// shader compile options
		0,						// effect compile options
		&pGS_Color,				// double pointer to ID3DBlob		
		nullptr);				// pointer for Error Blob messages.

	device->CreateGeometryShader(pGS_Color->GetBufferPointer(), pGS_Color->GetBufferSize(), nullptr, &this->shaderColor);

	pGS_Color->Release();

}

ID3D11GeometryShader* Geometry::getShader(void) {

	return this->shader;

}

ID3D11GeometryShader* Geometry::getShaderColor(void) {

	return this->shaderColor;

}