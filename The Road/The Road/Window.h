#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <Windows.h>

#include "ScreenManager.h"
#include "GameTime.h"
#include "Input.h"

class Window
{

public:

	void Shutdown
	(
		_In_  LPCSTR				WINDOW_TITLE
	);

	void MessageLoop
	(
									void
	);

	LRESULT CALLBACK MessageHandler
	(
		_In_ HWND					WINDOW_HANDLE, 
		_In_ unsigned int			MESSAGE_HANDLE, 
		_In_ WPARAM					INPUT_INDEX_W,
		_In_ LPARAM					INPUT_INDEX_L
	);

	bool Initialize
	(
		_In_ LPCSTR				WINDOW_TITLE
	);

private:

	void InitializeWindow
	(
		_Out_ unsigned int&			WINDOW_WIDTH,
		_Out_ unsigned int&			WINDOW_HEIGHT,
		_In_  LPCSTR				WINDOW_TITLE
	);

	bool Render
	(
									void
	);

	bool HandleInput
	(
									void
	);

	bool Update
	(
									void
	);

	void ShutdownWindow
	(
		_In_  LPCSTR				WINDOW_TITLE
	);

private:

	ScreenManager*					screenManager_;

	GameTime						gameTime_;

	Input							input_;

	HINSTANCE						hinstance_;

	HWND							hwnd_;
};

	static Window* ApplicationHandle = nullptr;

	static LRESULT CALLBACK WndProc
	(
		_In_ HWND, 
		_In_ unsigned int, 
		_In_ WPARAM, 
		_In_ LPARAM
	);


#endif