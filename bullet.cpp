
#include <vector>
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "texture.h"

#include "scene.h"
#include "manager.h"
#include "bullet.h"
#include "field.h"
#include "enemy.h"

void Bullet::Init()
{
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	model = new CModel();
	model->Load("asset/miku_01.obj");
}

void Bullet::Uninit()
{
	model->Unload();
	delete model;
}

void Bullet::Update()
{
	m_Position.z += 0.25f;
	m_Position.y = CManager::GetScene()->GetGameObject<CField>(0)->GetHeight(m_Position) + 1.75f;
	std::vector<Enemy*> enemys;
	enemys = CManager::GetScene()->GetGameObjects<Enemy>(0);
	XMVECTOR mPos = XMLoadFloat3(&m_Position);
	for (Enemy* enemy : enemys)
	{
		XMFLOAT3 enemyPos = enemy->GetPosition();
		XMVECTOR ePos = XMLoadFloat3(&enemyPos);
		XMVECTOR distance = ePos - mPos;
		if (*XMVector3Length(distance).m128_f32 <= 0.5f)
		{
			CManager::GetScene()->DestroyGameObject(enemy);
			CManager::GetScene()->DestroyGameObject(this);
			CManager::GetScene()->AddGameObject<Enemy>(0);
		}
	}
	if (m_Position.z >= 60)
	{
		CManager::GetScene()->DestroyGameObject(this);
		return;
	}
}

void Bullet::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	model->Draw();
}

void Bullet::SetPosition(XMFLOAT3 position)
{
	m_Position = position;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
