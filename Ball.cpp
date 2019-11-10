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
	m_Aabox.ChangePosition(m_Position);
	m_Aabox.ChangeScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
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
		AdjustPosition(-0.02f, 0.0f, 0.0f);
	}
	if (CInput::GetKeyPress('C'))
	{
		XMFLOAT3 vz = XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, -0.2f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		AdjustPosition(0.02f, 0.0f, 0.0f);
	}
	if (CInput::GetKeyPress('X'))
	{
		XMFLOAT3 vz = XMFLOAT3(1.0f, 0.0f, 0.0f);
		XMVECTOR axis = XMLoadFloat3(&vz);
		XMVECTOR rotation = XMQuaternionRotationAxis(axis, 0.2f);
		m_Quaternion = XMQuaternionMultiply(m_Quaternion, rotation);
		m_Quaternion = XMQuaternionNormalize(m_Quaternion);
		AdjustPosition(0.0f, 0.0f, 0.02f);
	}
}

void Ball::Draw()
{
	CCamera* camera;
	camera = CManager::GetScene()->GetGameObject<CCamera>(0);
	if (camera->GetVisibilityAabox(m_Aabox) == false) return;
	//if (camera->GetVisibility(m_Position) == false) return;
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationQuaternion(m_Quaternion);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);
	model->Draw();
}

void Ball::AdjustPosition(float x, float y, float z)
{
	m_Position.x += x;
	m_Position.y += y;
	m_Position.z += z;

	m_Aabox.ChangePosition(m_Position);
}
