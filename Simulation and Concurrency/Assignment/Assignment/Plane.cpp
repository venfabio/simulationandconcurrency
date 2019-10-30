#include "Plane.h"

const float Plane::MAX_SPEED = 3.5f;
const float Plane::TURN_SPEED = 0.05f;
const double Plane::WAIT_TIME = 10;

Plane::Plane()
{
	_position.X = 0;
	_position.Y = 1.0f;
	_position.Z = 0;
	_planeState = Grounded;
	_alive = true;
	_fuel = 1;
	elapsedTime = 0;
	whereAmI = 0;
	
}

/*Plane::Plane(int v)
{
	switch (v)
	{
	case 1:	_position.X = 0;
			_position.Y = 1.0f;
			_position.Z = 0;
			_angleX = 0;
			_angleY = 0;
			break;

	}
	
	_planeState = Grounded;
	whereAmI = 0;
	
}
*/
Plane::~Plane()
{
}

void Plane::Reset()
{
	_position.X = 0;
	_position.Y = 1.0f;
	_position.Z = 0;
	_angleX = 0;
	_planeState = Grounded;
	_alive = true;
	_fuel = 1;
	elapsedTime = 0;
	whereAmI = 0;
}


void Plane::TakeOff(double _deltaTime)
{
		_position.Y = _position.Y + MAX_SPEED * _deltaTime;
		if (_position.Y >= 2.2f)
		{
			_planeState = Flying;
		}
}

void Plane::PathOne(double _deltaTime)
{
	if (_planeState == Grounded)
		TakeOff(_deltaTime);
	else 
	if (_planeState == Flying)
	{
		if (whereAmI == 0)
		{
			_position.Z = _position.Z + MAX_SPEED * _deltaTime;
			//////////////////////////////////////////////////////
			if (_fuel <= 0 && _position.Z > -0.8f && _position.Z < 0.8f && _position.Y >= 1.5f)
			{
				_planeState = Oof;
				Landing(_deltaTime);
			}
			/////////////////////////////////////////////////////
			if (_angleX <= 0.015f && _position.Z >= 1.0f)
				_angleX = _angleX + (TURN_SPEED * _deltaTime);
			if (_position.Z >= 2.0f)
			{
				_position.Y = _position.Y - MAX_SPEED * _deltaTime;
				if (_position.Y <= 1.0f)
					_angleX = _angleX + (TURN_SPEED * _deltaTime);
				if (_position.Y <= -1.8f)
					whereAmI = 1;
			}
		}
		if (whereAmI == 1)
		{	
			if (_angleX <= 0.055f)
				_angleX = _angleX + (TURN_SPEED * _deltaTime);
			_position.Z = _position.Z - MAX_SPEED * _deltaTime;
			if (_position.Z <= -2.0f)
			{
				_position.Y = _position.Y + MAX_SPEED * _deltaTime;
				if (_position.Z <= -0.7f)
					_angleX = _angleX + (TURN_SPEED * _deltaTime);
				if (_position.Y >= 0.5f)
					if (_angleX <= 0.1f)
						_angleX = _angleX + (TURN_SPEED * _deltaTime);
				if (_position.Y >= 2.2f)
				{
					whereAmI = 0;
					_angleX = 0;
					_fuel--;
				}
			}
		}	
	}
	if (_planeState == Oof)
		Landing(_deltaTime);
}

void Plane::PathTwo(double _deltaTime)
{
	if (_planeState == Grounded)
		TakeOff(_deltaTime);
	else if (_planeState == Flying)
	{
		if (_angleY <= 0.025f)
			_angleY = _angleY + TURN_SPEED * _deltaTime;

		if (whereAmI == 0 && _angleY >= 0.025f)
		{
			_position.X = _position.X + MAX_SPEED * _deltaTime;
			if (_angleX <= 0.03f && _position.X > -0.2f)
				_angleX = _angleX + TURN_SPEED * _deltaTime;

		}
		if (_position.X >= 2.0f)
		{
			_position.Y = _position.Y - MAX_SPEED * _deltaTime;
			if (_position.Y <= 1.5f && whereAmI != 2)
				whereAmI = 1;
			if (_position.Y <= -2.0f)
				whereAmI = 2;
		}

		if (whereAmI == 2)
		{
			_position.X = _position.X - MAX_SPEED * _deltaTime;
			if (_angleX <= 0.055f)
				_angleX = _angleX + TURN_SPEED * _deltaTime;
		}
		if (_position.X <= -2.0f)
		{
			_angleX = _angleX + 0.02f * _deltaTime;
			_position.Y = _position.Y + MAX_SPEED * _deltaTime;
			if (_position.Y >= -1.5f && whereAmI != 0)
				whereAmI = 1;
			if (_position.Y >= 1.0f)
			{
				whereAmI = 0;
				_angleX = 0;
			}
		}
	}
}

