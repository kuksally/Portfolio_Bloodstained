#pragma once
#include "GameNode.h"

class Boss;
class EnemyManager;
class Player;
class CheckCollision;
class BattleUI;
class TilemapTool;
class Image;
class Stage3 : public GameNode
{
private:
	Image* sampleTile;
	Image* pixelTile;
	Image* pixelMap;
	Image* black;				// 맨 뒤에 기본적으로 깔릴 검은색 배경
	Image* middleBossBackGround;

	BattleUI* ui;
	
	CheckCollision* collisionCheck;
	
	Player* player;
	
	EnemyManager* enemyMgr;

	Boss* boss;

	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void TileLoad(int stageNum);
	void PixelLoad(int stageNum);

	~Stage3() {};
};

