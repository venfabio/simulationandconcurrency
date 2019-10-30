/////////////////////////////////////
/*Simulation and Concurrency	/
//Venuti Fabio						/
//Year 2015/2016///////////////////*/
/////////////////////////////////////

//// Code Based on Directxtutorial.com

// include the basic windows header files and the Direct3D header files
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>


#include "Basket.h"
#include "Window.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Basket app;

	Window application(&app, hInstance, nCmdShow);

	application.Start();
}



