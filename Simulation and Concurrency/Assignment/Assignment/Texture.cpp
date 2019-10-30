#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::Initialize(ID3D11Device* dev, LPCWSTR name)
{
	D3DX11CreateShaderResourceViewFromFile(dev, name, NULL, NULL, &_texture, NULL);
}

void Texture::Shutdown()
{
	if (_texture)
	{
		_texture->Release();
	}
}