#include "Ball.h"

int Ball::_countID = 0;

Ball::Ball()
{
	_objectID = _countID;
	_countID++;
	_mass = 0.1;		 // kg
	_radius = 0.075;	 // meters
	_alive = false;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
}


Ball::~Ball()
{
}

void Ball::Update()
{
	_velocity = _newVelocity;
	_position = _newPosition;
	QueryPerformanceCounter(&end);
	_deltaTime = (end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;

	if (_timeState == Faster)
		_deltaTimeModified = _deltaTime + timeModifier;
	if (_timeState == Slower)
		_deltaTimeModified = _deltaTime / timeModifier;
	else
		_deltaTimeModified = _deltaTime;

	fps = 1.0f / _deltaTime;
	start = end;
}

void Ball::Physics(std::deque<Triangle*> &_triangles, std::deque<Triangle*> &_rA, std::deque<Triangle*> &_rB)
{
	if (_ballState == Running)
	{
		glm::vec3 force(0, -9.8f * _mass, 0);
		_acceleration = force / _mass;		// ??

		// Euler integration. BOOOOOOOOOH!!!
		//_newVelocity = _velocity + (_acceleration * _deltaTime);
		//_newPosition = _position + (_velocity * _deltaTime);

		// http://gafferongames.com/game-physics/integration-basics/
		a = Evaluate(_deltaTimeModified, Derivative());
		b = Evaluate(_deltaTimeModified * 0.5f, a);
		c = Evaluate(_deltaTimeModified * 0.5f, b);
		d = Evaluate(_deltaTimeModified, c);
		glm::vec3 temp_vel = 1.0f / 6.0f * (a._derVelocity + 2.0f * (b._derVelocity + c._derVelocity) + d._derVelocity);
		glm::vec3 temp_acc = 1.0f / 6.0f * (a._derAcceleration + 2.0f * (b._derAcceleration + c._derAcceleration) + d._derAcceleration);
		_newPosition = _position + (temp_vel * _deltaTimeModified);
		_newVelocity = _velocity + ((temp_acc + _acceleration) *_deltaTimeModified);	// ??

		// Test collisions
		// Floor (Hard fix for a game breaking bug).
		glm::vec3 temp(_newPosition - glm::vec3(0.0f, -0.15f, 0.0f));

		//Plane collision
		if (glm::dot(temp, glm::vec3(0.0f, 1.0f, 0.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(0, 1, 0));
		}

		// Triangle collision. Lovely.
		for (int i = 0; i < _triangles.size(); i++)
		{
			// Christer Ericson - Real-Time Collision Detection P. 167
			glm::vec3 p = ClosestPointTriangle(_triangles[i]);
			glm::vec3 v = p - _newPosition;
			if (glm::dot(v, v) < _radius * _radius)
				CollisionResponseWithPlane(_triangles[i]->GetNormal());
		}

		// Roof
		temp = (_newPosition - glm::vec3(0.0f, 1.0f, 0.0f));
		if (glm::dot(temp, glm::vec3(0.0f, -1.0f, 0.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(0, -1, 0));
		}

		// South wall
		temp = (_newPosition - glm::vec3(0.0f, 0.0f, -1.0f));
		if (glm::dot(temp, glm::vec3(0.0f, 0.0f, 1.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(0, 0, -1));
		}

		// North wall
		temp = (_newPosition - glm::vec3(0.0f, 0.0f, 1.0f));
		if (glm::dot(temp, glm::vec3(0.0f, 0.0f, -1.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(0, 0, 1));
		}

		// West wall
		temp = (_newPosition - glm::vec3(-0.5f, 0.0f, 0.0f));
		if (glm::dot(temp, glm::vec3(1.0f, 0.0f, 0.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(-1, 0, 0));
		}

		// East wall
		temp = (_newPosition - glm::vec3(0.5f, 0.0f, 0.0f));
		if (glm::dot(temp, glm::vec3(-1.0f, 0.0f, 0.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(1, 0, 0));
		}
		/*
		// Ring collision
		for (int i = 0; i < _rA.size(); i++)
		{
			// Christer Ericson - Real-Time Collision Detection P. 167
			glm::vec3 p = ClosestPointTriangle(_rA[i]);
			glm::vec3 v = p - _newPosition;
			if (glm::dot(v, v) < _radius * _radius)
				CollisionResponseWithPlane(_rA[i]->GetNormal());
		}

		// Ring collision
		for (int i = 0; i < _rB.size(); i++)
		{
			// Christer Ericson - Real-Time Collision Detection P. 167
			glm::vec3 p = ClosestPointTriangle(_rB[i]);
			glm::vec3 v = p - _newPosition;
			if (glm::dot(v, v) < _radius * _radius)
				CollisionResponseWithPlane(_rB[i]->GetNormal());
		}*/

		_angle = _angle + _angularMomentum * _deltaTimeModified;
	}
}

// When impacting with multiple balls...
void Ball::CollisionWithBall(Ball* ball, ContactManifold* contactManifold)	
{
	glm::vec3 x = this->GetPos();
	glm::vec3 y = ball->GetPos();

	float radii = this->GetRadius() + ball->GetRadius();

	float dist = glm::distance(x, y);

	float res = dist - radii;

	if (res < 0)
	{
		// collision happened
		glm::vec3 loc;	// line of collision
		loc = y - x;
		//loc = glm::normalize(loc);
		ManifoldPoint point;
		point.contactPoint = (this->GetRadius() * loc) + this->GetPos();
		point.contactID1 = this;
		point.contactID2 = ball;
		point.contactNormal = glm::normalize(loc); // l (oblique impact formula)
		point.responded = false;
		contactManifold->Add(point);
	}
}

void Ball::CollisionResponseWithBall(ManifoldPoint &point)
{
	// Oblique impact formula
	//float e = 0.5f;		// w/e
	float m1 = point.contactID1->GetMass();
	float m2 = point.contactID2->GetMass();
	glm::vec3 u1 = point.contactID1->GetVelocity();
	glm::vec3 u2 = point.contactID2->GetVelocity();

	glm::vec3 vl1 = ((m1 - (_elasticity*m2)) * glm::dot(u1, point.contactNormal) *point.contactNormal + (m2 + (_elasticity*m2))*glm::dot(u2, point.contactNormal) * point.contactNormal) / (m1 + m2);
	glm::vec3 vl2 = ((m1 - (_elasticity*m1)) * glm::dot(u1, point.contactNormal) *point.contactNormal + (m2 + (_elasticity*m1))*glm::dot(u2, point.contactNormal) * point.contactNormal) / (m1 + m2);

	glm::vec3 v1 = u1 - (glm::dot(u1, point.contactNormal) * point.contactNormal) + vl1;
	glm::vec3 v2 = u2 - (glm::dot(u2, point.contactNormal) * point.contactNormal) + vl2;

	point.contactID1->SetNewVelocity(v1);
 	point.contactID2->SetNewVelocity(v2);
	//point.contactID2->SetNewVelocity(v2.x, v2.y, v2.z);
    //_newPosition = _position + (temp_vel * _deltaTime);

	//point.contactID1->SetNewPosition(point.contactID1->GetPos() + (v1 * _deltaTime));
	//point.contactID2->SetNewPosition(point.contactID2->GetPos() + (v2 * _deltaTime));

	point.responded = true;
}

void Ball::CollisionResponseWithPlane(glm::vec3 plane)
{
	glm::vec3 Vr;
	Vr = _velocity - ((1 + _elasticity) * glm::dot(_velocity, glm::normalize(plane)) * glm::normalize(plane));
	_newVelocity = Vr;
	_newPosition = _position;
	_angularMomentum = glm::dot(_velocity, glm::normalize(plane)) * glm::normalize(plane);		// The most fake rotation ever seen. (Still, it looks good.)
}

void Ball::CollisionWithRing()
{
	float radius = 0.1;
	glm::vec3 circlePoint;
	glm::vec3 temp;
	circlePoint.y = 0.5;
	for (float angle = 0.0f; angle < 2 * 3.14; angle += 0.1f)
	{
		circlePoint.x = sin(angle)*radius;
		circlePoint.z = cos(angle)*radius;
		circlePoint.z += 0.9f;
		temp = (_newPosition - circlePoint);
		if (glm::dot(temp, glm::vec3(0.0f, 1.0f, 0.0f)) < _radius)
		{
			CollisionResponseWithPlane(glm::vec3(0, 1, 0));
		}
	}

}

// http://gafferongames.com/game-physics/integration-basics/
Ball::Derivative Ball::Evaluate(float dt, Derivative &d)
{
	glm::vec3 temp_pos = _position + d._derVelocity * dt;
	glm::vec3 temp_vel = _velocity + d._derAcceleration * dt;

	Derivative output;

	//const float k = 10;
	//const float b = 1;
	glm::vec3 force(0, -9.8f * _mass, 0);

	output._derVelocity = temp_vel;
	//output._derAcceleration = force;
	output._derAcceleration = temp_pos - _mass * temp_vel;

	return output;
}

void Ball::Nudge()
{
	int n = rand() % 2 + (-2);
	int m = rand() % 2 + (-2);
	glm::vec3 force(n, 2.0f, m);
	_velocity = _velocity + force;
}

// Christer Ericson - Real-Time Collision Detection P. 136 - 142 
glm::vec3 Ball::ClosestPointTriangle(Triangle *t)
{
	glm::vec3 a, b, c;
	a = t->GetVertex1();
	b = t->GetVertex2();
	c = t->GetVertex3();

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;

	glm::vec3 ap = _newPosition - a;

	float d1 = glm::dot(ab, ap);
	float d2 = glm::dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) 
		return a;

	glm::vec3 bp = _newPosition - b;
	float d3 = glm::dot(ab, bp);
	float d4 = glm::dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3)
		return b;

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return a + v * ab;
	}

	glm::vec3 cp = _newPosition - c;
	float d5 = glm::dot(ab, cp);
	float d6 = glm::dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
		return c;

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + w * ac;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b);
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w;
}

void Ball::ReadMessage(Message* m)
{
	int msgType = m->GetType();

	if (msgType == 1)
	{
		Nudge();
		m->Answered();
	}
}