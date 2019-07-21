#pragma once
class Bullet :
	public GameObject
{
public:
	Bullet();
	~Bullet();
private:
	CModel* model;
	XMFLOAT3 velocity;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(XMFLOAT3 position);
};

