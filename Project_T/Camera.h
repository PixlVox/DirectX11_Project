#ifndef CAMERA_H
#define CAMERA_H

//Used for vector & matrix calculations
#include<DirectXMath.h>
#include <d3d11.h>

//Used for keyboard/mouse input
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

class Camera {

private:

	//Camera
	DirectX::XMVECTOR camForward;
	DirectX::XMVECTOR camRight;
	DirectX::XMVECTOR camY;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;
	DirectX::XMVECTOR position;

	DirectX::XMMATRIX camRotation;
	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX proj;

	bool pick;

	//Camera movement and rotation values
	float speed;
	float moveX;
	float moveZ;
	float yaw;
	float pitch;

	//Keyboard/Mouse controlls
	IDirectInputDevice8* keyboard;
	IDirectInputDevice8* mouse;

	//Previous mouse state and directInput object(used to get input from keyboard)
	DIMOUSESTATE previousMouseState;
	DIMOUSESTATE currentMouseState;
	LPDIRECTINPUT8 dI;

public:

	Camera(long wWidth, long wHeight);
	~Camera();

	void getInput();
	void update(float heightValue);
	bool initDI(HINSTANCE hInst, HWND wHandle);
	DirectX::XMVECTOR pickMouse(D3D11_VIEWPORT vp);

	//Get
	DirectX::XMMATRIX getView(void) const;
	DirectX::XMMATRIX getProj(void) const;
	DirectX::XMVECTOR getPosition(void);
	float getX(void) const;
	float getZ(void) const;
	bool getPick(void) const;

	//Set
	void setPick(bool pick);

};

#endif // !CAMERA_H
