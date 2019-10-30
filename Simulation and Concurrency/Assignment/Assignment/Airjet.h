#pragma once
#include "GameObject.h"
#include "Ball.h"

class Airjet :
	public GameObject
{
private:

	int _attribute;		// identifies the "number" of the air jet

	// ____[P2]____		// ____[P1]____
	// _7_______8_		// _15_______16_
	// ___________		// ____________
	// ___5___6___		// ___13___14___	
	// ___________		// ___________
	// ___3___4___		// ___11___12___
	// ___________		// ___________
	// _1_______2_		// _9_______10_
	// ____[P1]____		// ____[P2]____

public:
	Airjet();
	Airjet(float a, float b, float c, int x);
	~Airjet();

	//void InitializeBubbles(Renderer* r);
	void Activate(std::vector<Ball*> &b, int n);
	void Blow(Ball* b);
	bool CollideWith(Ball* bubble, const Ball* other);
};

