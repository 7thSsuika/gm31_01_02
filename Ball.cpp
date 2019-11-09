#include "main.h"
#include "renderer.h"
#include "model.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "Ball.h"

void Ball::Init()
{
	model = new CModel();
	model->Load("asset/miku_01.obj");

	m_Quaternion = XMQuaternionIdentity();
	m_Position = XMFLOAT3(0,3,0);
	m_Scale = XMFLOAT3(2.0f, 2.0f, 2.0f);
}

void Ball::Uninit()
{
	model->Unload();
	delete model;
}

void Ball::Update()
{
	if (CInput::GetKeyPress('Z'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
	if (CInput::GetKeyPress('C'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
	if (CInput::GetKeyPress('X'))
	{
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
	}
}

void Ball::Draw()
{
	CCamera* camera;
	camera = CManager::GetScene()->GetGameObject<CCamera>(0);
	if (camera->GetVisibility(m_Position) == false)
		return;
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationQuaternion(m_Quaternion);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);
	model->Draw();
}
