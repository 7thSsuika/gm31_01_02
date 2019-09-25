#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "renderer.h"
#include "audio_clip.h"
#include "input.h"
#include "camera.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "audio_clip.h"
#include "manager.h"

CScene* CManager::scene = nullptr;


void CManager::Init()
{
	CRenderer::Init();
	CInput::Init();
	CAudioClip::Init();

	SetScene<Title>();

}

void CManager::Uninit()
{
	scene->Uninit();
	delete scene;

	CAudioClip::Uninit();
	CInput::Uninit();
	CRenderer::Uninit();
}

void CManager::Update()
{

	scene->Update();
	CInput::Update();
}

void CManager::Draw()
{

	CRenderer::Begin();

	scene->Draw();

	CRenderer::End();

}

CScene*  CManager::GetScene()
{
	return scene;
}
