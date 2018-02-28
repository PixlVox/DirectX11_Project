#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include<DirectXMath.h>

class Geometry {

private:

	ID3D11GeometryShader* shader;
	ID3D11GeometryShader* shaderColor;

public:

	Geometry();
	~Geometry();

	//Create shaders
	void createShader(ID3D11Device* device);

	//Get
	ID3D11GeometryShader* getShader(void);
	ID3D11GeometryShader* getShaderColor(void);

};

#endif // !GEOMETRY_H

