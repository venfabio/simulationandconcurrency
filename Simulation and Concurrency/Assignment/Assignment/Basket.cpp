#include <iostream>
#include <fstream>
#include <thread>

#include "Basket.h"

Basket::Basket()
{
	counter = 0;
	timeModifier = 0;
	_cameraDisabled = false;
	_wait = 0;
	hold = 0;

	CURRENT_BALLS = 1; // +1

	_freqGraphics = 100; // Hz
	_freqPhysics = 2000;
	_freqNetworking = 20;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
}

Basket::~Basket()
{
}

void Basket::Initialize(Window* w)
{
	_window = w;
	_renderer = w->GetRenderer();

	// Read stuff from config file
	ReadConfig();

	// Calling a separate a thread, speeds up the initialization
	T_Reader = std::thread(std::bind(&Basket::Reader, this));

	_manifold = new ContactManifold();

	_meshCourt.LoadFromFile("TableMesh.txt");
	_meshCourt.CreateVBO(_renderer);
	_texCourt.Initialize(_renderer->GetDevice(), L"Sand.png");
	_court = new GameObject();
	_court->SetMesh(&_meshCourt);
	_court->SetTex(&_texCourt);
	_court->SetAlive(true);
	
	_meshBall.LoadFromFile("WatermelonMesh.txt");
	_meshBall.CreateVBO(_renderer);
	_texBall.Initialize(_renderer->GetDevice(), L"WatermelonTextureAlt.png");
	_texBall2.Initialize(_renderer->GetDevice(), L"texture3.jpg");	// Alt. texture.

	for (int i = 0; i < 20; i++)
	{
		Ball* b = new Ball();
		b->SetMesh(&_meshBall);
		b->SetTex(&_texBall);
		b->SetPosY(0.9f);
		b->SetElasticity(Elasticity);
		_balls.push_back(b);
	}
	_balls[0]->SetAlive(true);
	//_balls[0]->SetVelocity(0, 0, 3);
	_balls[1]->SetTex(&_texBall2);

	_meshPanel.LoadFromFile("PanelMesh.txt");
	_meshPanel.CreateVBO(_renderer);
	_texPanel_1.Initialize(_renderer->GetDevice(), L"num1.png");
	_texPanel_2.Initialize(_renderer->GetDevice(), L"num2.png");
	_texPanel_3.Initialize(_renderer->GetDevice(), L"num3.png");
	_texPanel_4.Initialize(_renderer->GetDevice(), L"num4.png");
	_texPanel_5.Initialize(_renderer->GetDevice(), L"num5.png");
	_texPanel_6.Initialize(_renderer->GetDevice(), L"num6.png");
	_texPanel_7.Initialize(_renderer->GetDevice(), L"num7.png");
	_texPanel_8.Initialize(_renderer->GetDevice(), L"num8.png");
	for (int i = 0; i < 8; i++)
	{
		GameObject* g = new GameObject();
		g->SetMesh(&_meshPanel);
		_panels.push_back(g);
	}
	// Assign the proper texture
	_panels[0]->SetTex(&_texPanel_1);
	_panels[1]->SetTex(&_texPanel_2);
	_panels[2]->SetTex(&_texPanel_3);
	_panels[3]->SetTex(&_texPanel_4);
	_panels[4]->SetTex(&_texPanel_5);
	_panels[5]->SetTex(&_texPanel_6);
	_panels[6]->SetTex(&_texPanel_7);
	_panels[7]->SetTex(&_texPanel_8);
	// Put them in place
	_panels[0]->SetPos(-0.3f, 0.1f, -0.9f);
	_panels[1]->SetPos(0.3f, 0.1f, -0.9f);
	_panels[2]->SetPos(-0.3f, 0.1f, -0.3f);
	_panels[3]->SetPos(0.3f, 0.1f, -0.3f);
	_panels[4]->SetPos(-0.3f, 0.1f, 0.0f);
	_panels[5]->SetPos(0.3f, 0.1f, 0.0f);
	_panels[6]->SetPos(-0.3f, 0.1f, 0.7f);
	_panels[7]->SetPos(0.3f, 0.1f, 0.7f);

	_meshEnclosure.LoadFromFile("EnclosureMesh.txt");
	_meshEnclosure.CreateVBO(_renderer);
	_texEnclosure.Initialize(_renderer->GetDevice(), L"Glass.png");
	_enclosure = new GameObject();
	_enclosure->SetMesh(&_meshEnclosure);
	_enclosure->SetTex(&_texEnclosure);
	_enclosure->SetTransparency(true);
	_enclosure->SetAlive(true);

	_meshRing.LoadFromFile("RingMesh.txt");
	_meshRing.CreateVBO(_renderer);
	_texRing.Initialize(_renderer->GetDevice(), L"tex1.png");
	_ringAlpha = new GameObject();
	_ringAlpha->SetMesh(&_meshRing);
	_ringAlpha->SetTex(&_texRing);
	_ringAlpha->SetPosZ(-0.9f);
	_ringAlpha->SetAlive(true);
	_ringBravo = new GameObject();
	_ringBravo->SetMesh(&_meshRing);
	_ringBravo->SetTex(&_texRing);
	_ringBravo->SetPosZ(0.9f);
	_ringBravo->SetAlive(true);

	_meshNet.LoadFromFile("NetMesh.txt");
	_meshNet.CreateVBO(_renderer);
	_texNet.Initialize(_renderer->GetDevice(), L"White.png");
	_netAlpha = new GameObject();
	_netAlpha->SetMesh(&_meshNet);
	_netAlpha->SetPosZ(-0.9f);
	_netAlpha->SetTex(&_texNet);
	_netAlpha->SetAlive(true);
	_netBravo = new GameObject();
	_netBravo->SetMesh(&_meshNet);
	_netBravo->SetPosZ(0.9f);
	_netBravo->SetTex(&_texNet);
	_netBravo->SetAlive(true);

	_aj[0] = new Airjet(-0.3, -0.1, -0.6, 1);
	_aj[1] = new Airjet(0.3, -0.1, -0.6, 2);
	_aj[2] = new Airjet(-0.1, -0.1, -0.2, 3);
	_aj[3] = new Airjet(0.1, -0.1, -0.2, 4);
	_aj[4] = new Airjet(-0.1, -0.1, 0.2, 5);
	_aj[5] = new Airjet(0.1, -0.1, 0.2, 6);
	_aj[6] = new Airjet(-0.3, -0.1, 0.6, 7);
	_aj[7] = new Airjet(0.3, -0.1, 0.6, 8);

	T_Draw = std::thread(std::bind(&Basket::Draw, this));
	SetThreadAffinityMask(T_Draw.native_handle(), 1);

	T_Physics = std::thread(std::bind(&Basket::PhysicsManager, this));
	SetThreadAffinityMask(T_Physics.native_handle(), 1 << 4);

	if (is_Server == true && is_Client == false)
	{
		server.Initialize();
		T_Net = std::thread(std::bind(&Basket::NetServerManager, this));
		SetThreadAffinityMask(T_Net.native_handle(), 1 << 2);
		
	}

	if (is_Client == true && is_Server == false)
	{
		client.Initialize();
		T_Net = std::thread(std::bind(&Basket::NetClientManager, this));
		SetThreadAffinityMask(T_Net.native_handle(), 1 << 2);
	}

	_timeState = Normal;
	_currentState = Running;
}

