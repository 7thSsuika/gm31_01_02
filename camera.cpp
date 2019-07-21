
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

}


void CCamera::Uninit()
{


}


void CCamera::Update()
{
	int input = CInput::GetKeyPress(VK_NUMPAD8) - CInput::GetKeyPress(VK_NUMPAD2);
	if (input)
	{
		m_Rotation.x += input * 0.03f;
	}

	input = CInput::GetKeyPress(VK_NUMPAD6) - CInput::GetKeyPress(VK_NUMPAD4);
	if (input)
	{
		m_Rotation.y += input * 0.03;
	}

	input = CInput::GetKeyPress(VK_UP) - CInput::GetKeyPress(VK_DOWN);
	if (input)
	{
		m_Position.z += input * 0.13;
	}

	input = CInput::GetKeyPress(VK_RIGHT) - CInput::GetKeyPress(VK_LEFT);
	if (input)
	{
		m_Position.x += input * 0.13;
	}

	input = CInput::GetKeyPress(VK_NUMPAD7) - CInput::GetKeyPress(VK_NUMPAD9);
	if (input)
	{
		m_Position.y += input * 0.13;
	}
}



void CCamera::Draw()
{

	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;



	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);



	// ビューマトリクス設定
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	CRenderer::SetViewMatrix(&m_ViewMatrix);



	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);



}