void Plane::PathThree(double _deltaTime)
{
	if (_planeState == Grounded)
		TakeOffTwo(_deltaTime);
	else 
		if (_planeState == Flying)
		{
			if (whereAmI == 1)
			{
				if (_angleX >= 0.08f && _position.Y > 0)
					_angleX = -0.025f;
				_position.Y = _position.Y + MAX_SPEED * _deltaTime;
				//////////////////////////////////////////////////////
				if (_fuel <= 0 && _position.Y > -0.8f && _position.Y < 0.8f && _position.X >= 1.0f)
				{
					_planeState = Oof;
					LandingTwo(_deltaTime);
				}
				/////////////////////////////////////////////////////
				if (_angleX <= 0.005f)
				_angleX = _angleX + TURN_SPEED * _deltaTime;				
				if (_position.Y >= 2.0f)
					whereAmI = 2;
			}
			
			if (whereAmI == 2)
			{
				_position.X = _position.X - MAX_SPEED * _deltaTime;
				if (_position.X <= -1.5f && whereAmI == 2)
				{
					_angleX = _angleX + TURN_SPEED * _deltaTime;
					if (_angleX >= 0.03f && _position.X <= -2.0f)
						whereAmI = 3;
				}
			}
			
			if (whereAmI == 3)
			{
				_position.Y = _position.Y - MAX_SPEED * _deltaTime;		
				if (_position.Y <= -0.5f)
				{
					if (_angleX <= 0.052f)
					{
						_angleX = _angleX + TURN_SPEED * _deltaTime;
					}
					else
						whereAmI = 4;
				}
			}
			if (whereAmI == 4)
			{
				_position.X = _position.X + MAX_SPEED * _deltaTime;
				if (_position.X >= 1.8f)
				{
					_angleX = _angleX + TURN_SPEED * _deltaTime;
					if (_angleX >= 0.08f)
					{
						whereAmI = 1;			
						_fuel--;
					}
				}
			}
		}
	if (_planeState == Oof)
		LandingTwo(_deltaTime);
}

void Plane::PathFour(double _deltaTime)
{
	if (_planeState == Grounded)
		TakeOffThree(_deltaTime);
	else
		if (_planeState == Flying)
		{
			if (whereAmI == 1)
			{
				if (_angleX >= 0.02f && _position.Y > 1.3f)
					_angleX = -0.08f;
				_position.Y = _position.Y - MAX_SPEED * _deltaTime;
				//////////////////////////////////////////////////////
				if (_fuel <= 0 && _position.Y > -0.8f && _position.Y < 0.8f && _position.X <= -1.0f)
				{
					_planeState = Oof;
					LandingTwo(_deltaTime);
				}
				/////////////////////////////////////////////////////
				if (_angleX <= 0.005f && _position.Y < 0)
					_angleX = _angleX + TURN_SPEED * _deltaTime;
				if (_position.Y <= -2.0f)
					whereAmI = 2;
			}
			
			if (whereAmI == 2)
			{
				_position.X = _position.X + MAX_SPEED * _deltaTime;
				if (_position.X >= 1.0f && whereAmI == 2)
				{
					_angleX = _angleX + TURN_SPEED * _deltaTime;
					if (_position.X >= 2.9f)
						whereAmI = 3;
				}
			}
			
			if (whereAmI == 3)
			{
				_position.Y = _position.Y + MAX_SPEED * _deltaTime;
				if (_position.Y >= 0.8f)
				{
					if (_angleX <= 0.002f)
					{
						_angleX = _angleX + TURN_SPEED * _deltaTime;
					}
					else
						whereAmI = 4;
				}
			}
			if (whereAmI == 4)
			{
				_position.X = _position.X - MAX_SPEED * _deltaTime;
				if (_position.X <= -1.0f)
				{
					_angleX = _angleX + TURN_SPEED * _deltaTime;
					if (_angleX >= 0.025f)
					{
						whereAmI = 1;
						_fuel--;
					}
				}
			}
		}
	if (_planeState == Oof)
		LandingThree(_deltaTime);
}

