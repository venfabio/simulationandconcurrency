#include "Airjet.h"


Airjet::Airjet()
{
}

Airjet::Airjet(float a, float b, float c, int x)
{
	_position.x = a;
	_position.y = b;
	_position.z = c;
	_attribute = x;
}

Airjet::~Airjet()
{
}

void Airjet::Activate(std::vector<Ball*> &b, int n)
{
	switch (_attribute)
	{
		case 1:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= -0.4f && b[i]->GetPosX() <= -0.2f)
					if (b[i]->GetPosZ() >= -0.7 && b[i]->GetPosZ() <= -0.5f)
						Blow(b[i]);
			}
			break;
		case 2:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= 0.1 && b[i]->GetPosX() <= 0.4f)
					if (b[i]->GetPosZ() >= -0.7 && b[i]->GetPosZ() <= -0.5)
						Blow(b[i]);
			}
			break;
		case 3:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= -0.25 && b[i]->GetPosX() <= 0.0)
					if (b[i]->GetPosZ() >= -0.3 && b[i]->GetPosZ() <= -0.1)
						Blow(b[i]);
			}
			break;
		case 4:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= 0.0 && b[i]->GetPosX() <= 0.2)
					if (b[i]->GetPosZ() >= -0.3 && b[i]->GetPosZ() <= -0.1)
						Blow(b[i]);
			}
			break;
		case 5:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= -0.25 && b[i]->GetPosX() <= 0.0)
					if (b[i]->GetPosZ() >= 0.1 && b[i]->GetPosZ() <= 0.3)
						Blow(b[i]);
			}
			break;
		case 6:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= 0.0 && b[i]->GetPosX() <= 0.2)
					if (b[i]->GetPosZ() >= 0.1 && b[i]->GetPosZ() <= 0.3)
						Blow(b[i]);
			}
			break;
		case 7:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= -0.4f && b[i]->GetPosX() <= -0.2f)
					if (b[i]->GetPosZ() >= 0.5 && b[i]->GetPosZ() <= 0.7f)
						Blow(b[i]);
			}
			break;
		case 8:
			for (int i = 0; i < n; i++)
			{
				if (b[i]->GetPosX() >= 0.2 && b[i]->GetPosX() <= 0.4f)
					if (b[i]->GetPosZ() >= 0.5 && b[i]->GetPosZ() <= 0.7)
						Blow(b[i]);
			}
			break;
	}
}

void Airjet::Blow(Ball* b)
{
	glm::vec3 force;
	switch (_attribute)
	{
		case 1: force = glm::vec3(3.0f, 10.0f, 10.0f); break;
		case 2: force = glm::vec3(-3.0f, 10.0f, 10.0f); break;
		case 3: force = glm::vec3(-1.0f, 10.0f, 5.0f); break;
		case 4: force = glm::vec3(1.0f, 10.0f, 5.0f); break;
		case 5: force = glm::vec3(-6.0f, 10.0f, 5.0f); break;
		case 6: force = glm::vec3(6.0f, 10.0f, 5.0f); break;
		case 7: force = glm::vec3(6.0f, 5.0f, 7.0f); break;
		case 8: force = glm::vec3(-6.0f, 5.0f, 7.0f); break;
	}
	
	b->SetVelocity(b->GetVelocity() + force);
}

bool Airjet::CollideWith(Ball* bubble, const Ball* other)
{
	float x, y, z;
	x = other->GetPosX();
	y = other->GetPosY();
	z = other->GetPosZ();

	x -= this->GetPosX();
	y -= this->GetPosY();
	z -= this->GetPosZ();

	double dist;

	dist = sqrt((x*x) + (y*y) + (z*z));

	double squaredRadi;

	squaredRadi = sqrt(0.05); // 

	if (dist <= squaredRadi)
		return true;
	else
		return false;
}