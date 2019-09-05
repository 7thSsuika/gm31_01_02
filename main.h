#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NOMINMAX
#include <windows.h>
#include <assert.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#pragma comment(lib, "assimp.lib")

#include "gameObject.h"



#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")


#define SCREEN_WIDTH	(960)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)			// �E�C���h�E�̍���


HWND GetWindow();
