#include"Terrain.h"

Terrain::Terrain() {

	this->nrOfFaces = 0;
	this->nrOfVertices = 0;

	this->hM.height = 0;
	this->hM.width = 0;
	this->hM.vertexData = nullptr;

	this->loadHeightMap("BitMapTest.bmp");

	this->vBuffer = nullptr;
	this->iBuffer = nullptr;

}

Terrain::~Terrain() {

	if (this->hM.vertexData != nullptr) {

		delete[] this->hM.vertexData;

	}

	if (this->vBuffer != nullptr) {

		this->vBuffer->Release();

	}

	if (this->iBuffer != nullptr) {

		this->iBuffer->Release();

	}

}

bool Terrain::loadHeightMap(char* url) {

	bool result = true;

	FILE* file = nullptr;		//Holds current position in file
	BITMAPFILEHEADER bmHeader;	//Stores information about file
	BITMAPINFOHEADER bmInfo;	//Stores information about image
	int imageSize = 0, index = 0;
	unsigned char heightValue = ' ';

	//Open file
	file = fopen(url, "rb");	//Filepath + mode to open in (rb = Read + binary data)

	if (file = nullptr) {

		result = false;

	}
	else {

		//Read bitmap header/info
		fread(&bmHeader, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&bmInfo, sizeof(BITMAPINFOHEADER), 1, file);

		//get width/height
		this->hM.width = bmInfo.biWidth;
		this->hM.height = bmInfo.biHeight;

		//Get size of image in bytes, stores RGB in 3 bytes
		imageSize = (this->hM.width * this->hM.height * 3);

		//init array that stores image data
		unsigned char* bmImage = new unsigned char[imageSize];

		//Set file pointer to start
		fseek(file, bmHeader.bfOffBits, SEEK_SET);

		//Store image data
		fread(bmImage, 1, imageSize, file);

		//Close file
		fclose(file);

		//Read data from greyscale image, only need value from one color channel
		int k = 0;
		float heightFactor = 10.0f;

		//Read imageData into heigthMap array
		for (int j = 0; j < hM.height; j++) {

			for (int i = 0; i < hM.width; i++) {

				heightValue = bmImage[k];		//Get heightValue(R)
				index = (hM.height * j) + i;	//Find correct index

												//Store coords
				this->hM.vertexData[index].x = (float)i;
				this->hM.vertexData[index].y = (float)heightValue / heightFactor;
				this->hM.vertexData[index].z = (float)j;

				//Skip ahead of the 2 other color channels(GB)
				k += 3;

			}

		}

		delete[] bmImage;
		bmImage = 0;

	}

	return result;

}

void Terrain::createBuffers() {

	//Vertex structure
	struct Vertex {

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;

	};

	//Set number of vertices and faces
	this->nrOfVertices = this->hM.height * this->hM.width;
	this->nrOfFaces = (this->hM.height - 1) * (this->hM.width - 1) * 2;

	//Create vector and store all vertex positions and a default normal
	std::vector<Vertex> vec(this->nrOfVertices);

	for (DWORD i = 0; i < this->hM.height; ++i) {

		for (DWORD j = 0; j < this->hM.width; ++j) {

			vec[(i * this->hM.height) + j].pos = this->hM.vertexData[(i* this->hM.height) + j];
			vec[(i * this->hM.height) + j].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

		}

	}



}

DirectX::XMFLOAT3 Terrain::getVertexPos(int index) const{

	return this->hM.vertexData[index];

}

ID3D11Buffer* Terrain::getVertexBuffer() {

	return this->vBuffer;

}

ID3D11Buffer* Terrain::getIndexBuffer() {

	return this->iBuffer;

}