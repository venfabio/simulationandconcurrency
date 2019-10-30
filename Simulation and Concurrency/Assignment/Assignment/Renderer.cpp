#include "Renderer.h"
#include "Mesh.h"

//// Code Based on Directxtutorial.com

Renderer::Renderer(HWND hWnd)
	: _hWnd(hWnd)
{
	// camera first setup
	cam_x = 0.0f;
	cam_y = 2.0f;
	cam_z = -2.5f;
	cam_look_x = 0.0f;
	cam_look_y = 0.0f;
	cam_look_z = 0.0f;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
}


Renderer::~Renderer()
{
}

void Renderer::ClearScreen()
{
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Renderer::SwapBuffer()
{
	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}
void Renderer::CleanD3D(void)
{
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

	// close and release all existing COM objects
	zbuffer->Release();
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pCBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();

	TwTerminate();
}

// this function initializes and prepares Direct3D for use
void Renderer::InitD3D()
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;                    // set the back buffer width
	scd.BufferDesc.Height = SCREEN_HEIGHT;                  // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = _hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.SampleDesc.Quality = 0;                             // multisample quality level
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);
	// create the depth buffer texture
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = SCREEN_WIDTH;
	texd.Height = SCREEN_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *pDepthBuffer;
	dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

	// create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
	pDepthBuffer->Release();

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

	devcon->RSSetViewports(1, &viewport);

	BuildATB();

	InitPipeline();
}

void Renderer::InitPipeline()
{
	// compile the shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// create the shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input element object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);

	// create the constant buffers
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 176;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev->CreateBuffer(&bd, NULL, &pCBuffer);

	bd.ByteWidth = 16;							// Changing byte width to fit in the pixel constant buffer
	dev->CreateBuffer(&bd, NULL, &pPXCBuffer);	// Creating the pixel constant buffer

	devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	devcon->PSSetConstantBuffers(0, 1, &pPXCBuffer);

	cBuffer.LightVector = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	cBuffer.LightColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	cBuffer.AmbientColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

}

void Renderer::SetViewMatrix()
{
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(cam_x, cam_y, cam_z),   // the camera position
		&D3DXVECTOR3(cam_look_x, cam_look_y, cam_look_z),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction
}
void Renderer::SetProjectionMatrix()
{
	// create a projection matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		(FLOAT)D3DXToRadian(45),                    // field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		0.01f,                                       // near view-plane
		100.0f);                                    // far view-plane
}

// this is the function used to render a single frame
void Renderer::RenderFrame()
{
	SetProjectionMatrix();

	SetViewMatrix();

	QueryPerformanceCounter(&end);
	_deltaTime = (end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;

	fps = 1.0f / _deltaTime;

	start = end;

	/*cBuffer.LightVector = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
	cBuffer.LightColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	cBuffer.AmbientColor = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);*/

	static float Time = 0.0f; Time += 0.0009f;

	D3DXMatrixRotationY(&matRotate, Time);
	cBuffer.Final = matView * matProjection;
	cBuffer.Rotation = matRotate;

	devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);

	OnKeyboard();

	TwDraw();
}

// initializes the states
void Renderer::InitStates()
{
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.FrontCounterClockwise = FALSE;
	rd.DepthClipEnable = TRUE;
	rd.ScissorEnable = FALSE;
	rd.AntialiasedLineEnable = FALSE;
	rd.MultisampleEnable = FALSE;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.SlopeScaledDepthBias = 0.0f;

	dev->CreateRasterizerState(&rd, &pRSDefault);

	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 16;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;

	dev->CreateSamplerState(&sd, &pSS[0]);    // create the default sampler

	sd.Filter = D3D11_FILTER_ANISOTROPIC;

	dev->CreateSamplerState(&sd, &pSS[1]);    // create the anisotropic sampler

	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MinLOD = 5.0f;						  // at mip level X will appear blurred

	dev->CreateSamplerState(&sd, &pSS[2]);    // create the blur sampler

	D3D11_BLEND_DESC bd;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd.IndependentBlendEnable = FALSE;
	bd.AlphaToCoverageEnable = FALSE;

	dev->CreateBlendState(&bd, &pBS);
}

void Renderer::DrawGO(const GameObject *obj)
{
	// Calculating transformations
	D3DXMatrixRotationX(&matRotateX, DirectX::XMConvertToDegrees(obj->GetAngleX()));
	D3DXMatrixRotationY(&matRotateY, DirectX::XMConvertToDegrees(obj->GetAngleY()));
	D3DXMatrixRotationZ(&matRotateZ, DirectX::XMConvertToDegrees(obj->GetAngleZ()));
	D3DXMatrixTranslation(&matTranslate, obj->GetPosX(), obj->GetPosY(), obj->GetPosZ());
	D3DXMatrixScaling(&matScale, obj->GetScale(), obj->GetScale(), obj->GetScale());

	devcon->PSSetSamplers(0, 1, &pSS[1]);	// applying sampler

	if (obj->GetTransparency() == true)
	{		
		devcon->OMSetBlendState(pBS, 0, 0xffffffff);
		PcBuffer.blending = 0.2f;
	}
	else
	{
		devcon->RSSetState(pRSDefault);
		PcBuffer.blending = 1.0f;
	}
	cBuffer.Final = matRotateX * matRotateY * matRotateZ * matTranslate * matScale * matView * matProjection;

	devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);
	devcon->UpdateSubresource(pPXCBuffer, 0, 0, &PcBuffer, 0, 0);
	Draw(obj->GetMesh(), obj->GetTex());
}

