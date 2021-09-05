#include "Stage5_Boss.h"
#include "CommonFunction.h"
#include "Image.h"
#include "TilemapTool.h"
#include "BattleUI.h"
#include "CheckCollision.h"
#include "Camera.h"
#include "GameOver.h"
#include "Player.h"
#include "LastBoss.h"

TILE_INFO Stage5_Boss::tileInfo[TILE_X * TILE_Y];
PIXEL_TILE_INFO Stage5_Boss::pixelTileInfo[TILE_X * TILE_Y];

HRESULT Stage5_Boss::Init()
{
	srand(time(NULL));

	Camera::GetSingleton()->SetMapMaxPos({ 540, 300 });
	Camera::GetSingleton()->SetMapMinPos({ WINSIZE_X / 2, 300 });

	SceneManager::GetSingleton()->AddScene("GameOver", new GameOver());

	// 픽셀맵 -> 픽셀 충돌을 위해 새로 맵을 그려줄 공간이 필요함
	pixelMap = new Image();
	pixelMap->Init(TILE_X * TILESIZE, TILE_Y * TILESIZE);

	// 타일 이미지
	sampleTile = ImageManager::GetSingleton()->FindImage("SampleTile");
	pixelTile = ImageManager::GetSingleton()->FindImage("PixelTile");

	// 배경
	black = ImageManager::GetSingleton()->FindImage("Black");

	for (int i = 0; i < 6; i++)
	{
		backGround += to_string(i + 1);
		bossBackGround[i] = ImageManager::GetSingleton()->FindImage(backGround);
		backGround.pop_back();
	}

	TileLoad(5);
	PixelLoad(5);

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

	boss = new LastBoss();
	boss->Init(WINSIZE_X / 2, WINSIZE_Y / 2);

	ui = new BattleUI();
	ui->Init(player);

	collisionCheck = new CheckCollision();
	collisionCheck->Init();
	return S_OK;
}

void Stage5_Boss::Release()
{
	SAFE_RELEASE(ui);
	SAFE_RELEASE(collisionCheck);
	SAFE_RELEASE(player);
	SAFE_RELEASE(boss);
}

void Stage5_Boss::Update()
{
	if (player)
	{
		player->Update();
	}

	if (boss)
	{
		boss->Update();
	}

	if (collisionCheck)
	{
		collisionCheck->PlayerBossCollisionCheck(player, boss);
		collisionCheck->Collision2(player, boss);
	}

	if (boss->GetBossHp() == 0)
	{
		player->SetIsVictioy(true);
	}

	if (player->GetPlayerLives() == 0)
	{
		SceneManager::GetSingleton()->ChangeScene("GameOver");
	}
}

void Stage5_Boss::Render(HDC hdc)
{

	black->Render(hdc);

	for (int i = 0; i < 6; i++)
	{
		if (i != 4)
		{
			bossBackGround[i]->Render(hdc, (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, (WINSIZE_Y / 2) + 60 - Camera::GetSingleton()->GetCameraPos().y);
		}
	}

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


	if (boss)
	{
		boss->Render(hdc);
	}

	if (player)
	{
		player->Render(hdc);
	}
}

void Stage5_Boss::TileLoad(int stageNum)
{
	DWORD readBytes;

	string fileName = "Save/saveMapData";
	fileName += to_string(stageNum) + ".map";

	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}

void Stage5_Boss::PixelLoad(int stageNum)
{
	string fileName = "Save/savePixelMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD readBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, pixelTileInfo, sizeof(PIXEL_TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}
