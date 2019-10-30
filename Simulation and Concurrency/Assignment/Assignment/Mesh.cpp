#include "Mesh.h"
#include <iostream>
#include <fstream>

Mesh::Mesh()
	: _locked(false),
	  _vbo(NULL)
{
	
}

Mesh::~Mesh()
{
	delete _vbo;
	_vbo = NULL;
}

bool Mesh::AddVertex(VERTEX v)
{
	if (!_locked)
	{
		_vertices.push_back(v);
		return true;
	}
	else
	{
		return false;
	}
}

VBO* Mesh::CreateVBO(Renderer* renderer)
{
	_locked = true;

	_vbo = new VBO();

	_vbo->Create(renderer, _vertices.data(), NumVertices());

	return _vbo;
}

bool Mesh::LoadFromFile(std::string filename)
{
	ifstream in(filename);
	if (in)
	{
		int numVertices;
		in >> numVertices;
		for (int i = 0; i < numVertices; i++)
		{
			VERTEX v;
			in >> v.X;
			in >> v.Y;
			in >> v.Z;
			in >> v.U;
			in >> v.V;
			in >> v.nX;
			in >> v.nY;
			in >> v.nZ;
			AddVertex(v);
		}
		in.close();
		return true;
	}
	else
	{
		// Could not open file
		return false;
	}

}