void Renderer::Draw(const Mesh *mesh, Texture *tex)
{
	mesh->GetVBO()->Draw(this, tex);
}

void Renderer::Draw(const Mesh *mesh)
{
	mesh->GetVBO()->Draw(this);
}

void Renderer::OnKeyboard()
{
	if (GetKeyState(0x57) & 0x8000)	// W
	{
		amt = 2.0075f * _deltaTime;
		MoveCameraY(amt);
	}
	if (GetKeyState(0x53) & 0x8000) // S
	{
		amt = -2.0075f * _deltaTime;
		MoveCameraY(amt);
	}
	if (GetKeyState(0x41) & 0x8000)	// A
	{
		amt = -2.0075f * _deltaTime;
		MoveCameraX(amt);
	}
	if (GetKeyState(0x44) & 0x8000)	// D
	{
		amt = 2.0075f * _deltaTime;
		MoveCameraX(amt);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		amt = -2.0075f * _deltaTime;
		MoveCameraZ(amt);
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		amt = 2.0075f * _deltaTime;
		MoveCameraZ(amt);
	}
}


void Renderer::SetCameraOne()
{
	cam_x = -7.0f;
	cam_y = 30.0f;
	cam_z = -35.0f;
	cam_look_x = 0.0f;
	cam_look_y = 0.0f;
	cam_look_z = 0.0f;
}

void Renderer::SetCameraTwo()
{
	cam_x = 0.0f;
	cam_y = 5.0f;
	cam_z = -8.0f;
	cam_look_x = 0.0f;
	cam_look_y = 0.0f;
	cam_look_z = 0.0f;
}

void Renderer::RotateCameraX(float amt)
{
	cam_look_x = cam_look_x + amt;
}

void Renderer::RotateCameraY(float amt)
{
	cam_look_y = cam_look_y + amt;
}

void Renderer::MoveCameraX(float amt)
{
	cam_x = cam_x + amt;
	cam_look_x = cam_look_x + amt;
}

void Renderer::MoveCameraY(float amt)
{
	cam_y = cam_y + amt;
	cam_look_y = cam_look_y + amt;
}

void Renderer::MoveCameraZ(float amt)
{
	cam_z = cam_z + amt;
	cam_y = cam_y + (amt * -1);
}

void Renderer::BuildATB()
{
	if (!TwInit(TW_DIRECT3D11, dev)) // create AntTweakBar
		MessageBoxA(_hWnd, TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
	TwWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);		// size of the ATB window
	myBar = TwNewBar("AntTweakBar toolbox");
	TwAddVarRW(myBar, "FPS ", TW_TYPE_FLOAT, &fps, "Frames per Second (Renderer)");
	//TwAddVarRW(myBar, "FPS Phys", TW_TYPE_FLOAT, &fps, "Frames per Second (Physics)");
	TwAddVarRW(myBar, "Balls ", TW_TYPE_INT32, &balls, "Active Balls");
	TwAddVarRW(myBar, "Elasticity ", TW_TYPE_INT32, &el, "Elasticity of each ball");
	TwAddVarRW(myBar, "Time Modifier ", TW_TYPE_FLOAT, &tm, "Speed up or Slow down the scene");
	TwAddVarRW(myBar, "Graphics Freq. ", TW_TYPE_FLOAT, &fg, "Number of Draw loops per second");
	TwAddVarRW(myBar, "Physics Freq. ", TW_TYPE_FLOAT, &fp, "Number of Physics loops per second");
	TwAddVarRW(myBar, "Networking Freq. ", TW_TYPE_FLOAT, &fn, "Number of Network loops per second");
	TwAddVarRW(myBar, "Cam. X ", TW_TYPE_FLOAT, &cam_x, "Camera position in world space");
	TwAddVarRW(myBar, "Cam. Y ", TW_TYPE_FLOAT, &cam_y, "Camera position in world space");
	TwAddVarRW(myBar, "Cam. Z ", TW_TYPE_FLOAT, &cam_z, "Camera position in world space");
	TwAddVarRW(myBar, "Cam. Look X ", TW_TYPE_FLOAT, &cam_look_x, "Camera look-at position");
	TwAddVarRW(myBar, "Cam. Look Y ", TW_TYPE_FLOAT, &cam_look_y, "Camera look-at position");
	TwAddVarRW(myBar, "Cam. Look Z ", TW_TYPE_FLOAT, &cam_look_z, "Camera look-at position");
	
}

void Renderer::SetStatistics(int b, float e_tm, float e_fg, float e_fp, float e_fn, float e, float f)
{
	balls = b;
	tm = e_tm;
	fg = e_fg;
	fp = e_fp;
	fn = e_fn;
	el = e;
	phys = f;
}