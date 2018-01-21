#ifndef VERTEX_H
#define VERTEX_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Vertex {

private:

	ID3D11Buffer* buffer;
	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;

	int nrOfVertices;

public:

	Vertex();
	~Vertex();

	void createShader(ID3D11Device* device);
	void createTriangleData(ID3D11Device* device);

	//Get
	ID3D11VertexShader* getShader(void);
	int getNrOfVertex(void) const;
	ID3D11Buffer* getBuffer(void);
	ID3D11InputLayout* getInputLayout(void);

};

#endif // !VERTEX_Hs
