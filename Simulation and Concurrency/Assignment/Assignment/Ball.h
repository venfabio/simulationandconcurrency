#pragma once

#include <deque>

#include "GLM\glm.hpp"
#include "GameObject.h"
#include "ContactManifold.h"
#include "Triangle.h"
#include "Message.h"



class Ball:
	public GameObject
{
public:
	Ball();
	~Ball();

private:
	enum State{

		Running,
		Paused,
		Resetting,
		Quit
	};

	enum TimeState{
		Normal,
		Faster,
		Slower
	};

	struct Derivative
	{
		glm::vec3 _derVelocity;
		glm::vec3 _derAcceleration;
		glm::vec3 _derTorque;
	} a, b, c, d;

	// Physics stuff
	glm::vec3 _velocity;
	glm::vec3 _acceleration;
	glm::vec3 _newVelocity;
	glm::vec3 _newPosition;
	glm::vec3 _angularMomentum;
	glm::vec3 _angularVelocity;

	float _inertia;
	float _inverseInertia;

	float _mass;
	float _radius;
	float _elasticity;
	float _friction;
	float fps;

	int _objectID;

	static int _countID;

	State		_ballState;
	TimeState	_timeState;

	// Time stuff
	LARGE_INTEGER start, end, frequency;
	float					_deltaTime;
	float					timeModifier;
	float					_deltaTimeModified;

public:
	void Update();
	void Physics(std::deque<Triangle*> &_triangles, std::deque<Triangle*> &_rA, std::deque<Triangle*> &_rB);
	void CollisionWithBall(Ball* ball, ContactManifold *contactManifold);
	void CollisionResponseWithBall(ManifoldPoint &point);
	void CollisionWithPlane();
	void CollisionResponseWithPlane(glm::vec3 plane);
	void CollisionWithRing();
	void Spawn();
	void Nudge();

	void ReadMessage(Message *m);

	float GetRadius()	  const	{ return _radius; }
	float GetMass()		  const { return _mass;   }

	// http://gafferongames.com/game-physics/integration-basics/
	Derivative Evaluate(float dt, Derivative &d);

	//Christer Ericson - Real - Time Collision Detection P. 139
	glm::vec3 ClosestPointTriangle(Triangle *t);

	glm::vec3 GetVelocity()	  const { return _velocity; }
	void SetVelocity(float a, float b, float c)
	{
		_velocity.x = a; 
		_velocity.y = b;
		_velocity.z = c;
	}
	void SetVelocity(glm::vec3 vec)
	{
		_velocity = vec;
	}

	glm::vec3 GetNewVelocity()	  const { return _newVelocity; }
	void SetNewVelocity(float a, float b, float c)
	{
		_newVelocity.x = a;
		_newVelocity.y = b;
		_newVelocity.z = c;
	}
	void SetNewVelocity(glm::vec3 vec)
	{
		_newVelocity = vec;
	}

	glm::vec3 GetPos() const { return _newPosition; }

	glm::vec3 GetNewPos() const { return _newPosition; }
	void SetNewPosition(float a, float b, float c)
	{
		_newPosition.x = a;
		_newPosition.y = b;
		_newPosition.z = c;
	}
	void SetNewPosition(glm::vec3 vec)
	{
		_newPosition = vec;
	}

	void SetState(int n)		{ _ballState = static_cast<State>(n); }
	void SetTimeState(int x, float y)	
	{	
		_timeState = static_cast<TimeState>(x);
		timeModifier = y;
	}

	void SetElasticity(float n)	{ _elasticity = n; }
	void GetPhys()	{ fps; }
};

