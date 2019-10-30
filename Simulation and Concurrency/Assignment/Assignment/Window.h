#pragma once

// include Windows and Direct3D headers
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// Other useful libraries 
#include <stdlib.h>
#include <time.h>
#include <vector>

class Renderer;
class Basket;

class Window {

	//variabili private 
private:

	HWND					_hWnd;
	Renderer*				renderer;

public:
	
	static Window*							TheWindow;
	Basket*					_app;
	

	Window(Basket* app, HINSTANCE hInstance, int nCmdShow);
	//~Window();

	// Windows Message loop
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void Start();	

	Renderer* GetRenderer()			const	{ return renderer; }
};