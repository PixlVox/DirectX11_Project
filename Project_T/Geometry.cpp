#include"Geometry.h"

Geometry::Geometry() {

	this->shader = nullptr;

}

Geometry::~Geometry() {

	this->shader->Release();

}

void Geometry::createShader(ID3D11Device* device) {

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

}

ID3D11GeometryShader* Geometry::getShader(void) {

	return this->shader;

}