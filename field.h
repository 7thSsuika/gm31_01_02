#pragma once


class CField : public GameObject
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;
	CTexture*		m_Texture = NULL;


	static const int FIELD_X = 32;
	static const int FIELD_Z = 32;
	float fieldSize = 1.0f;

	VERTEX_3D m_Vertex[FIELD_X * FIELD_Z];


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetHeight(XMFLOAT3 position);

};