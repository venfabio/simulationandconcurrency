#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

#include "GameObject.h"
#include "Vertex.h"
#include "Plane.h"

#include "AntTweakBar.h"

// define the screen resolution
#define SCREEN_WIDTH  1440
#define SCREEN_HEIGHT 900

class Mesh; 

class Renderer
{
public:
	Renderer(HWND _hWnd);
	~Renderer();

protected:

	HWND _hWnd;

	IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
	ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
	ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer
	ID3D11Buffer *pPXCBuffer;			   // the pointer to the pixel constant buffer

	ID3D11InputLayout *pLayout;            // the pointer to the input layout
	ID3D11VertexShader *pVS;               // the pointer to the vertex shader
	ID3D11PixelShader *pPS;                // the pointer to the pixel shader

	// state objects
	ID3D11RasterizerState *pRSDefault;     // the default rasterizer state
	ID3D11SamplerState *pSS[3];            // different sampler states
	ID3D11BlendState *pBS;                 // a blend state

	TwBar*					myBar;

	// a struct to define the constant buffer
	struct CBUFFER
	{
		D3DXMATRIX Final;
		D3DXMATRIX Rotation;
		D3DXVECTOR4 LightVector;
		D3DXCOLOR LightColor;
		D3DXCOLOR AmbientColor;
	};
	CBUFFER cBuffer;

	// a struct to define the constant buffer for the pixel shader
	struct PCBUFFER
	{
		float blending;
		D3DXVECTOR3 imNotDoingAnything;		// needed to make it 16 bytes
	};
	PCBUFFER PcBuffer;

	D3DXMATRIX matRotate, matView, matProjection;
	D3DXMATRIX matFinal;
	D3DXMATRIX matTranslate;			   

	D3DXMATRIX matRotateX;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matRotateZ;
	D3DXMATRIX matScale;

	/* Spotlights
	struct Light
	{
		DirectX::XMFLOAT3 pos;
		float range;
		DirectX::XMFLOAT3 dir;
		float cone;
		DirectX::XMFLOAT3 att;
		float pad2;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
	}light;
	*/

// Camera stuff \o/
	// Position coordinates
	float cam_x;
	float cam_y;
	float cam_z;
	// Look at coordinates
	float cam_look_x;
	float cam_look_y;
	float cam_look_z;

	void SetProjectionMatrix();
	void SetViewMatrix();

// Extra stuff
	int balls;	// active balls
	float tm;	// time modifier
	float fg;	// graphics freq.
	float fp;	// physics freq.
	float fn;	// networking freq.
	float el;   // elasticity
	float phys;


public:
	void ClearScreen();
	void SwapBuffer();
	void CleanD3D(void);
	void InitD3D();
	void InitPipeline();

public:
	void RenderFrame();
	void InitStates(void);      // initializes the states
	void SetCameraOne();
	void SetCameraTwo();
	void SetCameraPlane(Plane* plane);
	void SwitchCameraPlane();
	void BuildATB();

	void DrawGO(const GameObject* obj);
	void Draw(const Mesh* mesh, Texture* tex);
	// Draw Overload
	void Draw(const Mesh* mesh);

	void SetStatistics(int b, float tm, float fg, float fp, float fn, float e, float f);

// Camera movements
	
	void RotateCameraX(float amt);
	void RotateCameraY(float amt);
	void MoveCameraX(float amt);
	void MoveCameraY(float amt);
	void MoveCameraZ(float amt);

	void OnKeyboard();

private:
	// Time stuff
	LARGE_INTEGER start, end, frequency;
	float					_deltaTime;
	float					amt;
	float					fps;

public:
	ID3D11Device* GetDevice()					const	{ return dev; }
	void SetDevice(ID3D11Device* v)						{ dev = v; }

	ID3D11DeviceContext* GetContext()			const	{ return devcon; }
	void SetContext(ID3D11DeviceContext* v)				{ devcon = v; }

	ID3D11Buffer* GetConstantBuffer()			const	{ return pCBuffer; }
	void SetConstantBuffer(ID3D11Buffer* v)				{ pCBuffer = v; }

	CBUFFER Get_cBuffer()						const	{ return cBuffer; }
	
};

