#include"Vertex.h"

Vertex::Vertex() {

	this->shader = nullptr;
	this->shaderColor = nullptr;
	this->inputLayout = nullptr;
	this->inputLayoutColor = nullptr;

}

Vertex::~Vertex() {

	this->shader->Release();
	this->shaderColor->Release();
	this->inputLayout->Release();
	this->inputLayoutColor->Release();

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

	//Create the vertex color shader
	ID3DBlob* pVS_Color = nullptr;
	D3DCompileFromFile(
		L"VertexShader.hlsl",	//Filename
		nullptr,				//Macros
		nullptr,				//Include files
		"VS_main_Color",		//Entry point
		"vs_5_0",				//Model
		0,						//Shader compile options
		0,						//Effect compile options
		&pVS_Color,					//Double pointer to Blob
		nullptr);				//Pointer for error blob message

	device->CreateVertexShader(pVS_Color->GetBufferPointer(), pVS_Color->GetBufferSize(), nullptr, &this->shaderColor);

	D3D11_INPUT_ELEMENT_DESC inputDescColor[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inputDescColor, ARRAYSIZE(inputDescColor), pVS_Color->GetBufferPointer(), pVS_Color->GetBufferSize(), &this->inputLayoutColor);

	pVS_Color->Release();

}

ID3D11VertexShader* Vertex::getShader(void) {

	return this->shader;

}

ID3D11VertexShader* Vertex::getShaderColor(void) {

	return this->shaderColor;

}

ID3D11InputLayout* Vertex::getInputLayout(void) {

	return this->inputLayout;

}

ID3D11InputLayout* Vertex::getInputLayoutColor(void) {

	return this->inputLayoutColor;

}