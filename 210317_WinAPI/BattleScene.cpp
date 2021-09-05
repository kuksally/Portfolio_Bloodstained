#include "BattleScene.h"
#include "CommonFunction.h"
#include "Image.h"
#include "TilemapTool.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "MiddleBoss.h"
#include "Missile.h"
#include "Camera.h"
#include "TitleScene.h"
#include "GameOver.h"
#include "CheckCollision.h"
#include "BattleUI.h"

TILE_INFO BattleScene::tileInfo[TILE_X * TILE_Y];
PIXEL_TILE_INFO BattleScene::pixelTileInfo[TILE_X * TILE_Y];

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	Camera::GetSingleton()->SetMapMaxPos({ 2000, 300 });

	Camera::GetSingleton()->SetMapMinPos({ WINSIZE_X / 2, 0 });

	SceneManager::GetSingleton()->AddScene("Title", new TitleScene());
	SceneManager::GetSingleton()->AddScene("GameOver", new GameOver());

	// 픽셀맵 -> 픽셀 충돌을 위해 새로 맵을 그려줄 공간이 필요함
	pixelMap = new Image();
	pixelMap->Init(TILE_X * TILESIZE, TILE_Y * TILESIZE);

	// 타일 이미지
	sampleTile = ImageManager::GetSingleton()->FindImage("SampleTile");
	pixelTile = ImageManager::GetSingleton()->FindImage("PixelTile");

	// 배경
	black = ImageManager::GetSingleton()->FindImage("Black");
	moon = ImageManager::GetSingleton()->FindImage("Moon");

	for (int i = 0; i < 5; i++)
	{
		backGround += to_string(i + 1);
		backGroundImage[i] = ImageManager::GetSingleton()->FindImage(backGround);
		backGround.pop_back();
	}

	TileLoad(1);
	PixelLoad(1);

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		pixelTile->FrameRender(pixelMap->GetMemDC(),
			pixelTileInfo[i].rcTile.left,
			pixelTileInfo[i].rcTile.top,
			pixelTileInfo[i].frameX,
			pixelTileInfo[i].frameY);
	}

	player = new Player();
	player->Init();
	player->SetPixelMap(pixelMap);

	enemyMgr = new EnemyManager();
	enemyMgr->Init();

	boss = new MiddleBoss();
	boss->Init(WINSIZE_X + 400, WINSIZE_Y + 110);
	
	ui = new BattleUI();
	ui->Init(player);

	collisionCheck = new CheckCollision();
	collisionCheck->Init();

	return S_OK;
}

void BattleScene::Release()
{
	SAFE_RELEASE(player);
	SAFE_RELEASE(enemyMgr);
	SAFE_RELEASE(boss);
	SAFE_RELEASE(ui);
	SAFE_RELEASE(collisionCheck);
}

void BattleScene::Update()
{
	if (player)
	{
		player->Update();
	}

	if (enemyMgr)
	{
		enemyMgr->Update();
	}

	if (boss)
	{
		boss->Update();
	}

	if (collisionCheck)
	{
		collisionCheck->PlayerEnemyCollisionCheck(player, enemyMgr);
		collisionCheck->PlayerBossCollisionCheck(player, boss);
	}

	if (player->GetPlayerLives() == 0)
	{
		SceneManager::GetSingleton()->ChangeScene("GameOver");
	}
	
	BackScreen();
}

void BattleScene::Render(HDC hdc)
{
	black->Render(hdc);

	moon->Render(hdc, 0, 120);

	for (int i = 0; i < 5; i++)
	{
		backGroundImage[i]->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) + 60 - Camera::GetSingleton()->GetCameraPos().y);
	}

#pragma region 움직이는 배경(사용여부 고민중)
	//for (int i = 0; i < 5; i++)
	//{
	//	if (i == 3)
	//	{
	//		continue;
	//	}

	//	if (i == 4)
	//	{
	//		continue;
	//	}

	//	backGroundImage[i]->Render(hdc, 0 - (Camera::GetSingleton()->GetCameraPos().x / (i + 1)), 120);		
	//}
	//backGroundImage[3]->Render(hdc,0 -Camera::GetSingleton()->GetCameraPos().x, 120);
	//backGroundImage[4]->Render(hdc,0 -Camera::GetSingleton()->GetCameraPos().x, 120);
#pragma endregion

	if (ui)
	{
		ui->Render(hdc);
	}

	if (boss)
	{
		boss->Render(hdc);
	}

	if (enemyMgr)
	{
		enemyMgr->Render(hdc);
	}

	if (player)
	{
		player->Render(hdc);
	}

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		sampleTile->FrameRender(hdc,
			tileInfo[i].rcTile.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x,
			tileInfo[i].rcTile.top+ (WINSIZE_Y / 2) - 60 - Camera::GetSingleton()->GetCameraPos().y,
			tileInfo[i].frameX,
			tileInfo[i].frameY);

		//pixelTile->FrameRender(hdc,
		//	pixelTileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x,
		//	pixelTileInfo[i].rcTile.top,
		//	pixelTileInfo[i].frameX,
		//	pixelTileInfo[i].frameY);
	}

	//pixelMap->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) - 60 - Camera::GetSingleton()->GetCameraPos().y);
}

void BattleScene::BackScreen()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("Title");
	}
}

void BattleScene::TileLoad(int stageNum)
{
	DWORD readBytes;

	string fileName = "Save/saveMapData";
	fileName += to_string(stageNum) + ".map";

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}

void BattleScene::PixelLoad(int stageNum)
{
	string fileName = "Save/savePixelMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD readBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, pixelTileInfo, sizeof(PIXEL_TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}