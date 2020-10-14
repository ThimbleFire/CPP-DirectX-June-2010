#include "screenManager.h"
	
std::stack<std::shared_ptr<VGameScreen>> gameScreen;

bool ScreenManager::Initialize(unsigned int pScreenWidth, unsigned int pScreenHeight, HWND pHwnd)
{	
	bool result;
	
	this->screenWidth_ = pScreenWidth;
	this->screenHeight_ = pScreenHeight;

	directX_ = nullptr;
	directX_ = new DirectX;
	result = directX_->Initialize(pScreenWidth, pScreenHeight, pHwnd);

	if(!result)
		return false;

	Camera* camera_;
	camera_ = nullptr;
	camera_ = new Camera;
	camera_->SetPosition(0.0f, 0.0f, 0.0f);
	camera_->Render();
	camera_->GetViewMatrix(baseViewMatrix);
	delete camera_;
	camera_ = nullptr;

	soundEngine_ = nullptr;
	soundEngine_ = new SoundEngine;
	result = soundEngine_->Initialize(pHwnd);

	if(!result)
		return false;

	AddScreen(MainMenu());

	return true;
}

bool ScreenManager::Shutdown()
{
	for(unsigned int i = 0; i < gameScreen.size(); i++)
		gameScreen.top()->Shutdown();
	

	if(soundEngine_) {
		soundEngine_->Shutdown();
		delete soundEngine_;
		soundEngine_ = nullptr;
	}

	if(directX_) {
		directX_->Shutdown();
		delete directX_;
		directX_ = nullptr;
	}

	return true;
}

bool ScreenManager::Update(GameTime& gameTime)
{
	bool result;

	for(unsigned int i = 0; i < gameScreen.size(); i++)
	{
		  result = gameScreen.top()->Update(gameTime);
	
		if(!result)
			return false;
	}

	return true;
}

bool ScreenManager::HandleInput()
{
	bool result;

	for(unsigned int i = 0; i < gameScreen.size(); i++)
	{
		result = gameScreen.top()->HandleInput();
	
		if(!result)
			return false;
	}

	return true;
}

bool ScreenManager::Render()
{
	bool result;

	D3DXMATRIX worldMatrix, orthoMatrix, projectionMatrix;
	
	directX_->GetMatrix(DirectX::WORLD, worldMatrix);
	directX_->GetMatrix(DirectX::ORTHOGRAPHIC, orthoMatrix);
	directX_->GetMatrix(DirectX::PROJECTION, projectionMatrix);

	directX_->SetAlphaBlending(DirectX::ENABLED);
	directX_->SetZBuffer(DirectX::ENABLED);

	result = directX_->BeginScene(75, 95, 115, 255);
	
	if(!result)
		return false;

	for(unsigned int i = 0; i < gameScreen.size(); i++)
	{
		result = gameScreen.top()->Render(directX_->GetDeviceContext(), worldMatrix, baseViewMatrix, projectionMatrix, orthoMatrix);
	
		if(!result)
			return false;
	}

	result = directX_->EndScene(); 

	if(!result)
		return false;

	directX_->SetAlphaBlending(DirectX::DISABLED);
	directX_->SetZBuffer(DirectX::DISABLED);

	return true;
}

template<typename S>
bool ScreenManager::AddScreen(S& s)
{
	bool result;

	gameScreen.emplace(std::make_shared<S>(std::forward<S>(s)));

	result = gameScreen.top()->Initialize(directX_, soundEngine_);

	if(!result)
		return false;

	result = gameScreen.top()->LoadContent(directX_->GetDevice(), screenWidth_, screenHeight_);

	if(!result)
		return false;

	return true;
}

bool ScreenManager::RemoveScreen(int index)
{
	unsigned int a = GetCount();

	gameScreen.top()->Shutdown();
	
	return true;
}

int ScreenManager::GetCount()
{
	return gameScreen.size();
}