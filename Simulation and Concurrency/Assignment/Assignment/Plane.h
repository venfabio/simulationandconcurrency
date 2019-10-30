#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

#include "GameObject.h"

enum PlaneState
{
	Grounded,					// Ready to fly again
	Flying,						
	Oof,						// Out of fuel
	Dead						// Rest in Peace.
};

class Plane
	: public GameObject
{
public:
	static const float MAX_SPEED;
	static const float TURN_SPEED;
	static const double WAIT_TIME;

private:
	D3DXMATRIX matTranslate;

	PlaneState _planeState;

	int whereAmI;
	int _fuel;
	int _id;

	double elapsedTime;

public:
	Plane();
	~Plane();

	void Reset();
	
	void PathOne(double _deltaTime);
	void PathTwo(double _deltaTime);
	void PathThree(double _deltaTime);
	void PathFour(double _deltaTime);
	void PathFive(double _deltaTime);

	void TakeOff(double _deltaTime);
	void TakeOffTwo(double _deltaTime);
	void TakeOffThree(double _deltaTime);
	void TakeOffFour(double _deltaTime);

	void Landing(double _deltaTime);
	void LandingTwo(double _deltaTime);
	void LandingThree(double _deltaTime);
	void LandingFour(double _deltaTime);

	PlaneState GetPlaneState()			const	{ return _planeState; }
	
};

