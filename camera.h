#pragma once


class CCamera : public GameObject
{
private:

	// XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMMATRIX	m_ViewMatrix;

	RECT m_Viewport;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};