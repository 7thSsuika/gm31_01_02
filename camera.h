#pragma once
#include "Collision.h"

class CCamera : public GameObject
{
private:

	class ViewFrustum
	{
	public:
		enum ContainState
		{
			FRUSTUM_OUT,
			FRUSTUM_IN,
			FRUSTUM_INTERSECT
		};
	public:
		ViewFrustum();
		void ExtractPlanes(const XMMATRIX& comboMatrix, bool normalize);
		int SphereIntersection(const XMFLOAT3& centerPos, const float radius) const;
		int AaBoxIntersection(const AaBox& refBox) const;
	private:
		enum frustumFace
		{
			FRUSTUM_LEFT,
			FRUSTUM_RIGHT,
			FRUSTUM_TOP,
			FRUSTUM_DOWN,
			FRUSTUM_NEAR,
			FRUSTUM_FAR,
		};

		typedef struct plane_tag
		{
			XMFLOAT3 normal;
			float distance;
			void Normalize();
			float DistanceToPoint(const XMFLOAT3& point) const;
		}PLANE;

		PLANE planes[6];
	};

	// XMFLOAT3					m_Position;
	XMFLOAT3					m_Rotation;
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;

	bool inputValid;
	RECT m_Viewport;
	ViewFrustum viewFrustrum;

public:
	void Init();
	void Uninit();
	void Update();

	bool CheckInput();

	void Draw();

	bool GetVisibility(XMFLOAT3 Position);
	bool GetVisibilityAabox(AaBox& aabox) const;

};