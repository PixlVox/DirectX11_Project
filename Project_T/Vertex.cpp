#include"Vertex.h"

Vertex::Vertex() {

	this->shader = nullptr;
	this->inputLayout = nullptr;
	this->nrOfVertices = 0;
	this->valuesPerVertex = 0;

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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	this->valuesPerVertex = this->terrain.getValuesPerVertex();

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

int Vertex::getValuesPerVertex(void) const{

	return this->valuesPerVertex;

}

float Vertex::getHeightValueAtPos(float x, float z) {

	return this->terrain.getHeightValueAtPos(x, z);

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

void Vertex::createTextures(ID3D11Device* device) {

	this->terrain.createTexture(device);

}

ID3D11ShaderResourceView* Vertex::getGrassView(void) {

	return this->terrain.getGrassView();

}

ID3D11ShaderResourceView* Vertex::getStoneView(void) {

	return this->terrain.getStoneView();

}

ID3D11SamplerState* Vertex::getSamplerState() {

	return this->terrain.getSamplerState();

}

ID3D11BlendState* Vertex::getBlendState(void) {

	return this->terrain.getBlendState();

}

void Vertex::createSamplerState(ID3D11Device* device) {

	this->terrain.createSamplerState(device);

}

void Vertex::createBlendState(ID3D11Device* device) {

	this->terrain.createBlendState(device);

}