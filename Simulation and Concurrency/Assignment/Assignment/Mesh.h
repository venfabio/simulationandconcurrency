#pragma once
#include <vector>
#include <string>
#include "VBO.h"
#include "Vertex.h"

using namespace std;

class Renderer;

class Mesh
{
	// Data
private:

	std::vector<VERTEX> _vertices;
	bool _locked;
	VBO*	_vbo;


public:
	Mesh();
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh & operator=(const Mesh&) = delete;

public:
	VBO* CreateVBO(Renderer* r);
	bool AddVertex(VERTEX v);
	bool LoadFromFile(std::string filename);
	VBO* GetVBO()		const		{ return _vbo; }		// Exception to rule 9.1.3 from codingstandard.com
	int NumVertices()		const	{ return (int)_vertices.size(); }
};

