
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

	viewFrustrum.ExtractPlanes(m_ViewMatrix * m_ProjectionMatrix, false);

}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	viewChanged = false;
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

	if (viewChanged)
	{
		// ビューマトリクス設定
		m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		XMVECTOR det;
		m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

		viewFrustrum.ExtractPlanes(m_ViewMatrix * m_ProjectionMatrix, false);
	}
}



void CCamera::Draw()
{
	CRenderer::SetViewMatrix(&m_ViewMatrix);
	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

bool CCamera::GetVisibility(XMFLOAT3 Position)
{
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
}

CCamera::ViewFrustrum::ViewFrustrum()
{
	ZeroMemory(planes, sizeof(planes));
}

void CCamera::ViewFrustrum::ExtractPlanes(const XMMATRIX & comboMatrix, bool normalize)
{
	XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, comboMatrix);
	// Left clipping plane
	planes[0].x = mat._14 + mat._11;
	planes[0].y = mat._24 + mat._21;
	planes[0].z = mat._34 + mat._31;
	planes[0].w = mat._44 + mat._41;
	// Right clipping plane
	planes[1].x = mat._14 - mat._11;
	planes[1].y = mat._24 - mat._21;
	planes[1].z = mat._34 - mat._31;
	planes[1].w = mat._44 - mat._41;
	// Top clipping plane
	planes[2].x = mat._14 - mat._12;
	planes[2].y = mat._24 - mat._22;
	planes[2].z = mat._34 - mat._32;
	planes[2].w = mat._44 - mat._42;
	// Bottom clipping plane  
	planes[3].x = mat._14 + mat._12;
	planes[3].y = mat._24 + mat._22;
	planes[3].z = mat._34 + mat._32;
	planes[3].w = mat._44 + mat._42;
	// Near clipping plane  
	planes[4].x = mat._13;
	planes[4].y = mat._23;
	planes[4].z = mat._33;
	planes[4].w = mat._43;
	// Far clipping plane 
	planes[5].x = mat._14 - mat._13;
	planes[5].y = mat._24 - mat._23;
	planes[5].z = mat._34 - mat._33;
	planes[5].w = mat._44 - mat._43;
	// Normalize the plane equations, if requested  
	if (normalize == true)
	{
		XMVECTOR vec;
		for (int i = 0; i < sizeof(planes) / sizeof(planes[0]); i++)
		{
			vec = XMLoadFloat4(&planes[i]);
			XMVector4Normalize(vec);
		}
	}
}
