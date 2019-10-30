#pragma once
#pragma comment(lib, "ws2_32.lib")
#define WIN32_LEAN_AND_MEAN
#include <fstream>
#include <Windows.h>
#include <deque>
#include <thread>

#include "Window.h"
#include "Vertex.h"
#include "Renderer.h"

#include "Mesh.h"
#include "GameObject.h"
#include "Texture.h"
#include "ContactManifold.h"
#include "Ball.h"
#include "Airjet.h"
#include "Triangle.h"
#include "Message.h"
#include "Server.h"
#include "Client.h"

enum State{

	Running,
	Paused,
	Resetting,
	Quit
};

class Renderer;
class Window;

class Basket
{
public:
	Basket();
	~Basket();

private:

	enum TimeState{
		Normal,
		Faster,
		Slower
	};

	Window*					_window;
	Renderer*				_renderer;

	GameObject*				_court;
	GameObject*				_enclosure;
	GameObject*				_ringAlpha;
	GameObject*				_ringBravo;
	GameObject*				_netAlpha;
	GameObject*				_netBravo;
	std::vector<GameObject*>	_panels;

	ContactManifold*		_manifold;

	Ball*					_testBall;
	std::vector<Ball*>		_balls;

	Airjet*					_aj[8];

	Server					server;
	Client					client;

	// Meshes
	Mesh					_meshCourt;
	Mesh					_meshBall;
	Mesh					_meshEnclosure;
	Mesh					_meshRing;
	Mesh					_meshPanel;
	Mesh					_meshNet;

	// Texture
	Texture					_texCourt;
	Texture					_texBall;
	Texture					_texBall2;
	Texture					_texEnclosure;
	Texture					_texRing;
	Texture					_texNet;
	Texture					_texPanel_1;
	Texture					_texPanel_2;
	Texture					_texPanel_3;
	Texture					_texPanel_4;
	Texture					_texPanel_5;
	Texture					_texPanel_6;
	Texture					_texPanel_7;
	Texture					_texPanel_8;

	State					_currentState;
	int						_timeState;

	bool					_cameraDisabled;	// if True disable arrow movements
	bool					_keyStates[256];
	bool					is_Server;	
	bool					is_Client;
		
	double					_wait;
	double					timeModifier;
	double					_deltaTimeModified;

	float					_deltaTime;			// Time since last frame
	float					amt;
	float					power;				// CAN YOU FEEL IT ???
	float					fps;
	float					phys;

	float					cam_x;
	float					cam_y;
	float					cam_z;

	int						counter;
	int						MAX_BALLS;
	int						RESURRECTION_CD;
	int						configLines[9];
	int						hold;

	int						_freqGraphics;
	int						_freqPhysics;
	int						_freqNetworking;

	thread					T_Draw;
	thread					T_Physics;
	thread					T_Net;
	thread					T_Reader;

	// ATB Stuff
	int						CURRENT_BALLS;
	float					Elasticity;

	LARGE_INTEGER start, end, frequency;

protected:
	std::deque<Triangle*>	_triangles;
	std::deque<Triangle*>	_ringA;
	std::deque<Triangle*>	_ringB;

public:

	void Initialize(Window* w);
	void Run(void);     
	void Draw();
	void PhysicsManager();
	void NetServerManager();
	void NetClientManager();
	void OnKeyboard(int key, bool down);
	void Reset();
	void ReadConfig();
	void Destroy();
	void Reader();

	State GetPState()			const	{ return _currentState; }
};

