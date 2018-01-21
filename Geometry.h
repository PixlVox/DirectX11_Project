#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Geometry {

private:

	ID3D11GeometryShader* shader;

public:

	Geometry();
	~Geometry();

	void createShader(ID3D11Device* device);

	//Get
	ID3D11GeometryShader* getShader(void);

};

#endif // !GEOMETRY_H

