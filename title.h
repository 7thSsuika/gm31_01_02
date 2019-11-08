#pragma once
#include "scene.h"

class CScene;
class Title :
	public CScene
{
private:
	CAudioClip* bgm;
public:
	void Init();
	void Uninit();
	void Update();
};

