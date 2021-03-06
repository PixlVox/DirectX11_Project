#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include"WICTextureLoader.h"
#include<vector>

class BlendQuad {

private:

	ID3D11Buffer* vBuffer;
	ID3D11Buffer* cBuffer;
	ID3D11ShaderResourceView* windowView;
	ID3D11BlendState* blendState;

	struct Matrix {

		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX world;

	};

	Matrix mat;

	float scaling;
	float offset;

	int nrOfVertices;
	int valuesPerVertex;

public:

	BlendQuad();
	~BlendQuad();

	void createBuffer(ID3D11Device* device);
	void createTexture(ID3D11Device* device);
	void createConstBuffer(ID3D11Device* device);
	void createBlendState(ID3D11Device* device);

	void updateMatrix(DirectX::XMMATRIX view, DirectX::XMMATRIX proj,
		long wWidth, long wHeight);
	void mapConstBuffer(ID3D11DeviceContext* dContext);

	//Get
	ID3D11Buffer* getBuffer(void);
	ID3D11ShaderResourceView* getWindowView(void);
	ID3D11BlendState* getBlendState(void);
	int getNrOfVertices(void) const;
	int getValuesPerVertex(void) const;

};