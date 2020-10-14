#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	LPCSTR windowTitle = "The Road (_X86) (_DX11)";

	Window* wnd = nullptr;
			wnd = new Window;

	 if(wnd->Initialize(windowTitle))
		wnd->MessageLoop();

	wnd->Shutdown(windowTitle);

	delete wnd;
		   wnd = nullptr;

   return 1;
}