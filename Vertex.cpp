#include"Vertex.h"

Vertex::Vertex() {

	this->buffer = nullptr;
	this->shader = nullptr;
	this->inputLayout = nullptr;
	this->nrOfVertices = 0;

}

Vertex::~Vertex() {

	this->buffer->Release();
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->inputLayout);

	pVS->Release();

}

void Vertex::createTriangleData(ID3D11Device* device) {

	//Define the triangle vertices
	struct TriangleVertex {

		float x, y, z;
		float r, g, b;

	};

	this->nrOfVertices = 6;
	TriangleVertex triangleVertices[6]{

		//Triangle 1
		-0.25f, 0.25f, 0.0f,//v0 pos
		1.0f, 0.0f, 0.0f, 	//v0 color

		0.25f, -0.25f, 0.0f,//v1 pos
		0.0f, 1.0f,	0.0f,	//v1 color

		-0.25f, -0.25f, 0.0f,//v2 pos
		0.0f, 0.0f,	1.0f,	//v2 color

		//Triangle 2
		0.25f, -0.25f, 0.0f,//v0 pos
		1.0f, 1.0f,	0.0f	//v0 color

		- 0.25f, 0.25f, 0.0f,//v1 pos
		0.0f, 1.0f, 1.0f,	//v1 color

		0.25f, 0.25f, 0.0f, //v2 pos
		1.0f, 0.0f, 1.0f	//v2 color

	};

	//Create buffer desc
	D3D11_BUFFER_DESC bDesc;
	memset(&bDesc, 0, sizeof(bDesc));
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.Usage = D3D11_USAGE_DEFAULT;
	bDesc.ByteWidth = sizeof(triangleVertices);

	//Create data
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	device->CreateBuffer(&bDesc, &data, &this->buffer);

}

ID3D11VertexShader* Vertex::getShader(void) {

	return this->shader;

}

int Vertex::getNrOfVertex(void) const{

	return this->nrOfVertices;

}

ID3D11Buffer* Vertex::getBuffer(void) {

	return this->buffer;

}

ID3D11InputLayout* Vertex::getInputLayout(void) {

	return this->inputLayout;

}