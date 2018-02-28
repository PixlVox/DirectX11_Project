#ifndef PIXEL_H
#define PIXEL_H

#include <d3d11.h>
#include <d3dcompiler.h>

class Pixel {

private:

	ID3D11PixelShader* shader;
	ID3D11PixelShader* colorShader;
	ID3D11PixelShader* blendShader;

public:

	Pixel();
	~Pixel();

	void createShader(ID3D11Device* device);
	
	//Get
	ID3D11PixelShader* getShader(void);
	ID3D11PixelShader* getColorShader(void);
	ID3D11PixelShader* getBlendShader(void);

};

#endif // !PIXEL_H
