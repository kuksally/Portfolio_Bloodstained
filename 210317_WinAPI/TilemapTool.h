#pragma once
#include "CameraObject.h"

class Button;
class Image;
class TilemapTool : public CameraObject
{
private:
	Image* BackGround;	// ��� �̹���
	
	// ������ ���� �̹���
	Image* black;
	Image* moon;
	Image* backGroundImage[5];
	string backGround = "BackGround";

	// ī�޶�
	FPOINT pos;
	FPOINT targetPos;

	float moveSpeed;

	// ����Ÿ��
	Image* sampleTile;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcSample;

	// �ȼ�Ÿ��
	Image* pixelTileImage;
	PIXEL_TILE_INFO pixelInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcPixel;

	// ����Ÿ��
	static TILE_INFO tileInfo[TILE_X * TILE_Y];
	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];
	RECT rcMain;

	POINT ptStartSelectedFrame;
	POINT ptEndSelectedFrame;

	POINT ptSelected[2];
	HBRUSH hSelectedBrush;
	HBRUSH hOldSelectedBrush;

	// UI button
	Button* btnSave;
	Button* btnLoad;
	Button* btnStage;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	static void Save(int stageNum);
	static void Load(int stageNum);

	static void PixelSave(int stageNum);
	static void PixelLoad(int stageNum);

	void CameraMoveKey();

	void BackScreen();

	virtual ~TilemapTool() {};
};

