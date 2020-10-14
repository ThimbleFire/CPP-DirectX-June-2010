#include "Settings.h"

bool Settings::Initialize(DirectX* directX, SoundEngine* soundEngine)
{
	btnContinue = nullptr;
	btnContinue = new Button("continue");

	btnMemorial = nullptr;
	btnMemorial = new Button("memorial");

	btnSettings = nullptr;
	btnSettings = new Button("settings");

	btnCredits = nullptr;
	btnCredits = new Button("credits");

	btnExit = nullptr;
	btnExit = new Button("exit");

	return VGameScreen::Initialize(directX, soundEngine);
}

bool Settings::Shutdown()
{
	return VGameScreen::Shutdown();
}

bool Settings::LoadContent(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	btnContinue->LoadContent(device, screenWidth, screenHeight);
	btnMemorial->LoadContent(device, screenWidth, screenHeight);
	btnSettings->LoadContent(device, screenWidth, screenHeight);
	btnCredits->LoadContent(device, screenWidth, screenHeight);
	btnExit->LoadContent(device, screenWidth, screenHeight);

	return VGameScreen::LoadContent(device, screenWidth, screenHeight);
}

bool Settings::Update(GameTime gameTime)
{
	timer += gameTime.GetTime();

	return VGameScreen::Update(gameTime);
}

bool Settings::HandleInput()
{
	if(timer >= interval)
	{
		Analog analog;

		Input input;

		input.Initialize();

		input.Update();

		bool down = input.GetXbox().GetButtonState(MSXInput::BUTTON_DOWN);
		bool up = input.GetXbox().GetButtonState(MSXInput::BUTTON_UP);
		bool A = input.GetXbox().GetButtonState(MSXInput::BUTTON_A);

		if(A)
		{
			ConfirmSelection(optionSelected_);
		}
		else if (up && optionSelected_ > 0)
		{
			this->optionSelected_--;
		}
		else if (down && optionSelected_ < 4)
		{
			this->optionSelected_++;
		}

		timer = 0.0f;
	}
	return VGameScreen::HandleInput();
}

bool Settings::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX orthoMatrix)
{	
	float x = (this->screenWidth_ / 2) - (174 / 2);

	btnContinue->Render(deviceContext, x, 300, (optionSelected_ == 0), worldMatrix, viewMatrix, orthoMatrix);
	btnMemorial->Render(deviceContext, x, 350, (optionSelected_ == 1), worldMatrix, viewMatrix, orthoMatrix);
	btnSettings->Render(deviceContext, x, 400, (optionSelected_ == 2), worldMatrix, viewMatrix, orthoMatrix);
	btnCredits->Render(deviceContext,  x, 450, (optionSelected_ == 3), worldMatrix, viewMatrix, orthoMatrix);
	btnExit->Render(deviceContext,     x, 500, (optionSelected_ == 4), worldMatrix, viewMatrix, orthoMatrix);
	  
	return VGameScreen::Render(deviceContext, worldMatrix, baseViewMatrix, projectionMatrix, orthoMatrix);
}

void Settings::ConfirmSelection(unsigned int option)
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