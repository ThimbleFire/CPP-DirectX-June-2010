#ifndef _SCREENMANAGER_H_
#define _SCREENMANAGER_H_

#include "directX.h"

#include "Camera.h"

#include "GameTime.h"

#include "Input.h"

#include "Button.h"

class Game
{
public:

	Game();

	~Game();

	bool Initialize
	(
		_In_ unsigned int			SCREEN_WIDTH,
		_In_ unsigned int			SCREEN_HEIGHT,
		_In_ HWND					WINDOW_HANDLE
	);

	bool Update
	(
		_In_ GameTime&				gameTime_
	);

	bool Shutdown
	(
									void
	);

	bool Render
	(
									void
	);

	bool HandleInput
	(
		_In_ Input&					input_
	);

private:

	Button* btnContinue_;
	
	Button* btnMemorial_;

	Button* btnSettings_;

	Button* btnCredits_;

	Button* btnQuit_;

	Camera* camera_;

	DirectX* directX_;

private:

	float timer;

	float interval;

	unsigned int selectedOption_;

	D3DXMATRIX baseViewMatrix;

	unsigned int screenWidth_;

	unsigned int screenHeight_;
};

#endif