void Basket::Reader()
{
	// Triangles creation
	ifstream in("TableMesh.txt");	// Using the mesh file to create a collision mask for each triangle of the table
	float dummy;
	int numVertices;
	in >> numVertices;
	for (int i = 0; i < numVertices; i++)
	{
		Triangle* t = new Triangle();
		in >> dummy;
		t->SetPosXv1(dummy);
		in >> dummy;
		t->SetPosYv1(dummy);
		in >> dummy;
		t->SetPosZv1(dummy);
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // first vertex: DONE. ignore the rest
		in >> dummy;
		t->SetPosXv2(dummy);
		in >> dummy;
		t->SetPosYv2(dummy);
		in >> dummy;
		t->SetPosZv2(dummy);
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // here comes the second vertex. ignore the rest
		in >> dummy;
		t->SetPosXv3(dummy);
		in >> dummy;
		t->SetPosYv3(dummy);
		in >> dummy;
		t->SetPosZv3(dummy);
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // the star of the show: the third vertex. ignore the rest
		t->CalcNormal();
		_triangles.push_back(t);	// Triangle completed.
	}
	in.close();

	// Triangles creation
	ifstream inA("RingAlphaCM.txt");	// Using the mesh file to create a collision mask for each triangle of the ring
	inA >> numVertices;
	for (int i = 0; i < numVertices; i++)
	{
		Triangle* t = new Triangle();
		inA >> dummy;
		t->SetPosXv1(dummy);
		inA >> dummy;
		t->SetPosYv1(dummy);
		inA >> dummy;
		t->SetPosZv1(dummy);
		inA.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		inA >> dummy;
		t->SetPosXv2(dummy);
		inA >> dummy;
		t->SetPosYv2(dummy);
		inA >> dummy;
		t->SetPosZv2(dummy);
		inA.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		inA >> dummy;
		t->SetPosXv3(dummy);
		inA >> dummy;
		t->SetPosYv3(dummy);
		inA >> dummy;
		t->SetPosZv3(dummy);
		inA.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		t->CalcNormal();
		_ringA.push_back(t);	// Triangle completed.
	}
	inA.close();

	// Triangles creation
	ifstream inB("RingBravoCM.txt");	// Using the mesh file to create a collision mask for each triangle of the ring
	inB >> numVertices;
	for (int i = 0; i < numVertices; i++)
	{
		Triangle* t = new Triangle();
		inB >> dummy;
		t->SetPosXv1(dummy);
		inB >> dummy;
		t->SetPosYv1(dummy);
		inB >> dummy;
		t->SetPosZv1(dummy);
		inB.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		inB >> dummy;
		t->SetPosXv2(dummy);
		inB >> dummy;
		t->SetPosYv2(dummy);
		inB >> dummy;
		t->SetPosZv2(dummy);
		inB.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		inB >> dummy;
		t->SetPosXv3(dummy);
		inB >> dummy;
		t->SetPosYv3(dummy);
		inB >> dummy;
		t->SetPosZv3(dummy);
		inB.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		t->CalcNormal();
		_ringB.push_back(t);	
	}
	inB.close();
}

