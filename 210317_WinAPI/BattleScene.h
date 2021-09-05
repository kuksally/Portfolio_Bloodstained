#pragma once
#include "GameNode.h"

// ���� MainGame Ŭ������ ������ �̵�
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
	Image* black;				// �� �ڿ� �⺻������ �� ������ ���
	Image* moon;				// �� �տ� ������ �� ���
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