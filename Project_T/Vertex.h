#ifndef VERTEX_H
#define VERTEX_H

#include"Terrain.h"

#include <d3d11.h>
#include <d3dcompiler.h>

class Vertex {

private:

	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;

	Terrain terrain;

	int nrOfVertices;

public:

	Vertex();
	~Vertex();

	void createShader(ID3D11Device* device);
	void createTriangleData(ID3D11Device* device);

	//Get
	ID3D11VertexShader* getShader(void);
	int getNrOfVertex(void) const;
	int getNrOfFaces(void) const;
	//ID3D11Buffer* getBuffer(void);
	ID3D11Buffer* getVertexBuffer(void);
	ID3D11Buffer* getIndexBuffer(void);
	ID3D11InputLayout* getInputLayout(void);

};

#endif // !VERTEX_Hs
