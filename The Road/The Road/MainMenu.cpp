#include "MainMenu.h"

bool MainMenu::Initialize(DirectX* directX, SoundEngine* soundEngine)
{
	btnContinue = nullptr;
	btnContinue = new Button("connect");

	btnMemorial = nullptr;
	btnMemorial = new Button("memorial");

	btnSettings = nullptr;
	btnSettings = new Button("settings");

	btnCredits = nullptr;
	btnCredits = new Button("credits");

	btnExit = nullptr;
	btnExit = new Button("exit");

	bufferWindow = nullptr;
	bufferWindow = new VertexBuffer2D;

	textureWindow = nullptr;
	textureWindow = new Texture2D;

	mainMenuBackground = nullptr;
	mainMenuBackground = new Texture2D;

	textureShaderWindow = nullptr;
	textureShaderWindow = new TextureShader;

	soundMenuDown_ = nullptr;
	soundMenuDown_ = new IDirectSoundBuffer8*;

	soundMenuConfirm_ = nullptr;
	soundMenuConfirm_ = new IDirectSoundBuffer8*;

	soundMenuUp_ = nullptr;
	soundMenuUp_ = new IDirectSoundBuffer8*;

	return VGameScreen::Initialize(directX, soundEngine);
}

bool MainMenu::Shutdown()
{
	if(soundEngine_)
	{
		if(soundMenuUp_) {
			soundEngine_->ShutdownWaveFile(soundMenuUp_);
			(*soundMenuUp_) = nullptr;
		}
	
		if(soundMenuDown_) {
			soundEngine_->ShutdownWaveFile(soundMenuDown_);
			(*soundMenuDown_) = nullptr;
		}
	
		if(soundMenuConfirm_) {
			soundEngine_->ShutdownWaveFile(soundMenuConfirm_);
			(*soundMenuConfirm_) = nullptr;
		}
	}

	if(btnContinue) {
		btnContinue->Shutdown();
		delete btnContinue;
		btnContinue = nullptr;
	}
	
	if(btnMemorial) {
		btnMemorial->Shutdown();
		delete btnMemorial;
		btnMemorial = nullptr;
	}
	
	if(btnSettings) {
		btnSettings->Shutdown();
		delete btnSettings;
		btnSettings = nullptr;
	}
	
	if(btnCredits) {
		btnCredits->Shutdown();
		delete btnCredits;
		btnCredits = nullptr;
	}
	
	if(btnExit) {
		btnExit->Shutdown();
		delete btnExit;
		btnExit = nullptr;
	}
	if(bufferWindow) {
		bufferWindow->Shutdown();
		delete bufferWindow;
		bufferWindow = nullptr;
	}

	if(textureWindow) {
		textureWindow->Shutdown();
		delete textureWindow;
		textureWindow = nullptr;
	}

	if(mainMenuBackground) {
		mainMenuBackground->Shutdown();
		delete mainMenuBackground;
		mainMenuBackground = nullptr;
	}

	if(textureShaderWindow) {
		textureShaderWindow->Shutdown();
		delete textureShaderWindow;
		textureShaderWindow = nullptr;
	}

	return VGameScreen::Shutdown();
}

bool MainMenu::LoadContent(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	btnContinue->LoadContent(device, screenWidth, screenHeight);
	btnMemorial->LoadContent(device, screenWidth, screenHeight);
	btnSettings->LoadContent(device, screenWidth, screenHeight);
	btnCredits->LoadContent(device, screenWidth, screenHeight);
	btnExit->LoadContent(device, screenWidth, screenHeight);

	soundEngine_->Load("../The Road/content/sounds/explosion1.wav", soundMenuConfirm_);
	soundEngine_->Load("../The Road/content/sounds/menuSelectionChange.wav", soundMenuDown_);
	soundEngine_->Load("../The Road/content/sounds/menuSelectionChange2.wav", soundMenuUp_);

	textureWindow->Initialize(device, "../The Road/content/textures/window", 194, 208);
	mainMenuBackground->Initialize(device, "../The Road/content/textures/mainMenuBackground", 1080, 720);

	bufferWindow->Initialize(device, screenWidth, screenHeight);
	textureShaderWindow->Initialize(device);

	return VGameScreen::LoadContent(device, screenWidth, screenHeight);
}

