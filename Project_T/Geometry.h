#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include<DirectXMath.h>

class Geometry {

private:

	ID3D11GeometryShader* shader;
	ID3D11Buffer* constBuffer;

	struct Matrix {

		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX world;

	};

	Matrix mat;


public:

	Geometry();
	~Geometry();

	void createShader(ID3D11Device* device);
	void createConstBuffer(ID3D11Device* device);
	void mapConstBuffer(ID3D11DeviceContext* dContext);

	//Handle matrix
	void updateMatrixValues(DirectX::XMMATRIX view, int wWidth, int wHeight);

	//Get
	ID3D11GeometryShader* getShader(void);
	ID3D11Buffer* getConstBuffer(void);

};

#endif // !GEOMETRY_H

