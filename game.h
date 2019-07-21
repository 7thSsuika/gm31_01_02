#pragma once
class CScene;
class Game :
	public CScene
{
private:
	CAudioClip* bgm;
public:
	void Init();
	void Uninit();
	void Update();
};

