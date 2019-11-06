#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "renderer.h"
#include "audio_clip.h"
#include "scene.h"
#include "game.h"
#include "result.h"
#include "title.h"
#include "input.h"
#include "manager.h"
#include "camera.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "polygon.h"
#include "enemy.h"
#include "Ball.h"

void Game::Init()
{
	AddGameObject<CCamera>(0);
	AddGameObject<CField>(0);
	AddGameObject<Enemy>(0);
	AddGameObject<Enemy>(0);
	AddGameObject<Enemy>(0);
	AddGameObject<Enemy>(0);
	AddGameObject<Enemy>(0);
	AddGameObject<Ball>(0);
	AddGameObject<Player>(0);
	AddGameObject<CPolygon>(4);

	bgm = new CAudioClip();
	bgm->Load("asset/Audio/a.wav");
	bgm->Play(true);
}


void Game::Update()
{
	CScene::Update();
	if (CInput::GetKeyTrigger('K'))
	{
		CManager::SetScene<Result>();
	}
}

void Game::Uninit()
{
	bgm->Stop();
	bgm->Unload();
	delete bgm;
	CScene::Uninit();
}