bool MainMenu::Update(GameTime gameTime)
{
	if(timer < interval)
		timer += gameTime.GetTime();

	return VGameScreen::Update(gameTime);
}

bool MainMenu::HandleInput()
{
	Input input;
	input.Initialize();
	input.Update();

	//get the neccesary inputs
	bool A = input.GetXbox().GetButtonState(MSXInput::BUTTON_A);
	bool down = input.GetXbox().GetButtonState(MSXInput::BUTTON_DOWN);
	bool up = input.GetXbox().GetButtonState(MSXInput::BUTTON_UP);

	//set our volume
	LONG volume = -1500;

	if(timer >= interval)
	{
		Analog analog;

		if (up)
		{
			soundEngine_->PlayWaveFile(soundMenuUp_, volume);
			this->optionSelected_--;

			if(optionSelected_ < 0)
				optionSelected_ = 4;
		}
		else if (down)
		{
			soundEngine_->PlayWaveFile(soundMenuDown_, volume);
			this->optionSelected_++;
			
			if(optionSelected_ > 4)
				optionSelected_ = 0;
		}

		if(interval < 100.0f)
			interval = 100.0f;

		timer = 0.0f;
	}

	//if we aren't inputting then reset our timer and inverval
	if(!down && !up)
	{
		timer = 200.0f;
		interval = 200.0f;
	}
	else
	{
		//if we are moving we decrease our inverval so we quickly navigate the menu
		interval-=3.0f;
	}
	
	//if A is up then we can confirm menu
	if(!A)
		canConfirm = true;

	//if A is down and we can confirm, confirm selection and play sound
	if(A && canConfirm)
	{
		soundEngine_->PlayWaveFile(soundMenuConfirm_, volume);
		canConfirm = false;
		ConfirmSelection(optionSelected_);
	}

	return VGameScreen::HandleInput();
}

bool MainMenu::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX orthoMatrix)
{	
	float x = (this->screenWidth_ / 2) - (174 / 2);

	btnContinue->Render(deviceContext, x, 300, (optionSelected_ == 0), worldMatrix, baseViewMatrix, orthoMatrix);
	btnMemorial->Render(deviceContext, x, 340, (optionSelected_ == 1), worldMatrix, baseViewMatrix, orthoMatrix);
	btnSettings->Render(deviceContext, x, 380, (optionSelected_ == 2), worldMatrix, baseViewMatrix, orthoMatrix);
	btnCredits->Render(deviceContext,  x, 420, (optionSelected_ == 3), worldMatrix, baseViewMatrix, orthoMatrix);
	btnExit->Render(deviceContext,     x, 460, (optionSelected_ == 4), worldMatrix, baseViewMatrix, orthoMatrix);

	bufferWindow->Render(deviceContext, x-10, 290, textureWindow, SourceRectangle(0, 0, 1, 1), worldMatrix, baseViewMatrix, orthoMatrix, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	textureShaderWindow->Render(deviceContext, bufferWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, textureWindow->GetTexture2D(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	
	bufferWindow->Render(deviceContext, 0, 0, mainMenuBackground, SourceRectangle(0, 0, 1, 1), worldMatrix, baseViewMatrix, orthoMatrix, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	textureShaderWindow->Render(deviceContext, bufferWindow->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, mainMenuBackground->GetTexture2D(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	return VGameScreen::Render(deviceContext, worldMatrix, baseViewMatrix, projectionMatrix, orthoMatrix);
}
      
void MainMenu::ConfirmSelection(unsigned int option)
{
	switch(option)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
}