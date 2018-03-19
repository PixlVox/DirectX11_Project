#ifndef CATNIP_H
#define CATNIP_H
#include"IncludeDX11.h"
#include"WICTextureLoader.h"
#include"Drawable.h"

class Catnip : public Drawable 
{
public:
	struct vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uvs;
	};


private:

	XMFLOAT4 sampleDirectionBuffer;

	int height;
	int width;

	int topology;
	int layout;

	XMMATRIX rotation;
	XMMATRIX scale;
	XMMATRIX translation;

	vertex vertices[4];
	int index[6];

	ID3D11Device * device;
	ID3D11DeviceContext * context;
	ID3D11ShaderResourceView * catSRV;
	ID3D11Buffer * vBuffer;
	ID3D11Buffer * iBuffer;
	ID3D11Buffer * catCBbuffer;

	void reportObjects();

	void loadWICTexture();

public:
	Catnip(int in_height, int in_width);
	~Catnip();

	void createBuffers();
	int getLayout();
	int getTopology();
	int getNrOfVertices();
	int getSizeOfVertex();
	void initiate();
	XMMATRIX getWorldMatrix();
	void SetTranslationMatix(XMMATRIX in_trans);
	void setScaleMatrix(XMMATRIX in_scale);
	void setRotationMatrix(XMMATRIX in_rotation);
	void setDevice(ID3D11Device * in_device);
	void setContext(ID3D11DeviceContext * in_context);
	void setIndexAndVertices(int * in_index, vertex * in_vertices);
	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getCatCB();
	ID3D11ShaderResourceView* getCatSRV() const;

};
#endif // !CATNIP_H
