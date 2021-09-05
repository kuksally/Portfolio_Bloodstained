// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include <Windows.h>
#include <string>
#include <ctime>
#include <bitset>
#include <map>
#include <vector>
#include <time.h>
#include <random>

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "GameData.h"

#define WINSIZE_X	1080
#define WINSIZE_Y	720

#define TILEMAPTOOLSIZE_X	1080
#define TILEMAPTOOLSIZE_Y	720

#define ASTARSIZE_X	1620
#define ASTARSIZE_Y	900

#define TILESIZE		40
#define	TILE_X			50
#define	TILE_Y			40

// 샘플타일 정보
#define SAMPLE_TILE_X	7//4
#define SAMPLE_TILE_Y   10//5

#define PI			3.141592f
#define DegToRad(x)	((x) * PI / 180.0f)
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, *PFPOINT;

typedef struct tagTile
{
	RECT rcTile;
	int frameX;
	int frameY;
	int num;
} TILE_INFO;

typedef struct pixelTile
{
	RECT rcTile;
	int frameX;
	int frameY;
}PIXEL_TILE_INFO;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;