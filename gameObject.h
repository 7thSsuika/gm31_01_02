#pragma once
#include "main.h"

class GameObject
{
protected:
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

	bool m_Destroy = false;

public:
	GameObject() {}
	virtual ~GameObject() {}
	void SetDestroy();
	bool Destroy();
	XMFLOAT3 GetPosition() { return m_Position; }

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}
};