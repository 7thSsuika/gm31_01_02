
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "camera.h"
#include "field.h"
#include "model.h"
#include "bullet.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "scene.h"




void Player::Init()
{
	m_Position = XMFLOAT3(0.0f, 1.0f, -5.0f);
	m_Rotation = XMFLOAT3(0.0f, 3.0f, 0.0f);
	m_Scale = XMFLOAT3(2.0f, 2.0f, 2.0f);
	model = new CModel();
	model->Load("asset/miku_01.obj");

	animModel = new CModelAnimation();
	animModel->Load("asset/Model/me.fbx");
}

void Player::Uninit()
{
	model->Unload();
	delete model;

	animModel->Unload();
	delete animModel;
}

void Player::Update()
{
	static float f = 0;
	if (CInput::GetKeyPress('A'))
	{
		f += 0.03;
	}
	if (CInput::GetKeyPress('D') && f > 0.03f)
	{
		f -= 0.03;
	}

	m_Rotation.y += 0.062f;

	m_Position.x += -cosf(m_Rotation.y) * f;
	m_Position.z += sinf(m_Rotation.y) * f;
	m_Position.y = CManager::GetScene()->GetGameObject<CField>(0)->GetHeight(m_Position) + 1.75f;

	if (CInput::GetKeyPress(VK_SPACE))
	{
		CScene* scene = CManager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(0);
		bullet->SetPosition(m_Position);
	}

}

void Player::Draw()
{
	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	//model->Draw();
	animModel->Draw();
}
