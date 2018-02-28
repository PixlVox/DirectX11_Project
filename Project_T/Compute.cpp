#include"Compute.h"

Compute::Compute() {

	this->shader = nullptr;

	this->glowBuffer = nullptr;
	this->glowMapBuffer = nullptr;
	this->outputBuffer = nullptr;
	this->vertexBuffer = nullptr;

	this->glowResource = nullptr;
	this->glowMapResource = nullptr;

	this->glowView = nullptr;
	this->glowMapView = nullptr;
	this->outputView = nullptr;

	this->glowData = nullptr;
	this->glowMapData = nullptr;

	this->nrOfVertex = 0;
	this->valuesPerVertex = 0;
	this->glowSize = 0;

}

Compute::~Compute() {

	this->shader->Release();

	this->glowBuffer->Release();
	this->glowMapBuffer->Release();
	this->outputBuffer->Release();
	this->vertexBuffer->Release();

	this->glowResource->Release();
	this->glowMapResource->Release();

	this->glowView->Release();
	this->glowMapView->Release();
	this->outputView->Release();

	delete[] this->glowData;

}

void Compute::createShader(ID3D11Device* device) {

	//Create the vertex shader
	ID3DBlob* pCS = nullptr;
	D3DCompileFromFile(
		L"ComputeShader.hlsl",	//Filename
		nullptr,				//Macros
		nullptr,				//Include files
		"CS_main",				//Entry point
		"cs_5_0",				//Model
		0,						//Shader compile options
		0,						//Effect compile options
		&pCS,					//Double pointer to Blob
		nullptr);				//Pointer for error blob message

	device->CreateComputeShader(pCS->GetBufferPointer(), pCS->GetBufferSize(), nullptr, &this->shader);
	pCS->Release();

}

void Compute::createBuffers(ID3D11Device* device, ID3D11DeviceContext* dContext) {

	//Buffer for glow texture
	this->createInputBuffer(device, dContext, 0, this->glowView, 
		this->glowData, this->glowBuffer);

	//Buffer for glowMap texture
	this->createInputBuffer(device, dContext, 1, this->glowMapView, 
		this->glowMapData, this->glowMapBuffer);

	//Output buffer
	this->createOutputBuffer(device);

}

void Compute::createInputBuffer(ID3D11Device* device, ID3D11DeviceContext* dContext, int textureType, 
	ID3D11ShaderResourceView* view, byte* byteData, ID3D11Buffer* buffer) {

	ID3D11Texture2D* tempTexture = nullptr;
	ID3D11Texture2D* texture = nullptr;

	//Load texture
	if (textureType == 0) {

		DirectX::CreateWICTextureFromFile(device, L"Textures//GlowTexture.png", &this->glowResource, nullptr);
		DirectX::CreateWICTextureFromFile(device, L"Textures//GlowTexture.png", nullptr, &view);
		texture = static_cast<ID3D11Texture2D*>(this->glowResource);

	}
	else if (textureType == 1) {

		DirectX::CreateWICTextureFromFile(device, L"Textures//GlowMapTexture.png", &this->glowMapResource, nullptr);
		DirectX::CreateWICTextureFromFile(device, L"Textures//GlowMapTexture.png", nullptr, &view);
		texture = static_cast<ID3D11Texture2D*>(this->glowMapResource);

	}

	//Create texture desc
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.BindFlags = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	//Create temptexture and cppy texture data into it
	device->CreateTexture2D(&textureDesc, NULL, &tempTexture);
	dContext->CopyResource(tempTexture, texture);

	//Map Temptexture and copy into byte array
	D3D11_MAPPED_SUBRESOURCE mapR;
	dContext->Map(tempTexture, 0, D3D11_MAP_READ, 0, &mapR);
	this->glowSize = mapR.RowPitch * textureDesc.Height;
	byteData = new byte[this->glowSize];
	memcpy(byteData, mapR.pData, this->glowSize);

	//Unmap tempTexture
	dContext->Unmap(tempTexture, 0);

	//Create buffer desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = this->glowSize;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = 4;		//RGBA Format

	//Create data
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = byteData;
	device->CreateBuffer(&bufferDesc, &data, &buffer);

	//Create buffer for resource view
	D3D11_BUFFER_DESC viewBufferDesc;
	ZeroMemory(&viewBufferDesc, sizeof(viewBufferDesc));
	buffer->GetDesc(&viewBufferDesc);

	//Create shader resource view desc
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	viewDesc.BufferEx.FirstElement = 0;
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.BufferEx.NumElements = (viewBufferDesc.ByteWidth / viewBufferDesc.StructureByteStride);

	device->CreateShaderResourceView(buffer, &viewDesc, &view);

}

void Compute::createOutputBuffer(ID3D11Device* device) {

	//Create buffer desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = this->glowSize;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = 4;	//RGBA Format

	device->CreateBuffer(&bufferDesc, NULL, &this->outputBuffer);

	//Create buffer desc for UAV
	D3D11_BUFFER_DESC bufferUAVDesc;
	ZeroMemory(&bufferUAVDesc, sizeof(bufferUAVDesc));
	this->glowBuffer->GetDesc(&bufferUAVDesc);

	//Create UAV view desc
	D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	viewDesc.Buffer.FirstElement = 0;
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.Buffer.NumElements = (bufferUAVDesc.ByteWidth / bufferUAVDesc.StructureByteStride);

	device->CreateUnorderedAccessView(this->outputBuffer, &viewDesc, &this->outputView);

}

