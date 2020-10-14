#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "VGameScreen.h"

#include "Button.h"

class Settings : public VGameScreen
{
public:
	Settings() : VGameScreen()
	{
		timer = 0.0f;
		interval = 200.0f;
		optionSelected_ = 0;
	}
	~Settings(void)
	{

	}

	bool Initialize
	(
		DirectX*,
		SoundEngine*
	)	override;

	bool Shutdown
	(

	)	override;

	bool LoadContent
	(
		     ID3D11Device*	device,
		_In_ unsigned int   screenWidth,
		_In_ unsigned int   screenHeight
	)	override;

	bool Update
	(
		_In_ GameTime gameTime
	)	override;

	bool HandleInput
	(

	)	override;

	bool Render
	(
		ID3D11DeviceContext* deviceContext, 
		_In_ D3DXMATRIX worldMatrix, 
		_In_ D3DXMATRIX baseViewMatrix,
		_In_ D3DXMATRIX projectionMatrix,
		_In_ D3DXMATRIX orthoMatrix
	)	override;

private:
	
	void ConfirmSelection(unsigned int);

	unsigned int optionSelected_;

	float timer, interval;

	Button* btnContinue;
	
	Button* btnMemorial;
	
	Button* btnSettings;
	
	Button* btnCredits;
	
	Button* btnExit;

};

#endif