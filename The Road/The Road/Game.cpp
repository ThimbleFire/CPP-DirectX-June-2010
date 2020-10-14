#include "Game.h"

Game::Game()
{
	selectedOption_ = 0;

	timer = 0.0f;
	interval = 200.0f;

	directX_ =		 nullptr;
	camera_  =		 nullptr;

	btnContinue_ = nullptr;
	btnMemorial_ = nullptr;
	btnSettings_ = nullptr;
	btnCredits_  = nullptr;
	btnQuit_     = nullptr;

}

Game::~Game()
{

}

bool Game::Initialize(unsigned int pScreenWidth, unsigned int pScreenHeight, HWND pHwnd)
{	
	bool result;
	
	this->screenWidth_ = pScreenWidth;
	this->screenHeight_ = pScreenHeight;

	directX_ = new DirectX;
	result = directX_->Initialize(pScreenWidth, pScreenHeight, pHwnd);

	if(!result)
		return false;

	camera_ = new Camera;
	camera_->SetPosition(0.0f, 0.0f, 0.0f);
	camera_->Render();
	camera_->GetViewMatrix(baseViewMatrix);
	
	btnContinue_ = new Button("continue");
	btnContinue_->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);
	
	btnMemorial_ = new Button("memorial");
	btnMemorial_->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);
	
	btnSettings_ = new Button("settings");
	btnSettings_->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);
	
	btnCredits_ = new Button("credits");
	btnCredits_->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);
	
	btnQuit_ = new Button("quit");
	btnQuit_->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);

	return true;
}

bool Game::Shutdown()
{
	if(btnContinue_) {
		btnContinue_->Shutdown();
		delete btnContinue_;
		btnContinue_ = nullptr;
	}

	if(btnMemorial_) {
		btnMemorial_->Shutdown();
		delete btnMemorial_;
		btnMemorial_ = nullptr;
	}

	if(btnSettings_) {
		btnSettings_->Shutdown();
		delete btnSettings_;
		btnSettings_ = nullptr;
	}

	if(btnCredits_) {
		btnCredits_->Shutdown();
		delete btnCredits_;
		btnCredits_ = nullptr;
	}

	if(btnQuit_) {
		btnQuit_->Shutdown();
		delete btnQuit_;
		btnQuit_ = nullptr;
	}

	if(camera_) {
		delete camera_;
		camera_ = nullptr;
	}

	if(directX_)
	{
		directX_->Shutdown();
		delete directX_;
		directX_ = nullptr;
	}

	return true;
}

bool Game::Update(GameTime& gameTime_)
{
	bool result;

	timer += gameTime_.GetTime();

	camera_->Render();

	return true;
}

bool Game::HandleInput(Input& input_)
{
	if(timer > interval)
	{
		Analog analog_;
	
		input_.GetXbox().GetThumbStick(analog_, MSXInput::RIGHT);

		if(analog_.GetY() > 0.75f && analog_.GetMagnitude() > 0.75f && selectedOption_ > 0)
			selectedOption_--;
	
		if(analog_.GetY() <  -0.75f && analog_.GetMagnitude() > 0.75f && selectedOption_ < 4)
			selectedOption_++;

		timer = 0.0f;
	}

	return true;
}

bool Game::Render()
{
	bool result;

	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix, projectionMatrix;
	
	directX_->GetMatrix(DirectX::WORLD, worldMatrix);
	directX_->GetMatrix(DirectX::ORTHOGRAPHIC, orthoMatrix);
	directX_->GetMatrix(DirectX::PROJECTION, projectionMatrix);
	camera_->GetViewMatrix(viewMatrix);

	directX_->SetAlphaBlending(DirectX::ENABLED);
	directX_->SetZBuffer(DirectX::ENABLED);

	result = directX_->BeginScene(75, 95, 115, 255);
	
	if(!result)
		return false;

	float x = directX_->GetViewport().Width / 2;


			 //87 is half the width of button texture
		  x -= 87;

	//TODO: Render here...
	btnContinue_->Render(directX_->GetDeviceContext(), x, 300.0f, selectedOption_ == 0, worldMatrix, viewMatrix, orthoMatrix);
	btnMemorial_->Render(directX_->GetDeviceContext(), x, 350.0f, selectedOption_ == 1, worldMatrix, viewMatrix, orthoMatrix);
	btnSettings_->Render(directX_->GetDeviceContext(), x, 400.0f, selectedOption_ == 2, worldMatrix, viewMatrix, orthoMatrix);
	btnCredits_->Render(directX_->GetDeviceContext(),  x, 450.0f, selectedOption_ == 3, worldMatrix, viewMatrix, orthoMatrix);
	btnQuit_->Render(directX_->GetDeviceContext(),     x, 500.0f, selectedOption_ == 4, worldMatrix, viewMatrix, orthoMatrix);

	result = directX_->EndScene(); 

	if(!result)
		return false;

	directX_->SetAlphaBlending(DirectX::DISABLED);
	directX_->SetZBuffer(DirectX::DISABLED);

	return true;
}