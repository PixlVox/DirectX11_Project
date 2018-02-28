#include"BlendQuad.h"

BlendQuad::BlendQuad() {

	this->vBuffer = nullptr;
	this->cBuffer = nullptr;
	this->windowView = nullptr;
	this->valuesPerVertex = 0;
	this->nrOfVertices = 0;

}

BlendQuad::~BlendQuad() {

	this->vBuffer->Release();
	this->windowView->Release();

}

void BlendQuad::createBuffer(ID3D11Device* device) {

	//Vertex struct
	struct Vertex {

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texUV;
		DirectX::XMFLOAT3 normal;

	};

	//Init vars
	this->valuesPerVertex = 8;
	this->nrOfVertices = 6;
	std::vector<Vertex> vec(this->nrOfVertices);

	//Create vertex points

	//Set pos and UV
	vec[0].pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	vec[0].texUV = DirectX::XMFLOAT2(0.0f, 0.0f);

	vec[1].pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vec[1].texUV = DirectX::XMFLOAT2(0.0f, 1.0f);

	vec[2].pos = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vec[2].texUV = DirectX::XMFLOAT2(1.0f, 0.0f);

	vec[3].pos = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vec[3].texUV = DirectX::XMFLOAT2(1.0f, 0.0f);

	vec[4].pos = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vec[4].texUV = DirectX::XMFLOAT2(0.0f, 1.0f);

	vec[5].pos = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
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

	for (int i = 0; i < this->nrOfVertices; i++) {

		vec[i].normal = DirectX::XMFLOAT3(DirectX::XMVectorGetX(normal),
			DirectX::XMVectorGetY(normal), DirectX::XMVectorGetZ(normal));

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

	vBufferData.pSysMem = &vec[0];
	device->CreateBuffer(&vBufferDesc, &vBufferData, &this->vBuffer);

}

void BlendQuad::createTexture(ID3D11Device* device) {

	//Create Grass texture view
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Textures//Window.png",
		nullptr, &this->windowView);

	if (FAILED(hr)) {

		exit(-1);

	}

}

void BlendQuad::createBlendState(ID3D11Device* device) {

	//Setup blending state for alpha blending
	//(source * sourceAlpha) + (destination * inverseDestinationAlpha)
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&blendDesc, &this->blendState);

	if (FAILED(hr)) {

		exit(-1);

	}

}


ID3D11Buffer* BlendQuad::getBuffer(void) {

	return this->vBuffer;

}

ID3D11ShaderResourceView* BlendQuad::getWindowView(void) {

	return this->windowView;

}

int BlendQuad::getNrOfVertices(void) const{

	return this->nrOfVertices;

}

int BlendQuad::getValuesPerVertex(void) const{

	return this->valuesPerVertex;

}

void BlendQuad::createConstBuffer(ID3D11Device* device) {

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

void BlendQuad::updateMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, 
	long wWidth, long wHeight) {

	//World offset and scale
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world = DirectX::XMMatrixScaling(6400, 3200, 400) *  DirectX::XMMatrixTranslation(-1500.0f, -1500.0f, -1500.0f);
	mat.world = world;

	//World * View * Projection Matrix
	mat.wvp = DirectX::XMMatrixTranspose(world * view * proj);

}

void BlendQuad::mapConstBuffer(ID3D11DeviceContext* dContext) {

	//Map buffer
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	dContext->Map(this->cBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	memcpy(dataPtr.pData, &this->mat, sizeof(Matrix));
	dContext->Unmap(this->cBuffer, 0);

	//Set rescources to shaders
	dContext->GSSetConstantBuffers(0, 1, &this->cBuffer);

}

ID3D11BlendState* BlendQuad::getBlendState(void) {

	return this->blendState;

}