#include"Geometry.h"

Geometry::Geometry() {

	this->shader = nullptr;
	this->constBuffer = nullptr;

	this->mat.wvp = DirectX::XMMatrixIdentity();
	this->mat.world = DirectX::XMMatrixIdentity();

}

Geometry::~Geometry() {

	this->shader->Release();

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

}

void Geometry::createConstBuffer(ID3D11Device* device) {

	HRESULT hr = 0;

	//Buffer desc
	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(Matrix);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	//Check if failed
	hr = device->CreateBuffer(&constBufferDesc, nullptr, &this->constBuffer);

	if (FAILED(hr)) {

		// handle the error, could be fatal or a warning...
		exit(-1);

	}

}

void Geometry::updateMatrixValues(DirectX::XMMATRIX view, int wWidth, int wHeight) {
	
	//World offset pos by -1500 and scale by 400x
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixScaling(400, 400, 400) *  DirectX::XMMatrixTranslation(-1500.0f, -1500.0f, -1500.0f);
	mat.world = world;

	//Projection matrix
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH((DirectX::XM_PI * 0.45f), (wWidth / wHeight), 0.1f, 10000.0f);

	//World * View * Projection Matrix
	mat.wvp = DirectX::XMMatrixTranspose(world * view * proj);

}

void Geometry::mapConstBuffer(ID3D11DeviceContext* dContext) {

	//Map buffer
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	dContext->Map(this->constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &this->mat, sizeof(Matrix));
	dContext->Unmap(this->constBuffer, 0);

	//Set rescources to shaders
	dContext->GSSetConstantBuffers(0, 1, &this->constBuffer);

}

ID3D11GeometryShader* Geometry::getShader(void) {

	return this->shader;

}

ID3D11Buffer* Geometry::getConstBuffer(void) {

	return this->constBuffer;

}