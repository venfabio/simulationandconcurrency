#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <string>
#include "Vertex.h"
#include "Texture.h"

using namespace std;

class Renderer;

class VBO
{
public:
	VBO();
	~VBO();

protected:
	ID3D11Buffer* _vbo;
	int _numVertices;

	ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer
	ID3D11ShaderResourceView *pTexture;    // the pointer to the texture

public:
	void Draw(Renderer* renderer, Texture* tex);
	// overloads
	void Create(Renderer*, VERTEX vertices[], int numVertices);
	void Draw(Renderer* renderer);
};

