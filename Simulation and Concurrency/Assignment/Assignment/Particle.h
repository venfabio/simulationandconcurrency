#pragma once
#include "GameObject.h"
class Particle :
	public GameObject
{
public:
	Particle();
	~Particle();

private:
	static const double		MAX_LIFESPAN;
	double					_lifespan;
	double					_velocity;

public:
	void Create(float x, float y, float z);
	void Update(double _deltaTime);
	void Kill();
};

