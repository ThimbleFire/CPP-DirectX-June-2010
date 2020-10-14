#include "VGameScreen.h"

VGameScreen::VGameScreen()
{
	screenCamera_ = nullptr;
}

VGameScreen::~VGameScreen()
{

}

bool VGameScreen::Initialize(DirectX* directX, SoundEngine* soundEngine)
{	
	screenCamera_ = new Camera;
	screenCamera_->Render();
	screenCamera_->GetViewMatrix(viewMatrix);

	directX_ = directX;

	soundEngine_ = soundEngine;

	return true;
}

bool VGameScreen::LoadContent(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight)
{
	this->screenWidth_ = screenWidth;
	this->screenHeight = screenHeight;

	return true;
}

bool VGameScreen::Shutdown()
{
	return true;
}

bool VGameScreen::Update(GameTime gameTime)
{
	screenCamera_->Render();

	return true;
}

bool VGameScreen::HandleInput()
{
	return true;
}

bool VGameScreen::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX baseViewMatrix, D3DXMATRIX projectionMatrix, D3DXMATRIX orthoMatrix)
{
	return true;
}