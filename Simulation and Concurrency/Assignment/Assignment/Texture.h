#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Initialize(ID3D11Device* dev, LPCWSTR name);
	void Shutdown();

	ID3D11ShaderResourceView** GetTexture()		{ return &_texture; }

private:
	ID3D11ShaderResourceView* _texture;
};

