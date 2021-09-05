#pragma once
#include "CameraObject.h"

class Button;
class Image;
class TilemapTool : public CameraObject
{
private:
	Image* BackGround;	// 배경 이미지
	
	// 영상을 위한 이미지
	Image* black;
	Image* moon;
	Image* backGroundImage[5];
	string backGround = "BackGround";

	// 카메라
	FPOINT pos;
	FPOINT targetPos;

	float moveSpeed;

	// 샘플타일
	Image* sampleTile;
	TILE_INFO sampleTileInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcSample;

	// 픽셀타일
	Image* pixelTileImage;
	PIXEL_TILE_INFO pixelInfo[SAMPLE_TILE_X * SAMPLE_TILE_Y];
	RECT rcPixel;

	// 메인타일
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

