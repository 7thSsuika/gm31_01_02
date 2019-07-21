
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "texture.h"
#include "enemy.h"
#include <random>


void Enemy::Init()
{
	model = new CModel();
	model->Load("asset/miku_01.obj");
	model->SetMaterialColor(COLOR(1.0f, 0.4f, 0.4f, 1.0f));

	m_Position = XMFLOAT3(rand() % 300 / 10.0f - 15.0f, 1.0f, rand() % 400 / 10.0f);
	m_Rotation = XMFLOAT3(0.0f, 3.0f, 0.0f);
	m_Scale = XMFLOAT3(2.0f, 2.0f, 2.0f);
}

void Enemy::Uninit()
{
	model->Unload();
	delete model;
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);
	model->Draw();
}

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}