void Basket::ReadConfig()
{
	const string ConfigFile = "config.txt";

	ifstream f(ConfigFile);
	string line;

	getline(f, line);
	getline(f, line);
	
	Elasticity = stof(line);
	
	getline(f, line);
	getline(f, line);

	is_Server = stoi(line);

	getline(f, line);
	getline(f, line);

	is_Client = stoi(line);
}

// this is the function used to render a single frame
void Basket::Run(void)
{
	// Get delta time
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
	
	if (_currentState == Running)
	{
		
		//_testBall->Physics(_deltaTime);
		//_testBall->Update();
		/*	Old test code.
		for (int i = 0; i < CURRENT_BALLS; i++)
		{
			_balls[i]->Physics(_triangles);
			_manifold->Assess();
			
			for (int j = i + 1; j < CURRENT_BALLS; j++)
			{	
				if (i != j &&
					_balls[i]->IsAlive() &&
					_balls[j]->IsAlive() )
				_balls[i]->CollisionWithBall(_balls[j], _manifold);

				//_balls[0]->CollisionWithBall(_balls[1], _manifold);
				
				for (int collision = 0; collision < _manifold->GetNumPoints(); collision++)
				{
					ManifoldPoint &point = _manifold->GetPoint(collision);
					if (!point.responded)
					{
						point.contactID1->CollisionResponseWithBall(point);

					}
				}
			}
			_balls[i]->Update();
		}*/
	}
	//phys = _balls[0]->GetFPS();
	// Send statistics to Renderer so ATB can use them.
	_renderer->SetStatistics(CURRENT_BALLS, timeModifier, _freqGraphics, _freqPhysics, _freqNetworking, Elasticity, phys);

}

void Basket::PhysicsManager()
{
	while (1)
	{
		for (int i = 0; i < CURRENT_BALLS; i++)
		{
			_balls[i]->Physics(_triangles, _ringA, _ringB);
			_manifold->Assess();

			for (int j = i + 1; j < CURRENT_BALLS; j++)
			{
				if (i != j &&
					_balls[i]->IsAlive() &&
					_balls[j]->IsAlive())
					_balls[i]->CollisionWithBall(_balls[j], _manifold);

				for (int collision = 0; collision < _manifold->GetNumPoints(); collision++)
				{
					ManifoldPoint &point = _manifold->GetPoint(collision);
					if (!point.responded)
					{
						point.contactID1->CollisionResponseWithBall(point);

					}
				}
			}
			_balls[i]->Update();
			_balls[i]->SetState((int)_currentState);
			_balls[i]->SetTimeState((int)_timeState, timeModifier);
		}
		/*
		for (int i = 0; i < _msgPool.size(); i++)
			if (_msgPool[i]->GetReceiver() == 1 && _msgPool[i]->HasResponded() == false)
				for (int j = 0; j < CURRENT_BALLS; j++)
					_balls[j]->ReadMessage(_msgPool[i]);*/
		Sleep(1000 / _freqPhysics);
	}
}

