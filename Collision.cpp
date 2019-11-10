#include "Collision.h"



Collision::Collision()
{
}


Collision::~Collision()
{
}

AaBox::AaBox(const XMFLOAT3 & position, const XMFLOAT3 & scale)
	:
	centerPos(position), mScale(scale)
{
	for (int i = 0; i < 8; i++) // create box vertices
	{
		if (i < 4)
			corner[i].z = -0.5f * scale.z + position.z;
		else
			corner[i].z =  0.5f * scale.z + position.z;

		if (i % 2 == 0)
			corner[i].x = -0.5f * scale.x + position.x;
		else
			corner[i].x =  0.5f * scale.x + position.x;

		if (i % 4 < 2)
			corner[i].y =  0.5f * scale.y + position.y;
		else
			corner[i].y = -0.5f * scale.y + position.y;
	}
}

AaBox::AaBox()
{
	AaBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.1f, 0.1f, 0.1f));
}

void AaBox::GetVertices(XMFLOAT3 * corners) const
{
	for(int i = 0; i < 8; i++)
		corners[i] = corner[i];
}

void AaBox::ChangePosition(const XMFLOAT3 & newPos)
{
	XMFLOAT3 offset;
	offset.x = newPos.x - centerPos.x;
	offset.y = newPos.y - centerPos.y;
	offset.z = newPos.z - centerPos.z;
	for (int i = 0; i < 8; i++)
	{
		corner[i].x += offset.x;
		corner[i].y += offset.y;
		corner[i].z += offset.z;
	}
	centerPos.x = newPos.x;
	centerPos.y = newPos.y;
	centerPos.z = newPos.z;
}

void AaBox::ChangeScale(const XMFLOAT3 & newScale)
{
	for (int i = 0; i < 8; i++) // create box vertices
	{
		if (i < 4)
			corner[i].z = -0.5f * newScale.z + centerPos.z;
		else
			corner[i].z = 0.5f * newScale.z + centerPos.z;

		if (i % 2 == 0)
			corner[i].x = -0.5f * newScale.x + centerPos.x;
		else
			corner[i].x = 0.5f * newScale.x + centerPos.x;

		if (i % 4 < 2)
			corner[i].y = 0.5f * newScale.y + centerPos.y;
		else
			corner[i].y = -0.5f * newScale.y + centerPos.y;
	}
	mScale.x = newScale.x;
	mScale.y = newScale.y;
	mScale.z = newScale.z;
}
