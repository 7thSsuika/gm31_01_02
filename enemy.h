#pragma once

class Enemy :
	public GameObject
{
private:
	CModel* model;
public:
	Enemy();
	~Enemy();
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

