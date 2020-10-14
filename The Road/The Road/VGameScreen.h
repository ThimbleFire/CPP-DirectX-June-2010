#ifndef _VGAMESCREEN_H_
#define _VGAMESCREEN_H_

#include <d3d11.h>
#include <D3DX10math.h>

#include "directX.h"
#include "SoundEngine.h"
#include "GameTime.h"
#include "Input.h"
#include "Camera.h"


class VGameScreen
{
public:

	VGameScreen
	(

	);

	~VGameScreen
	(

	);

	virtual bool Initialize
	(
		DirectX*,
		SoundEngine*
	);

	virtual bool Shutdown
	(

	);
	
	virtual bool LoadContent
	(
		     ID3D11Device*	device,
		_In_ unsigned int   screenWidth,
		_In_ unsigned int   screenHeight
	);
	
	virtual bool Update
	(
		_In_ GameTime gameTime
	);

	virtual bool HandleInput
	(

	);

	virtual bool Render
	(
		ID3D11DeviceContext* deviceContext, 
		_In_ D3DXMATRIX worldMatrix,
		_In_ D3DXMATRIX baseViewMatrix,
		_In_ D3DXMATRIX projectionMatrix,
		_In_ D3DXMATRIX orthoMatrix
	);

protected:

	D3DXMATRIX viewMatrix;

	DirectX*		directX_;

	SoundEngine*	soundEngine_;

	Camera*			screenCamera_;

	float screenWidth_, screenHeight;

};

#endif