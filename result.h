#pragma once
#include "scene.h"

class CScene;
class Result :
	public CScene
{
private:
	CAudioClip* bgm;
public:
	void Init();
	void Uninit();
	void Update();
};

