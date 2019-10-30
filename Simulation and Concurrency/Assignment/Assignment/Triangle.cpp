#include <iostream>
#include <fstream>

#include "Triangle.h"


Triangle::Triangle()
{
}


Triangle::~Triangle()
{
}

// https://www.opengl.org/wiki/Calculating_a_Surface_Normal
void Triangle::CalcNormal()
{
	u = _vertexTwo - _vertexOne;
	v = _vertexThree - _vertexOne;

	_normal.x = (u.y * v.z) - (u.z * v.y);
	_normal.y = (u.z * v.x) - (u.x * v.z);
	_normal.z = (u.x * v.y) - (u.y * v.x);

	_normal = glm::normalize(_normal);
}