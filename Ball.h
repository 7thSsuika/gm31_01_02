#pragma once
#include "Collision.h"

class Ball : public GameObject
{
private :
	CModel* model;
	XMVECTOR m_Quaternion;
	AaBox m_Aabox;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AdjustPosition(float x, float y, float z);
};

