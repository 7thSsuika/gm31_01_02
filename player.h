#pragma once
#include "modelAnimation.h"


class Player : public GameObject
{

private:
	CModel* model;
	CModelAnimation* animModel;
	int modelFrame = 0;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

