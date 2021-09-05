#pragma once
#include "GameNode.h"

class EnemyManager;
class Player;
class CheckCollision;
class Item;
class BattleUI;
class TilemapTool;
class Image;
class Stage4 : public GameNode
{
private:
	Image* sampleTile;
	Image* pixelTile;
	Image* pixelMap;
	Image* black;				// 맨 뒤에 기본적으로 깔릴 검은색 배경
	Image* bossBackGround[5];

	string backGround = "B_BackGround";

	BattleUI* ui;

	Item* item;

	CheckCollision* collisionCheck;

	Player* player;

	EnemyManager* enemyMgr;

	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void TileLoad(int stageNum);
	void PixelLoad(int stageNum);

	~Stage4() {};
};