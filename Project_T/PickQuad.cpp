#include"PickQuad.h"

PickQuad::PickQuad() {

	this->vBuffer = nullptr;
	this->cBuffer = nullptr;
	this->valuesPerVertex = 0;
	this->nrOfVertices = 0;
	this->color = { 1.0f, 0.0f, 0.0f };
	this->scaling = 2000;
	this->offset = { 0.0f, 0.0f, 5000.0f };

}

PickQuad::~PickQuad() {

	this->vBuffer->Release();
	this->cBuffer->Release();

}

void PickQuad::createBuffer(ID3D11Device* device) {

	//Init vars
	this->valuesPerVertex = 9;
	this->nrOfVertices = 6;
	std::vector<Vertex> vec(this->nrOfVertices);

	//Set pos and UV
	vec[0].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vec[0].color = this->color;

	vec[1].pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vec[1].color = this->color;

	vec[2].pos = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vec[2].color = this->color;

	vec[3].pos = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vec[3].color = this->color;

	vec[4].pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vec[4].color = this->color;

	vec[5].pos = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	vec[5].color = this->color;

	//Set normal

	for (int i = 0; i < this->nrOfVertices; i++) {

		vec[i].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

	}

	//Copy content to quadData
	this->quadData = vec;

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	ZeroMemory(&vBufferDesc, sizeof(vBufferDesc));

	//Flags
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(Vertex) * this->nrOfVertices;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	//Data
	D3D11_SUBRESOURCE_DATA vBufferData;
	ZeroMemory(&vBufferData, sizeof(vBufferData));

	vBufferData.pSysMem = &vec[0];
	device->CreateBuffer(&vBufferDesc, &vBufferData, &this->vBuffer);

}

void PickQuad::createConstBuffer(ID3D11Device* device) {


	HRESULT hr = 0;

	//Buffer desc
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(Matrix);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	//Check if failed
	hr = device->CreateBuffer(&cBufferDesc, nullptr, &this->cBuffer);

	if (FAILED(hr)) {

		exit(-1);

	}

}

void PickQuad::updateBuffer(ID3D11Device* device) {

	//Update color
	if (this->color.x == 1.0f) {

		this->color = { 0.0f, 1.0f, 0.0f };

	}
	else if (this->color.y == 1.0f) {

		this->color = { 0.0f, 0.0f, 1.0f };

	}
	else if (this->color.z == 1.0f) {

		this->color = { 1.0f, 0.0f, 0.0f };

	}

	//Rebuild buffer with new color
	for (int i = 0; i < this->nrOfVertices; i++) {

		this->quadData[i].color = this->color;

	}

	if (this->vBuffer != nullptr) {

		this->vBuffer->Release();

	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vBufferDesc;
	ZeroMemory(&vBufferDesc, sizeof(vBufferDesc));

	//Flags
	vBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vBufferDesc.ByteWidth = sizeof(Vertex) * this->nrOfVertices;
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBufferDesc.CPUAccessFlags = 0;
	vBufferDesc.MiscFlags = 0;

	//Data
	D3D11_SUBRESOURCE_DATA vBufferData;
	ZeroMemory(&vBufferData, sizeof(vBufferData));

	vBufferData.pSysMem = &this->quadData[0];
	device->CreateBuffer(&vBufferDesc, &vBufferData, &this->vBuffer);

}

bool PickQuad::intersect(DirectX::XMVECTOR mouseDir, DirectX::XMVECTOR camPos) {

	//Ray/Plane intersection
	bool intersects = false;
	bool stopSearch = false;
	float dist = 0.0f;

	for (int i = 0; i < this->nrOfVertices && !stopSearch; i+=3) {

		DirectX::XMVECTOR v0 = { this->quadData[i].pos.x, this->quadData[i].pos.y, this->quadData[i].pos.z };
		v0 = DirectX::XMVector3Transform(v0, this->mat.world);

		DirectX::XMVECTOR v1 = { this->quadData[i + 1].pos.x, this->quadData[i + 1].pos.y, this->quadData[i + 1].pos.z };
		v1 = DirectX::XMVector3Transform(v1, this->mat.world);

		DirectX::XMVECTOR v2 = { this->quadData[i + 2].pos.x, this->quadData[i + 2].pos.y, this->quadData[i + 2].pos.z };
		v2 = DirectX::XMVector3Transform(v2, this->mat.world);

		if (DirectX::TriangleTests::Intersects(camPos, mouseDir, v0, v1, v2, dist)) {

			intersects = true;
			stopSearch = true;

		}

	}

	return intersects;

}

ID3D11Buffer* PickQuad::getBuffer(void) {

	return this->vBuffer;

}

int PickQuad::getValuesPerVertex(void) const{

	return this->valuesPerVertex;

}

int PickQuad::getNrOfVertices(void) const{

	return this->nrOfVertices;

}

void PickQuad::updateMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj,
	long wWidth, long wHeight) {

	//World offset and scale
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixScaling(this->scaling, this->scaling, this->scaling) * 
		DirectX::XMMatrixTranslation(this->offset.x, this->offset.y, this->offset.z);
	mat.world = world;

	//World * View * Projection Matrix
	mat.wvp = DirectX::XMMatrixTranspose(world * view * proj);

}

void PickQuad::mapConstBuffer(ID3D11DeviceContext* dContext) {

	//Map buffer
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	dContext->Map(this->cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &this->mat, sizeof(Matrix));
	dContext->Unmap(this->cBuffer, 0);

	//Set rescources to shaders
	dContext->GSSetConstantBuffers(0, 1, &this->cBuffer);

}