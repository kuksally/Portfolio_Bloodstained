#include "Stage2.h"
#include "CommonFunction.h"
#include "Image.h"
#include "TilemapTool.h"
#include "BattleUI.h"
#include "Camera.h"
#include "Player.h"
#include "EnemyManager.h"
#include "MissileManager.h"
#include "CheckCollision.h"
#include "GameOver.h"
#include "Stage1.h"
#include "Stage3.h"

TILE_INFO Stage2::tileInfo[TILE_X * TILE_Y];
PIXEL_TILE_INFO Stage2::pixelTileInfo[TILE_X * TILE_Y];

HRESULT Stage2::Init()
{
	Camera::GetSingleton()->SetMapMaxPos({ 1460, 300 });
	Camera::GetSingleton()->SetMapMinPos({ WINSIZE_X / 2, 300 });

	SceneManager::GetSingleton()->AddScene("GameOver", new GameOver());
	SceneManager::GetSingleton()->AddScene("Stage1", new Stage1());
	SceneManager::GetSingleton()->AddScene("Stage3", new Stage3());

	// 픽셀맵 -> 픽셀 충돌을 위해 새로 맵을 그려줄 공간이 필요함
	pixelMap = new Image();
	pixelMap->Init(TILE_X * TILESIZE, TILE_Y * TILESIZE);

	// 타일 이미지
	sampleTile = ImageManager::GetSingleton()->FindImage("SampleTile");
	pixelTile = ImageManager::GetSingleton()->FindImage("PixelTile");

	// 배경
	black = ImageManager::GetSingleton()->FindImage("Black");
	backGroundImage1 = ImageManager::GetSingleton()->FindImage("BackGround6");
	backGroundImage2 = ImageManager::GetSingleton()->FindImage("BackGround7");

	TileLoad(2);
	PixelLoad(2);

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
	enemyMgr->SetEnemy1Count(4);
	enemyMgr->Init(2);

	ui = new BattleUI();
	ui->Init(player);

	collisionCheck = new CheckCollision();
	collisionCheck->Init();

	return S_OK;
}

void Stage2::Release()
{
	SAFE_RELEASE(ui);
	SAFE_RELEASE(player);
	SAFE_RELEASE(enemyMgr);
	SAFE_RELEASE(collisionCheck);
}

void Stage2::Update()
{
	if (player)
	{
		player->Update();
	}

	if (enemyMgr)
	{
		enemyMgr->Update();
	}

	if (collisionCheck)
	{
		collisionCheck->PlayerEnemyCollisionCheck(player, enemyMgr);
		collisionCheck->Collision(player, enemyMgr);
	}
	
	if (player->GetIsLeftDoor() == true)
	{
		SceneManager::GetSingleton()->ChangeScene("Stage1");
		return;
	}
	if (player->GetIsRightDoor() == true)
	{
		SceneManager::GetSingleton()->ChangeScene("Stage3");
		return;
	}

	if (player->GetPlayerLives() == 0)
	{
		SceneManager::GetSingleton()->ChangeScene("GameOver");
	}
}

void Stage2::Render(HDC hdc)
{
	black->Render(hdc);

	backGroundImage1->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) + 60 - Camera::GetSingleton()->GetCameraPos().y);
	backGroundImage2->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) + 60 - Camera::GetSingleton()->GetCameraPos().y);


	if (ui)
	{
		ui->Render(hdc);
	}

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		sampleTile->FrameRender(hdc,
			tileInfo[i].rcTile.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x,
			tileInfo[i].rcTile.top + (WINSIZE_Y / 2) - 60 - Camera::GetSingleton()->GetCameraPos().y,
			tileInfo[i].frameX,
			tileInfo[i].frameY);

		//pixelTile->FrameRender(hdc,
		//	pixelTileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x,
		//	pixelTileInfo[i].rcTile.top,
		//	pixelTileInfo[i].frameX,
		//	pixelTileInfo[i].frameY);
	}


	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
	{
		pixelMap->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) - 60 - Camera::GetSingleton()->GetCameraPos().y);
	}

	if (enemyMgr)
	{
		enemyMgr->Render(hdc);
	}

	if (player)
	{
		player->Render(hdc);
	}


}

void Stage2::TileLoad(int stageNum)
{
	DWORD readBytes;

	string fileName = "Save/saveMapData";
	fileName += to_string(stageNum) + ".map";

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}

void Stage2::PixelLoad(int stageNum)
{
	string fileName = "Save/savePixelMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD readBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, pixelTileInfo, sizeof(PIXEL_TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}
