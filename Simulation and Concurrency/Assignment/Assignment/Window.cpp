#define _USE_MATH_DEFINES // Allows access to PI from math.h
#include <math.h>
#include "Window.h"
#include "Renderer.h"
#include "Basket.h"

Window*	Window::TheWindow = 0;

Window::Window(Basket *app, HINSTANCE hInstance, int nCmdShow) 
	: renderer(0),
	_app(app)
{
	TheWindow = this;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	_hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Application Yeah",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		SCREEN_WIDTH, SCREEN_HEIGHT,    // set window to new resolution
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(_hWnd, nCmdShow);
}

// this is the main message handler for the program
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hWnd, message, wParam, lParam))
		return 0;

	switch (message)
	{
		case WM_DESTROY:
		{
			//PostQuitMessage(0); 
			exit(0);
			return 0;
		} 
		break;
		case WM_KEYDOWN:
		{
			TheWindow->_app->OnKeyboard(wParam, true);
		}
		break;
		case WM_KEYUP:
		{
			TheWindow->_app->OnKeyboard(wParam, false);
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::Start()

{
	renderer = new Renderer(_hWnd);

	// set up and initialize Direct3D
	renderer->InitD3D();
	renderer->InitStates();

	_app->Initialize(this);

	// enter the main loop:

	MSG msg;

	while (_app->GetPState() != Quit)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		_app->Run();
	}

	// clean up DirectX and COM
	
	renderer->CleanD3D();
	_app->Destroy();
}

