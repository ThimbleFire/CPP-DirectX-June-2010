#ifndef _SCREENMANAGER_H_
#define _SCREENMANAGER_H_

#include <stack>

#include "MainMenu.h"

#include "directX.h"

#include "Camera.h"

#include "GameTime.h"

#include "Input.h"

#include "SoundEngine.h"

class VGameScreen;

class ScreenManager
{
public:

	bool Initialize
	(
		_In_ unsigned int			SCREEN_WIDTH,
		_In_ unsigned int			SCREEN_HEIGHT,
		_In_ HWND					WINDOW_HANDLE
	);

	bool Update
	(
		_In_ GameTime&				GAMETIME
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

	);

	template<typename S>
	bool AddScreen
	(
		S& s
	);

	bool RemoveScreen
	(
		_In_ int					INDEX
	);

	int GetCount
	(
									void
	);

private:

	D3DXMATRIX baseViewMatrix;

	DirectX* directX_;

	SoundEngine* soundEngine_;

private:

	unsigned int screenWidth_;

	unsigned int screenHeight_;
};

#endif