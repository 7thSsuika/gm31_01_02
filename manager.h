#pragma once

class CScene;
class CManager
{
private:
	static CScene* scene;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static CScene* GetScene();
	template<typename T>
	static void SetScene()
	{
		if (scene != nullptr)
		{
			scene->Uninit();
			delete scene;
		}
		T* _scene = new T();
		_scene->Init();
		scene = _scene;
	}
};