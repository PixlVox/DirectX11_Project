#ifndef VERTEX_H
#define VERTEX_H

#include"Terrain.h"

#include <d3d11.h>
#include <d3dcompiler.h>

class Vertex {

private:

	ID3D11VertexShader* shader;
	ID3D11VertexShader* shaderColor;
	ID3D11InputLayout* inputLayout;
	ID3D11InputLayout* inputLayoutColor;

public:

	Vertex();
	~Vertex();

	//Create shaders
	void createShader(ID3D11Device* device);

	//Get
	ID3D11VertexShader* getShader(void);
	ID3D11VertexShader* getShaderColor(void);
	ID3D11InputLayout* getInputLayout(void);
	ID3D11InputLayout* getInputLayoutColor(void);


};

#endif // !VERTEX_Hs
