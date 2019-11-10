
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"





void CCamera::Init()
{

	m_Position = XMFLOAT3( 0.0f, 5.0f, -10.0f );
	m_Rotation = XMFLOAT3( 0.5f, 0.0f, 0.0f );
	

	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;


	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	// ビューマトリクス設定
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	viewFrustrum.ExtractPlanes(m_ViewMatrix * m_ProjectionMatrix, true);

}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	inputValid = CheckInput();

	if (inputValid)
	{
		// ビューマトリクス設定
		m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		XMVECTOR det;
		m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

		viewFrustrum.ExtractPlanes(m_ViewMatrix * m_ProjectionMatrix, true);
	}
}



bool CCamera::CheckInput()
{
	bool viewChanged = false;
	int input;
	viewChanged |= input = Mouse::Get().GetState().leftButton && (Mouse::Get().mouseTracker.leftButton == Mouse::Get().mouseTracker.HELD);
	if (input)
	{
		viewChanged = true;
		m_Rotation.y += (Mouse::Get().GetState().x - Mouse::Get().mouseTracker.GetLastState().x) * 0.01f;
		m_Rotation.x += (Mouse::Get().GetState().y - Mouse::Get().mouseTracker.GetLastState().y) * 0.01f;
	}
	viewChanged |= input = CInput::GetKeyPress(VK_NUMPAD8) - CInput::GetKeyPress(VK_NUMPAD2);
	if (input)
	{
		viewChanged = true;
		m_Rotation.x += input * 0.03f;
	}

	viewChanged |= input = CInput::GetKeyPress(VK_NUMPAD6) - CInput::GetKeyPress(VK_NUMPAD4);
	if (input)
	{
		m_Rotation.y += input * 0.03f;
	}

	viewChanged |= input = CInput::GetKeyPress(VK_UP) - CInput::GetKeyPress(VK_DOWN);
	if (input)
	{
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, m_InvViewMatrix);
		m_Position.x += mat._31 * input * 0.1f;
		m_Position.y += mat._32 * input * 0.1f;
		m_Position.z += mat._33 * input * 0.1f;

	}

	viewChanged |= input = CInput::GetKeyPress(VK_RIGHT) - CInput::GetKeyPress(VK_LEFT);
	if (input)
	{
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, m_InvViewMatrix);
		m_Position.x += mat._11 * input * 0.1f;
		m_Position.y += mat._12 * input * 0.1f;
		m_Position.z += mat._13 * input * 0.1f;
	}

	viewChanged |= input = CInput::GetKeyPress(VK_NUMPAD7) - CInput::GetKeyPress(VK_NUMPAD9);
	if (input)
	{
		m_Position.y += input * 0.13;
	}
	return viewChanged;
}

void CCamera::Draw()
{
	CRenderer::SetViewMatrix(&m_ViewMatrix);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

bool CCamera::GetVisibility(XMFLOAT3 Position)
{
	return viewFrustrum.SphereIntersection(Position, 0.01f);

	/*      old style(check point in perspective space)
	XMVECTOR worldPos, viewPos, projPos;
	XMFLOAT3 projPosF;

	worldPos = XMLoadFloat3(&Position);
	viewPos = XMVector3TransformCoord(worldPos, m_ViewMatrix);
	projPos = XMVector3TransformCoord(viewPos, m_ProjectionMatrix);
	XMStoreFloat3(&projPosF, projPos);

	if (-1.0f < projPosF.x && projPosF.x < 1.0f &&
		-1.0f < projPosF.y && projPosF.y < 1.0f &&
		0.0f < projPosF.z && projPosF.z < 1.0f)
	{
		return true;
	}

	return false;
	*/
}

CCamera::ViewFrustum::ViewFrustum()
{
	ZeroMemory(planes, sizeof(planes));
}

void CCamera::ViewFrustum::ExtractPlanes(const XMMATRIX & comboMatrix, bool normalize)
{
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, comboMatrix);
	// Left clipping plane
	planes[FRUSTUM_LEFT].normal.x = mat._14 + mat._11;
	planes[FRUSTUM_LEFT].normal.y = mat._24 + mat._21;
	planes[FRUSTUM_LEFT].normal.z = mat._34 + mat._31;
	planes[FRUSTUM_LEFT].distance = mat._44 + mat._41;
	// Right clipping plane
	planes[FRUSTUM_RIGHT].normal.x = mat._14 - mat._11;
	planes[FRUSTUM_RIGHT].normal.y = mat._24 - mat._21;
	planes[FRUSTUM_RIGHT].normal.z = mat._34 - mat._31;
	planes[FRUSTUM_RIGHT].distance = mat._44 - mat._41;
	// Top clipping plane
	planes[FRUSTUM_TOP].normal.x = mat._14 - mat._12;
	planes[FRUSTUM_TOP].normal.y = mat._24 - mat._22;
	planes[FRUSTUM_TOP].normal.z = mat._34 - mat._32;
	planes[FRUSTUM_TOP].distance = mat._44 - mat._42;
	// Bottom clipping plane  
	planes[FRUSTUM_DOWN].normal.x = mat._14 + mat._12;
	planes[FRUSTUM_DOWN].normal.y = mat._24 + mat._22;
	planes[FRUSTUM_DOWN].normal.z = mat._34 + mat._32;
	planes[FRUSTUM_DOWN].distance = mat._44 + mat._42;
	// Near clipping plane  
	planes[FRUSTUM_NEAR].normal.x = mat._13;
	planes[FRUSTUM_NEAR].normal.y = mat._23;
	planes[FRUSTUM_NEAR].normal.z = mat._33;
	planes[FRUSTUM_NEAR].distance = mat._43;
	// Far clipping plane 
	planes[FRUSTUM_FAR].normal.x = mat._14 - mat._13;
	planes[FRUSTUM_FAR].normal.y = mat._24 - mat._23;
	planes[FRUSTUM_FAR].normal.z = mat._34 - mat._33;
	planes[FRUSTUM_FAR].distance = mat._44 - mat._43;
	// Normalize the plane equations, if requested  
	if (normalize == true)
	{
		for (int i = 0; i < 6; i++)
		{
			planes[i].Normalize();
		}
	}
}

int CCamera::ViewFrustum::SphereIntersection(const XMFLOAT3& centerPos, const float radius)
{
	float dis;

	for (int i = 0; i < 6; i++)
	{
		dis = planes[i].DistanceToPoint(centerPos);

		if (dis < -radius)  // 外离
			return FRUSTUM_OUT;
		if (fabs(dis) < radius) // 相交
			return FRUSTUM_INTERSECT;
	}
	return FRUSTUM_IN;  // 内含
}

void CCamera::ViewFrustum::plane_tag::Normalize()
{
	XMVECTOR nor;
	nor = XMLoadFloat3(&normal);
	XMFLOAT3 length;
	XMStoreFloat3(&length, XMVector3Length(nor));
	float scale = 1 / length.x;

	normal.x *= scale;
	normal.y *= scale;
	normal.z *= scale;
	distance *= scale;
}

float CCamera::ViewFrustum::plane_tag::DistanceToPoint(const XMFLOAT3 & point)
{
	return normal.x * point.x + normal.y * point.y + normal.z * point.z + distance;
}