void Compute::saveResult(ID3D11Device* device, ID3D11DeviceContext* dContext) {

	//Create texture
	ID3D11Texture2D* tempTexture;
	ID3D11Texture2D* tempTextureGlow = static_cast<ID3D11Texture2D*>(this->glowResource);

	//Create texture desc
	D3D11_TEXTURE2D_DESC textureDesc;
	tempTextureGlow->GetDesc(&textureDesc);
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.MipLevels = 1;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateTexture2D(&textureDesc, NULL, &tempTexture);

	//Copy data
	D3D11_MAPPED_SUBRESOURCE mapR;
	dContext->Map(tempTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapR);

	//Create copy of result
	ID3D11Buffer* copyBuffer = NULL;

	//Buffer desc
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	this->outputBuffer->GetDesc(&bufferDesc);

	UINT size = bufferDesc.ByteWidth;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.MiscFlags = 0;

	//Create buffer
	device->CreateBuffer(&bufferDesc, NULL, &copyBuffer);

	//Copy over data from outputBuffer
	dContext->CopyResource(copyBuffer, this->outputBuffer);

	//Map data into byte array
	D3D11_MAPPED_SUBRESOURCE copyMapR;
	dContext->Map(copyBuffer, 0, D3D11_MAP_READ, 0, &copyMapR);

	//Copy over data
	byte* copy = new byte(size);
	memcpy(copy, copyMapR.pData, size);
	dContext->Unmap(copyBuffer, 0);
	copyBuffer->Release();

	//Copy over texture
	memcpy(mapR.pData, copy, this->glowSize);

	//Unmap texture
	dContext->Unmap(tempTexture, 0);

	//Create a shader resource view for the result
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(tempTexture, &viewDesc, &this->resultView);

}

void Compute::createVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* dContext) {

	struct Vertex {

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texUV;
		DirectX::XMFLOAT3 normal;

	};
	this->valuesPerVertex = 8;
	this->nrOfVertex = 6;

	std::vector<Vertex> vec(this->nrOfVertex);

	//Set pos and UV
	vec[0].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f);
	vec[0].texUV = DirectX::XMFLOAT2(0.0f, 0.0f);

	vec[1].pos = DirectX::XMFLOAT3(0.0f, 2.0f, 10.0f);
	vec[1].texUV = DirectX::XMFLOAT2(0.0f, 1.0f);

	vec[2].pos = DirectX::XMFLOAT3(2.0f, 0.0f, 10.0f);
	vec[2].texUV = DirectX::XMFLOAT2(1.0f, 0.0f);

	vec[3].pos = DirectX::XMFLOAT3(2.0f, 0.0f, 10.0f);
	vec[3].texUV = DirectX::XMFLOAT2(1.0f, 0.0f);

	vec[4].pos = DirectX::XMFLOAT3(0.0f, 2.0f, 10.0f);
	vec[4].texUV = DirectX::XMFLOAT2(0.0f, 1.0f);

	vec[5].pos = DirectX::XMFLOAT3(2.0f, 2.0f, 10.0f);
	vec[5].texUV = DirectX::XMFLOAT2(1.0f, 1.0f);

	//Set normal
	DirectX::XMVECTOR e1 = { 0.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR e2 = { 0.0f, 0.0f, 0.0f };

	DirectX::XMVectorSetX(e1, (vec[1].pos.x - vec[0].pos.x));
	DirectX::XMVectorSetY(e1, (vec[1].pos.y - vec[0].pos.y));
	DirectX::XMVectorSetZ(e1, (vec[1].pos.z - vec[0].pos.z));

	DirectX::XMVectorSetX(e2, (vec[2].pos.x - vec[0].pos.x));
	DirectX::XMVectorSetY(e2, (vec[2].pos.y - vec[0].pos.y));
	DirectX::XMVectorSetZ(e2, (vec[2].pos.z - vec[0].pos.z));

	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(e1, e2);

	for (int i = 0; i < this->nrOfVertex; i++) {

		vec[i].normal = DirectX::XMFLOAT3(DirectX::XMVectorGetX(normal),
			DirectX::XMVectorGetY(normal), DirectX::XMVectorGetZ(normal));

	}

	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * this->nrOfVertex;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &vec[0];

	device->CreateBuffer(&bufferDesc, &data, &this->vertexBuffer);

}

ID3D11ComputeShader* Compute::getShader(void) {

	return this->shader;

}

ID3D11Buffer* Compute::getVertexBuffer(void) {

	return this->vertexBuffer;

}

ID3D11ShaderResourceView* Compute::getGlowView(void) {

	return this->glowView;

}

ID3D11ShaderResourceView* Compute::getGlowMapView(void) {

	return this->glowMapView;

}

ID3D11ShaderResourceView* Compute::getResultView(void) {
	
	return this->resultView;

}

ID3D11UnorderedAccessView* Compute::getOutputView(void) {

	return this->outputView;

}

int Compute::getValuesPerVertex(void) const{

	return this->valuesPerVertex;

}

int Compute::getNrOfVertex(void) const{

	return this->nrOfVertex;

}