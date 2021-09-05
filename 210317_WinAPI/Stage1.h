#pragma once
#include "GameNode.h"

class Player;
class CheckCollision;
class Item;
class BattleUI;
class TilemapTool;
class Image;
class Stage1 : public GameNode
{
private:
	Image* sampleTile;
	Image* pixelTile;
	Image* pixelMap;
	Image* black;				// �� �ڿ� �⺻������ �� ������ ���
	Image* moon;				// �� �տ� ������ �� ���
	Image* backGroundImage[5];

	string backGround = "BackGround";

	BattleUI* ui;

	Item* item;

	CheckCollision* collisionCheck;

	Player* player;

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

	~Stage1() {};
};

