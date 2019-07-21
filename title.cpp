#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "renderer.h"
#include "audio_clip.h"
#include "scene.h"
#include "input.h"
#include "manager.h"
#include "title.h"
#include "camera.h"
#include "polygon.h"
#include "game.h"


void Title::Init()
{
	AddGameObject<CCamera>(0);
	AddGameObject<CPolygon>(4);

	bgm = new CAudioClip();
	bgm->Load("asset/Audio/a.wav");
	bgm->Play(true);
}


void Title::Update()
{
	CScene::Update();
	if (CInput::GetKeyTrigger('K'))
	{
		CManager::SetScene<Game>();
	}
}

void Title::Uninit()
{
	bgm->Stop();
	bgm->Unload();
	delete bgm;
	CScene::Uninit();
}
