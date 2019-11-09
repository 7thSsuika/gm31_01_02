#pragma once

class Ball : public GameObject
{
private :
	CModel* model;
	XMVECTOR m_Quaternion;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

