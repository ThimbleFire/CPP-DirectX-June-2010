// @(#)root/net:$Id$
// Author: Anthony Boothroyd 28/2/14

#include "window.h"

bool Window::Initialize(LPCSTR windowTitle)
{
	unsigned int screenWidth  = 0;
	unsigned int screenHeight = 0;

	InitializeWindow(screenWidth, screenHeight, windowTitle);

	screenManager_ = nullptr;
	screenManager_ = new ScreenManager;
	screenManager_->Initialize(screenWidth, screenHeight, hwnd_);
	
	gameTime_.Start();

	input_.Initialize();

	return true;
}

void Window::Shutdown(LPCSTR windowTitle)
{
	if(screenManager_) {
		screenManager_->Shutdown();
		delete screenManager_;
		screenManager_ = nullptr;
	}

	ShutdownWindow(windowTitle);
}

bool Window::Render()
{
	bool result = true;
	
	result = screenManager_->Render();
	
	if(!result)
		return false;

	return true;
}

bool Window::Update()
{
	bool result = true;

	gameTime_.GetFrameTime();

	result = screenManager_->Update(gameTime_);
	
	if(!result)
		return false;
	return true;
}

bool Window::HandleInput()
{
	bool result = true;

	input_.Update();

	result = screenManager_->HandleInput();
	
	if(!result)
		return false;
	return true;
}

LRESULT CALLBACK Window::MessageHandler(HWND hwnd, unsigned int umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Window::InitializeWindow(unsigned int &screenWidth, unsigned int &screenHeight, LPCSTR windowTitle)
{
	//the location the window appears on screen

	int posx, posy;

	ApplicationHandle = this;

	hinstance_ = GetModuleHandle(NULL);
	
	WNDCLASSEX wc;
			   wc.cbSize =			sizeof(WNDCLASSEX);
			   wc.style =			CS_HREDRAW | CS_VREDRAW;
		   	   wc.lpfnWndProc =		WndProc;
		 	   wc.cbClsExtra =		NULL;
			   wc.cbWndExtra =		NULL;
			   wc.hInstance =		hinstance_;
			   wc.hIcon =			LoadIcon(NULL, IDI_APPLICATION);
			   wc.hIconSm =			wc.hIcon;
			   wc.hCursor =			LoadCursor(NULL, IDC_ARROW);
			   wc.hbrBackground =	(HBRUSH)(COLOR_WINDOW + 1);
			   wc.lpszMenuName =	NULL;
			   wc.lpszClassName =	windowTitle;

	RegisterClassEx(&wc);

	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	DEVMODE dmScreenSettings;

	if(FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posx = 0;
		posy = 0;		
	}
	else
	{
		screenWidth = 1080;
		screenHeight = 720;
		
		posx = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hwnd_ = CreateWindowEx(NULL, windowTitle, windowTitle, WS_OVERLAPPEDWINDOW, posx, posy, screenWidth, screenHeight, NULL, NULL, hinstance_, NULL);

	ShowWindow(hwnd_, SW_SHOW);

	SetForegroundWindow(hwnd_);

	SetFocus(hwnd_);

	ShowCursor(true);	
}

void Window::ShutdownWindow(LPCSTR windowTitle)
{
	ShowCursor(true);
	
	ChangeDisplaySettings(NULL, 0);

	DestroyWindow(hwnd_);
	hwnd_ = NULL;

	UnregisterClass(windowTitle, hinstance_);
	hinstance_ = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, unsigned int umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
				return 0;
		default:
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

void Window::MessageLoop()
{
	MSG msg;

	bool quitting = false;

	ZeroMemory(&msg, sizeof(MSG));

	while(!quitting)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD))
		{
			if (WM_QUIT == msg.message)
				quitting = true;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}
		else
		{			
			if(!Update())
				break;

			if(!HandleInput())
				break;

			if(!Render())
				break;
		}
	}
}