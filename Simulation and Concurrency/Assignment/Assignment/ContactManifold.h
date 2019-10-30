#pragma once

#include "GLM\glm.hpp"

// Forward declaration
class Ball;

// Structure of a point of the manifold
struct ManifoldPoint {
	glm::vec3 contactPoint;
	Ball* contactID1;
	Ball* contactID2;
	glm::vec3 contactNormal;
	bool responded;
};

class ContactManifold
{
public:
	ContactManifold();
	~ContactManifold();

	void Add(ManifoldPoint point);
	void Assess();
	int GetNumPoints() const;
	ManifoldPoint& GetPoint(int index);

private:
	ManifoldPoint _points[10000]; // 1000 ??? probably not enough
	int _numOfPoints;
};

