#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>

#include "Mesh.h"
#include "Texture.h"
#include "GLM\glm.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();		

protected:
	Mesh*		_mesh;		// Vertex info
	Texture*	_tex;
	float		_scale;		// 1 = normal

	bool		isTransparent = false;
	bool		_alive;

	LPCWSTR	_texture_name;

	glm::vec3 _position;
	glm::vec3 _angle;

public:
	Mesh* GetMesh()			const	{ return _mesh; }
	void SetMesh(Mesh* m)			{ _mesh = m; }

	float GetScale()		const	{ return _scale; }
	void SetScale(float v)			{ _scale = v; }

	LPCWSTR GetTexture()	const	{ return _texture_name; }
	void SetTexture(LPCWSTR	t)		{ _texture_name = t; }

	Texture* GetTex()		const	{ return _tex; }
	void SetTex(Texture* t)			{ _tex = t; }

public:

	bool GetTransparency()		const	{ return isTransparent; }
	void SetTransparency(bool t)			{ isTransparent = t; }

	float GetAngleX()		const	{ return _angle.x; }
	void SetAngleX(float x)			{ _angle.x = x; }
	float GetAngleY()		const	{ return _angle.y; }
	void SetAngleY(float y)			{ _angle.y = y; }
	float GetAngleZ()		const	{ return _angle.z; }
	void SetAngleZ(float z)			{ _angle.z = z; }

	glm::vec3 GetPos()		const	{ return _position; }
	void SetPos(float a, float b, float c)	
	  {		_position.x = a;
			_position.y = b;
			_position.z = c;
	  }
	void SetPos(glm::vec3 pos)
	{
		_position = pos;
	}

	float GetPosX()		    const	{ return _position.x; }
	void SetPosX(float x)				{ _position.x = x; }
	float GetPosY()			const	{ return _position.y; }
	void SetPosY(float y)				{ _position.y = y; }
	float GetPosZ()			const	{ return _position.z; }
	void SetPosZ(float z)				{ _position.z = z; }

	bool IsAlive()						const	{ return _alive; }
	void SetAlive(bool v)						{ _alive = v; }
	
	bool CollideWith(const GameObject* other);
};

