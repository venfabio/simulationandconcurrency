#include "ContactManifold.h"
#include "Ball.h"

ContactManifold::ContactManifold()
{
	_numOfPoints = 0;
}


ContactManifold::~ContactManifold()
{
}

void ContactManifold::Add(ManifoldPoint point)
{
	float dist = 0.0f;
	for (int collision = 0; collision < _numOfPoints; collision++)
	{
		glm::vec3 collisionNormal = glm::normalize(_points[collision].contactID2->GetNewPos() - _points[collision].contactID1->GetNewPos());
		glm::vec3 collisionPosition = _points[collision].contactID1->GetNewPos() + collisionNormal * _points[collision].contactID1->GetRadius();

		float dist = glm::distance(collisionPosition, point.contactPoint);
		if (dist < 0.01f)
		{
			return;
		}
	}

	_points[_numOfPoints] = point;
	_numOfPoints++;
}

void ContactManifold::Assess()
{
	bool loop = true;
	while (loop)
	{
		loop = false;
		for (int collision = 0; collision < _numOfPoints; collision++)
		{
			Ball* ball1 = _points[collision].contactID1;
			Ball* ball2 = _points[collision].contactID2;

			if (glm::distance(ball1->GetPos(), ball2->GetPos()) > ball1->GetRadius() + ball2->GetRadius())
			{
				for (int n = collision; n < _numOfPoints - 1; n++)
				{
					_points[n] = _points[n + 1];
				}
				_numOfPoints--;
				loop = true;
			}
		}
	}
}

int ContactManifold::GetNumPoints() const
{
	return _numOfPoints;
}

ManifoldPoint& ContactManifold::GetPoint(int index)
{
	return _points[index];
}
