#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include<DirectXCollision.h>
#include<vector>

class PickQuad {

private:

	ID3D11Buffer* vBuffer;
	ID3D11Buffer* cBuffer;

	struct Matrix {

		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX world;

	};

	struct Vertex {

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT3 normal;

	};

	Matrix mat;
	std::vector<Vertex> quadData;

	float scaling;
	DirectX::XMFLOAT3 offset;
	DirectX::XMFLOAT3 color;

	int nrOfVertices;
	int valuesPerVertex;

public:
	
	PickQuad();
	~PickQuad();

	void createBuffer(ID3D11Device* device);
	void createConstBuffer(ID3D11Device* device);

	void updateBuffer(ID3D11Device* device);
	void updateMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, 
		long wWidth, long wHeight);
	void mapConstBuffer(ID3D11DeviceContext* dContext);

	bool intersect(DirectX::XMVECTOR mouseDir, DirectX::XMVECTOR camPos);

	//Get
	ID3D11Buffer* getBuffer(void);
	int getNrOfVertices(void) const;
	int getValuesPerVertex(void) const;

};