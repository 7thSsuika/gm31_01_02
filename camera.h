#pragma once


class CCamera : public GameObject
{
private:

	class ViewFrustrum
	{
	public:
		ViewFrustrum();
		void ExtractPlanes(const XMMATRIX& comboMatrix, bool normalize);
	private:
		XMFLOAT4 planes[6];
	};

	// XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;

	bool viewChanged;
	RECT m_Viewport;
	ViewFrustrum viewFrustrum;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetVisibility(XMFLOAT3 Position);

};