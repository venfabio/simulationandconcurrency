#include "Particle.h"

const double Particle::MAX_LIFESPAN = 5;

Particle::Particle()
{
	_alive = false;
}


Particle::~Particle()
{
}

void Particle::Create(float x, float y, float z)
{
	_position.X = x;
	_position.Y = y;
	_position.Z = z;
	_alive = true;
	_lifespan = MAX_LIFESPAN;
}

void Particle::Update(double _deltaTime)
{
	_angleZ = _angleZ + 0.5f * _deltaTime;
	if (_lifespan > 0)
	{
		_position.Y = _position.Y - (2.0f * _deltaTime);
		_lifespan -= _deltaTime;
	}
	else
	{
		_alive = false;
		//delete this;
	}
}