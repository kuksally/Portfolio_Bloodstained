#pragma once
#include "GameNode.h"

class MissileManager;
class EnemyManager;
class Player;
class CheckCollision;
class Item;
class BattleUI;
class TilemapTool;
class Image;
class Stage2 : public GameNode
{
	Image* sampleTile;
	Image* pixelTile;
	Image* pixelMap;
	Image* black;				// 맨 뒤에 기본적으로 깔릴 검은색 배경
	Image* backGroundImage1;
	Image* backGroundImage2;

	BattleUI* ui;

	Item* item;

	vector<FPOINT>* vPos;

	CheckCollision* collisionCheck;

	Player* player;

	EnemyManager* enemyMgr;

	MissileManager* missileMgr;

	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void TileLoad(int stageNum);
	void PixelLoad(int stageNum);

	~Stage2() {};
};

