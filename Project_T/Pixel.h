#ifndef PIXEL_H
#define PIXEL_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Pixel {

private:

	ID3D11PixelShader* shader;

public:

	Pixel();
	~Pixel();

	void createShader(ID3D11Device* device);
	
	//Get
	ID3D11PixelShader* getShader(void);

};

#endif // !PIXEL_H
