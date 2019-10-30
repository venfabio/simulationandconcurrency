#pragma once

#include "GLM\glm.hpp"

class Triangle 
{
private:
	glm::vec3 _vertexOne;
	glm::vec3 _vertexTwo;
	glm::vec3 _vertexThree;
	glm::vec3 _normal;

	glm::vec3 u, v;

public:
	Triangle();
	~Triangle();

	void CalcNormal();

	void SetPosXv1(float x)				{ _vertexOne.x = x; }
	void SetPosXv2(float x)				{ _vertexTwo.x = x; }
	void SetPosXv3(float x)				{ _vertexThree.x = x; }

	void SetPosYv1(float y)				{ _vertexOne.y = y; }
	void SetPosYv2(float y)				{ _vertexTwo.y = y; }
	void SetPosYv3(float y)				{ _vertexThree.y = y; }

	void SetPosZv1(float z)				{ _vertexOne.z = z; }
	void SetPosZv2(float z)				{ _vertexTwo.z = z; }
	void SetPosZv3(float z)				{ _vertexThree.z = z; }

	glm::vec3 GetVertex1()		const	{ return _vertexOne; }
	glm::vec3 GetVertex2()		const	{ return _vertexTwo; }
	glm::vec3 GetVertex3()		const	{ return _vertexThree; }

	glm::vec3 GetNormal()		const	{ return _normal; }
};

