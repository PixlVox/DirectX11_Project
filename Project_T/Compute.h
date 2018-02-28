#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include"WICTextureLoader.h"
#include<vector>

class Compute {

private:

	ID3D11ComputeShader* shader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* glowBuffer;
	ID3D11Buffer* glowMapBuffer;
	ID3D11Buffer* outputBuffer;

	ID3D11Resource* glowResource;
	ID3D11Resource* glowMapResource;

	ID3D11ShaderResourceView* glowView;
	ID3D11ShaderResourceView* glowMapView;
	ID3D11ShaderResourceView* resultView;
	ID3D11UnorderedAccessView* outputView;

	byte* glowData;
	byte* glowMapData;

	int nrOfVertex;
	int valuesPerVertex;
	UINT glowSize;

	//Private functions
	void createOutputBuffer(ID3D11Device* device);
	void createInputBuffer(ID3D11Device* device, ID3D11DeviceContext* dContext,
		int textureType, ID3D11ShaderResourceView* view, byte* byteData,
		ID3D11Buffer* buffer);

public:

	Compute();
	~Compute();

	void createShader(ID3D11Device* device);
	void createBuffers(ID3D11Device* device, ID3D11DeviceContext* dContext);
	void saveResult(ID3D11Device* device, ID3D11DeviceContext* dContext);
	void createVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* dContext);

	//Get
	ID3D11ComputeShader* getShader(void);

	ID3D11Buffer* getVertexBuffer(void);

	ID3D11ShaderResourceView* getGlowView(void);
	ID3D11ShaderResourceView* getGlowMapView(void);
	ID3D11ShaderResourceView* getResultView(void);
	ID3D11UnorderedAccessView* getOutputView(void);

	int getValuesPerVertex(void) const;
	int getNrOfVertex(void) const;

};