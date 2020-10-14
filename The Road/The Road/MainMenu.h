#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "VGameScreen.h"
#include "Button.h"
#include "VertexBuffer2D.h"

class MainMenu : public VGameScreen
{
public:
	MainMenu() : VGameScreen()
	{
		timer = 0.0f;
		interval = 200.0f;
		optionSelected_ = 0;
	}
	~MainMenu(void)
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
	
	IDirectSoundBuffer8** soundMenuDown_;
	
	IDirectSoundBuffer8** soundMenuUp_;
	
	IDirectSoundBuffer8** soundMenuConfirm_;

	void ConfirmSelection(unsigned int);

	int optionSelected_;

	bool canConfirm;

	float timer, interval;

	Button* btnContinue;
	
	Button* btnMemorial;
	
	Button* btnSettings;
	
	Button* btnCredits;
	
	Button* btnExit;

	Texture2D* textureWindow;

	Texture2D* mainMenuBackground;

	VertexBuffer2D* bufferWindow;

	TextureShader* textureShaderWindow;
};

#endif