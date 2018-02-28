#ifndef TERRAIN_H
#define TERRAIN_H

#include <DirectXMath.h>
#include <d3d11.h>
#include<stdio.h>
#include<vector>
#include"WICTextureLoader.h"

class Terrain {

private:

	//Heightmap struct for loading in data
	struct HeightMap {

		int width;
		int height;
		DirectX::XMFLOAT3** vertexData;

	};

	//Constant buffer struct
	struct Matrix {

		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX world;

	};

	//Matrix & values for world matrix
	Matrix mat;
	float worldScale;
	float worldOffset;

	//Heightmap values
	HeightMap hM;
	int nrOfFaces;
	int nrOfVertices;
	int valuesPerVertex;

	//Texture & Sampler
	ID3D11SamplerState* sampState;
	ID3D11ShaderResourceView* grassView;
	ID3D11ShaderResourceView* stoneView;

	//Buffers
	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;
	ID3D11Buffer* cBuffer;

	//Private functions
	bool loadHeightMap();
	void createBuffers(ID3D11Device* device);

public:

	Terrain();
	~Terrain();

	void initHeightMap(ID3D11Device* device);
	void createTexture(ID3D11Device* device);
	void createSamplerState(ID3D11Device* device);
	void createConstBuffer(ID3D11Device* device);
	void mapConstBuffer(ID3D11DeviceContext* dContext);
	void updateMatrixValues(DirectX::XMMATRIX view, DirectX::XMMATRIX proj, 
		int wWidth, int wHeight);

	//Get
	int getNrOfFaces(void) const;
	int getNrOfVertices(void) const;
	int getValuesPerVertex(void) const;
	float getHeightValueAtPos(float x, float z) const;

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	ID3D11ShaderResourceView* getGrassView(void);
	ID3D11ShaderResourceView* getStoneView(void);
	ID3D11SamplerState* getSamplerState(void);

};

#endif // !TERRAIN_H
