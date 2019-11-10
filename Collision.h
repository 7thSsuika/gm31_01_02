#pragma once
#include "main.h"

class AaBox
{
public:
	AaBox(const XMFLOAT3& position, const XMFLOAT3& scale);
	AaBox();
	void GetVertices(XMFLOAT3* corners) const;
	void ChangePosition(const XMFLOAT3 & newPos);
	void ChangeScale(const XMFLOAT3& newScale);
private:
	XMFLOAT3 centerPos;
	XMFLOAT3 mScale;
	XMFLOAT3 corner[8];
};

class Collision
{
public:
	Collision();
	~Collision();
};

