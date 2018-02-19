#ifndef TERRAIN_H
#define TERRAIN_H

#include <DirectXMath.h>
#include <d3d11.h>
#include<stdio.h>
#include<vector>
#include"WICTextureLoader.h"

class Terrain {

private:

	struct HeightMap {

		int width;
		int height;
		DirectX::XMFLOAT3** vertexData;

	};

	float scalingValue;
	float offsetValue;

	HeightMap hM;
	int nrOfFaces;
	int nrOfVertices;
	int valuesPerVertex;

	//Texture
	ID3D11SamplerState* sampState;
	ID3D11ShaderResourceView* grassView;

	//Buffers
	ID3D11Buffer* vBuffer;
	ID3D11Buffer* iBuffer;

public:

	Terrain();
	~Terrain();

	bool loadHeightMap();
	void createBuffers(ID3D11Device* device);
	void createTexture(ID3D11Device* device);
	void createSamplerState(ID3D11Device* device);

	//Get
	int getNrOfFaces(void) const;
	int getNrOfVertices(void) const;
	int getValuesPerVertex(void) const;
	float getHeightValueAtPos(float x, float z) const;

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	ID3D11ShaderResourceView* getGrassView(void);
	ID3D11SamplerState* getSamplerState();

};

#endif // !TERRAIN_H