void Plane::PathFive(double _deltaTime)
{
	if (_planeState == Grounded)
		TakeOffFour(_deltaTime);
	else
		if (_planeState == Flying)
		{
			if (whereAmI == 1)
			{
				if (_angleY <= 0.025f)
					_angleY = _angleY + TURN_SPEED * _deltaTime;

				if (whereAmI == 1 && _angleY >= 0.025f)
				{
					_position.X = _position.X - MAX_SPEED * _deltaTime;
					//////////////////////////////////////////////////////
					if (_fuel <= 0 && _position.X > -0.8f && _position.X < 0.8f && _position.Y <= -1.0f)
					{
						_planeState = Oof;
						LandingFour(_deltaTime);	
					}
					/////////////////////////////////////////////////////
					if (_position.X < -1.5f && _angleX < 0.08f)
					{
						_angleX = _angleX + TURN_SPEED * _deltaTime;
						_position.Y = _position.Y + MAX_SPEED * _deltaTime;
						if (_position.X < -3.2f)
							whereAmI = 2;
					}
				}
			}
			if (whereAmI == 2)
			{
				_position.Y = _position.Y + MAX_SPEED * _deltaTime;
				if (_position.Y > 2.4f && _angleX < 0.11f)
					_angleX = _angleX + TURN_SPEED * _deltaTime;
				if (_angleX > 0.11f)
					whereAmI = 3;
			}
			if (whereAmI == 3)
			{
				_position.X = _position.X + MAX_SPEED * _deltaTime;
				if (_position.X > 1.5f && _angleX < 0.135f)
				{
					_angleX = _angleX + TURN_SPEED * _deltaTime;
					_position.Y = _position.Y - MAX_SPEED * _deltaTime;
				}
				if (_angleX > 0.135f)
					whereAmI = 4;
			}
			if (whereAmI == 4)
			{
				_position.Y = _position.Y - MAX_SPEED * _deltaTime;
				if (_position.Y < -1.5f)							// +
					_angleX = _angleX + TURN_SPEED * _deltaTime;
				if (_angleX > 0.165f)
				{
					whereAmI = 1;
					_angleX = 0.055f;
					_fuel--;
				}
			}
		}
	if (_planeState == Oof)
		LandingFour(_deltaTime);

	if (_position.Y < -5.0f)	// bugfix
	{
		_alive = false;
		//_planeState == Dead;
	}
}
void Plane::TakeOffTwo(double _deltaTime)
{
	if (whereAmI == 0)
	{
		_position.X = 1.0f;
		_position.Y = -0.5f;
		_position.Z = 0;
		_angleX = -0.025f;
		_angleY = -0.025f;
		_angleZ = 0.0f;
	}

	else
		_position.X = _position.X + MAX_SPEED * _deltaTime;
	whereAmI = 1;
	if (_position.X >= 3.5f)
	{
		_planeState = Flying;
	}
}

void Plane::TakeOffThree(double _deltaTime)
{
	if (whereAmI == 0)
	{
		_position.X = -1.0f;
		_position.Y = 0;
		_position.Z = 0;
		_angleX = -0.08f;
		_angleY = -0.025f;
		_angleZ = 0;
	}
	else
		_position.X = _position.X - MAX_SPEED * _deltaTime;
	whereAmI = 1;
	if (_position.X <= -3.5f)
	{
		_planeState = Flying;
	}
}

void Plane::TakeOffFour(double _deltaTime)
{
	if (whereAmI == 0)
	{
		_position.X = 0;
		_position.Y = -1.1f;
		_position.Z = 0;
		_angleX = 0.055f;
		_angleY = 0;
		_angleZ = 0;
	}
	
	else
		_position.Y = _position.Y - MAX_SPEED * _deltaTime;
	whereAmI = 1;
	if (_position.Y <= -3.5f)
	{
		_planeState = Flying;
	}
}

void Plane::Landing(double _deltaTime)
{
	if (_position.Y >= 1.2f)
		_position.Y = _position.Y - MAX_SPEED * _deltaTime;
	elapsedTime = elapsedTime + _deltaTime;
	if (elapsedTime > WAIT_TIME)
	{
		_fuel = 2;
		_planeState = Grounded;
		elapsedTime = 0;
	}
}

void Plane::LandingTwo(double _deltaTime)
{
	if (_position.X >= 1.2f)
		_position.X = _position.X - MAX_SPEED * _deltaTime;
	elapsedTime = elapsedTime + _deltaTime;
	if (elapsedTime > WAIT_TIME)
	{
		_fuel = 4;
		_planeState = Grounded;
		elapsedTime = 0;
	}
}

void Plane::LandingThree(double _deltaTime)
{
	if (_position.X <= -1.2f)
		_position.X = _position.X + MAX_SPEED * _deltaTime;
	elapsedTime = elapsedTime + _deltaTime;
	if (elapsedTime > WAIT_TIME)
	{
		_fuel = 1;
		_planeState = Grounded;
		elapsedTime = 0;
	}
}

void Plane::LandingFour(double _deltaTime)
{
	if (_position.Y <= -1.1f)
		_position.Y = _position.Y + MAX_SPEED * _deltaTime;
	elapsedTime = elapsedTime + _deltaTime;
	if (elapsedTime > WAIT_TIME)
	{
		_fuel = 2;
		_planeState = Grounded;
		elapsedTime = 0;
	}
}