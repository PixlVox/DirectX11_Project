#include"Vertex.h"

Vertex::Vertex() {

	this->shader = nullptr;
	this->inputLayout = nullptr;
	this->nrOfVertices = 0;

}

Vertex::~Vertex() {

	this->shader->Release();
	this->inputLayout->Release();

}

void Vertex::createShader(ID3D11Device* device) {

	//Create the vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"VertexShader.hlsl",	//Filename
		nullptr,				//Macros
		nullptr,				//Include files
		"VS_main",				//Entry point
		"vs_5_0",				//Model
		0,						//Shader compile options
		0,						//Effect compile options
		&pVS,					//Double pointer to Blob
		nullptr);				//Pointer for error blob message

	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->shader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->inputLayout);

	pVS->Release();

}

void Vertex::createTriangleData(ID3D11Device* device) {

	//Load in values from bitmap
	this->terrain.loadHeightMap();

	//Create vertex & index buffer and store vertex info/index info
	this->terrain.createBuffers(device);

}

ID3D11VertexShader* Vertex::getShader(void) {

	return this->shader;

}

int Vertex::getNrOfVertex(void) const{

	return this->terrain.getNrOfVertices();

}

int Vertex::getNrOfFaces(void) const {

	return this->terrain.getNrOfFaces();

}

ID3D11Buffer* Vertex::getVertexBuffer(void) {

	return this->terrain.getVertexBuffer();

}

ID3D11Buffer* Vertex::getIndexBuffer(void) {

	return this->terrain.getIndexBuffer();

}

ID3D11InputLayout* Vertex::getInputLayout(void) {

	return this->inputLayout;

}