void Basket::Draw()
{
	while (1)
	{
		_renderer->ClearScreen();
		_renderer->RenderFrame();

		for (int i = 0; i < 8; i++)
			_renderer->DrawGO(_panels[i]);

		for (int i = 0; i < CURRENT_BALLS; i++)
			if (_balls[i]->IsAlive())
				_renderer->DrawGO(_balls[i]);

		_renderer->DrawGO(_ringAlpha);
		_renderer->DrawGO(_ringBravo);
		_renderer->DrawGO(_netAlpha);
		_renderer->DrawGO(_netBravo);

		_renderer->DrawGO(_court);
		_renderer->DrawGO(_enclosure);

		_renderer->SwapBuffer();
		Sleep(1000 / _freqGraphics);
	}
}

void Basket::NetServerManager()
{
	while (1)
	{
		server.ListenEcho();
		Sleep(1000 / _freqNetworking);
	}
}

void Basket::NetClientManager()
{
	while (1)
	{
		client.Send();
		Sleep(1000 / _freqNetworking);
	}
}

void Basket::OnKeyboard(int key, bool down)
{
	_keyStates[key] = down;

	// Misc
	if (_keyStates[27])
	{
		exit(0);
	}
	
	// Numstates
	if (_keyStates[49])
	{
	_aj[0]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[50])
	{
	_aj[1]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[51])
	{
	_aj[2]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[52])
	{
	_aj[3]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[53])
	{
	_aj[4]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[54])
	{
	_aj[5]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[55])
	{
	_aj[6]->Activate(_balls, CURRENT_BALLS);
	}
	if (_keyStates[56])
	{
	_aj[7]->Activate(_balls, CURRENT_BALLS);
	}

	if (_keyStates[66])	// B
	{
		if (CURRENT_BALLS < 19 && _currentState != Paused)
		{
			_balls[CURRENT_BALLS]->SetPos(0.0f, 0.9f, 0.0f);
			_balls[CURRENT_BALLS]->SetAlive(true);
			CURRENT_BALLS++;
		}
	}

	if (_keyStates[72])  // H
	{
		if (_timeState == Slower)
			timeModifier += 0.5f;
		else
		{
			_timeState = Slower;
			timeModifier = 1.5f;
		}
	}

	if (_keyStates[73])  // I
	{
		_freqGraphics++;
	}

	if (_keyStates[74])  // J
	{
		if (_freqNetworking > 1)
			_freqNetworking--;
	}

	if (_keyStates[75])  // K
	{
		if(_freqGraphics > 1)
			_freqGraphics--;
	}

	if (_keyStates[76])  // L
	{
		if (_freqPhysics > 1)
			_freqPhysics--;
	}

	if (_keyStates[78])	// N
	{
		
		for (int i = 0; i < CURRENT_BALLS; i++)
		{ 
			_balls[i]->Nudge();
		}
	}

	if (_keyStates[79])  // O
	{
		_freqPhysics++;
	}

	if (_keyStates[80])	// P
	{
		if (_currentState == Running)
			_currentState = Paused;
		else if (_currentState == Paused)
			_currentState = Running;
	}

	if (_keyStates[82])  // R
	{
		_currentState = Resetting;
		Reset();
		_currentState = Running;
	}

	if (_keyStates[85])  // U
	{
		_freqNetworking++;
	}

	if (_keyStates[89])  // Y
	{
		if (_timeState == Faster)
			timeModifier = timeModifier + 0.01f;
		else
		{
			_timeState = Faster;
			timeModifier = 0.01f;
		}
	}
}

void Basket::Reset()
{
	_timeState = Normal;
	timeModifier = 0;
	_balls[0]->SetPos(0, 0.8, 0);
	for (int i = 1; i < CURRENT_BALLS; i++)
		_balls[i]->SetAlive(false);
	CURRENT_BALLS = 1;
}

void Basket::Destroy()
{
	T_Draw.join();
	T_Physics.join();

	if (is_Server == true && is_Client == false)
	{
		T_Net.join();
		server.Clean();
	}

	if (is_Client == true && is_Server == false)
	{
		T_Net.join();
	}
}