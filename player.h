#pragma once
class Player : public GameObject
{

private:
	CModel* model;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

