#pragma once
#include "GameNode.h"

// 기존 MainGame 클래스의 내용을 이동
class CheckCollision;
class Boss;
class EnemyManager;
class Player;
class BattleUI;
class TilemapTool;
class Image;
class BattleScene : public GameNode
{
private:
	Image* sampleTile;
	Image* pixelTile;
	Image* pixelMap;
	Image* black;				// 맨 뒤에 기본적으로 깔릴 검은색 배경
	Image* moon;				// 그 앞에 고정될 달 배경
	Image* backGroundImage[5];

	string backGround = "BackGround";

	Player* player;

	EnemyManager* enemyMgr;

	Boss* boss;

	BattleUI* ui;

	CheckCollision* collisionCheck;

	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void BackScreen();

	void TileLoad(int stageNum);
	void PixelLoad(int stageNum);

	~BattleScene() {};
};