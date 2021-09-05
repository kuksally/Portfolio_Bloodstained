#include "TilemapTool.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Button.h"
#include "TitleScene.h"
#include "Camera.h"

TILE_INFO TilemapTool::tileInfo[TILE_X * TILE_Y];
PIXEL_TILE_INFO TilemapTool::pixelTileInfo[TILE_X * TILE_Y];

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y);

	Camera::GetSingleton()->SetMapMaxPos({ 4000, 720 });
	Camera::GetSingleton()->SetMapMinPos({ 0, 0 });

	SceneManager::GetSingleton()->AddScene("Title", new TitleScene());

	pos.x = 0;
	pos.y = TILEMAPTOOLSIZE_Y / 2;

	moveSpeed = 300.0f;

	// 배경 이미지
	//BackGround = ImageManager::GetSingleton()->FindImage("BackGround4");		// stage1
	//BackGround = ImageManager::GetSingleton()->FindImage("BackGround7");		// stage2
	//BackGround = ImageManager::GetSingleton()->FindImage("MB_BackGround");	// stage3
	//BackGround = ImageManager::GetSingleton()->FindImage("B_BackGround5");	// stage4
	//BackGround = ImageManager::GetSingleton()->FindImage("B_BackGround6");	// stage5

	// 영상을 위한 배경 이미지
	black = ImageManager::GetSingleton()->FindImage("Black");
	moon = ImageManager::GetSingleton()->FindImage("Moon");
	for (int i = 0; i < 5; i++)
	{
		backGround += to_string(i + 1);
		backGroundImage[i] = ImageManager::GetSingleton()->FindImage(backGround);
		backGround.pop_back();
	}

	// 샘플타일
	sampleTile = ImageManager::GetSingleton()->FindImage("SampleTile");

	// 픽셀타일
	pixelTileImage = ImageManager::GetSingleton()->ImageManager::GetSingleton()->FindImage("PixelTile");

	hSelectedBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	// 메인 공간 렉트 설정
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			tileInfo[i * TILE_X + j].frameX = 0;
			tileInfo[i * TILE_X + j].frameY = 0;

			tileInfo[i * TILE_X + j].rcTile.left = TILESIZE * j;
			tileInfo[i * TILE_X + j].rcTile.top = TILESIZE * i;
			tileInfo[i * TILE_X + j].rcTile.right = tileInfo[i * TILE_X + j].rcTile.left + TILESIZE;
			tileInfo[i * TILE_X + j].rcTile.bottom = tileInfo[i * TILE_X + j].rcTile.top + TILESIZE;
		}
	}

	// 샘플 공간 & 픽셀 공간 렉트 설정
	for (int i = 0; i < SAMPLE_TILE_Y; i++)
	{
		for (int j = 0; j < SAMPLE_TILE_X; j++)
		{
			sampleTileInfo[i * SAMPLE_TILE_X + j].frameX = j;
			sampleTileInfo[i * SAMPLE_TILE_X + j].frameY = i;

			sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.left = TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (TILESIZE * j);
			sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.top = (TILESIZE * i);
			sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.right = sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.left + TILESIZE;
			sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.bottom = sampleTileInfo[i * SAMPLE_TILE_X + j].rcTile.top + TILESIZE;

			pixelTileInfo[i * SAMPLE_TILE_X + j].frameX = j;
			pixelTileInfo[i * SAMPLE_TILE_X + j].frameY = i;

			pixelInfo[i * SAMPLE_TILE_X + j].rcTile.left = TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (TILESIZE * j);
			pixelInfo[i * SAMPLE_TILE_X + j].rcTile.top = (TILESIZE * i);
			pixelInfo[i * SAMPLE_TILE_X + j].rcTile.right = pixelInfo[i * SAMPLE_TILE_X + j].rcTile.left + TILESIZE;
			pixelInfo[i * SAMPLE_TILE_X + j].rcTile.bottom = pixelInfo[i * SAMPLE_TILE_X + j].rcTile.top + TILESIZE;
		}
	}

	// UI Button
	ImageManager::GetSingleton()->FindImage("SaveButton");
	ImageManager::GetSingleton()->FindImage("LoadButton");

	btnSave = new Button();
	btnSave->Init("SaveButton", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() - 60, TILEMAPTOOLSIZE_Y / 2 + 50);
	btnSave->SetFunc(Save, 1);

	btnLoad = new Button();
	btnLoad->Init("LoadButton", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + 50, TILEMAPTOOLSIZE_Y / 2 + 50);
	btnLoad->SetFunc(Load, 1);

	btnStage = new Button();
	btnStage->Init("StageButton", TILEMAPTOOLSIZE_X - sampleTile->GetWidth() - 280, TILEMAPTOOLSIZE_Y / 2 + 200);

	return S_OK;
}

void TilemapTool::Release()
{
	SAFE_RELEASE(btnSave);
	SAFE_RELEASE(btnLoad);
}

void TilemapTool::Update()
{
	worldPos.x = pos.x;
	worldPos.y = pos.y;

	if (btnSave)
	{
		btnSave->SetCurrStageIndex(btnStage->GetCurrStageIndex());
		btnSave->Update();
	}

	if (btnLoad)
	{
		btnLoad->SetCurrStageIndex(btnStage->GetCurrStageIndex());
		btnLoad->Update();
	}

	if (btnStage)
	{
		btnStage->Update();
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
		{
			Load(1);
			PixelLoad(1);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
		{
			Load(2);
			PixelLoad(2);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))
		{
			Load(3);
			PixelLoad(3);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F4))
		{
			Load(4);
			PixelLoad(4);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F5))
		{
			Load(5);
			PixelLoad(5);
		}
	}
	else
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
		{
			Save(1);
			PixelSave(1);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
		{
			Save(2);
			PixelSave(2);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F3))
		{
			Save(3);
			PixelSave(3);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F4))
		{
			Save(4);
			PixelSave(4);
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F5))
		{
			Save(5);
			PixelSave(5);
		}
	}

	// 메인 영역 계산
	rcMain.left = 0 - Camera::GetSingleton()->GetCameraPos().x;
	rcMain.top = 0;
	rcMain.right = 800;
	rcMain.bottom = rcMain.top + (TILESIZE * TILE_Y);

	// 샘플 영역 계산
	rcSample.left = TILEMAPTOOLSIZE_X - sampleTile->GetWidth();
	rcSample.top = 0;
	rcSample.right = TILEMAPTOOLSIZE_X;
	rcSample.bottom = sampleTile->GetHeight();

	// 픽셀 영역 계산
	rcPixel.left = 0 - Camera::GetSingleton()->GetCameraPos().x;
	rcPixel.top = 0;
	rcPixel.right = rcPixel.left + (TILESIZE * TILE_X);
	rcPixel.bottom = rcPixel.top + (TILESIZE * TILE_Y);

	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			tileInfo[i * TILE_X + j].rcTile.left = (TILESIZE * j) - Camera::GetSingleton()->GetCameraPos().x;
			tileInfo[i * TILE_X + j].rcTile.top = (TILESIZE * i) - Camera::GetSingleton()->GetCameraPos().y;
			tileInfo[i * TILE_X + j].rcTile.right = (tileInfo[i * TILE_X + j].rcTile.left + TILESIZE);
			tileInfo[i * TILE_X + j].rcTile.bottom = (tileInfo[i * TILE_X + j].rcTile.top + TILESIZE);

			pixelTileInfo[i * TILE_X + j].rcTile.left = (TILESIZE * j) - Camera::GetSingleton()->GetCameraPos().x;
			pixelTileInfo[i * TILE_X + j].rcTile.top = (TILESIZE * i) - Camera::GetSingleton()->GetCameraPos().y;
			pixelTileInfo[i * TILE_X + j].rcTile.right = (pixelTileInfo[i * TILE_X + j].rcTile.left + TILESIZE);
			pixelTileInfo[i * TILE_X + j].rcTile.bottom = (pixelTileInfo[i * TILE_X + j].rcTile.top + TILESIZE);
		}
	}

	if (PtInRect(&rcMain, g_ptMouse))
	{
		// 마우스 왼쪽 버튼 클릭시 좌표 사용
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON)
			|| KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TILE_X * TILE_Y; i++)
			{
				if (tileInfo[i].rcTile.right < 800)
				{
					if (PtInRect(&(tileInfo[i].rcTile), g_ptMouse))
					{
						tileInfo[i].frameX = ptStartSelectedFrame.x;
						tileInfo[i].frameY = ptStartSelectedFrame.y;

						pixelTileInfo[i].frameX = ptStartSelectedFrame.x;
						pixelTileInfo[i].frameY = ptStartSelectedFrame.y;

						for (int j = 0; j <= ptEndSelectedFrame.y - ptStartSelectedFrame.y; j++)
						{
							for (int k = 0; k <= ptEndSelectedFrame.x - ptStartSelectedFrame.x; k++)
							{
								if ((i % TILE_X) + k >= TILE_X) continue;
								if ((i / TILE_X) + j >= TILE_Y) continue;

								tileInfo[i + j * TILE_X + k].frameX = ptStartSelectedFrame.x + k;
								tileInfo[i + j * TILE_X + k].frameY = ptStartSelectedFrame.y + j;

								pixelTileInfo[i + j * TILE_X + k].frameX = ptStartSelectedFrame.x + k;
								pixelTileInfo[i + j * TILE_X + k].frameY = ptStartSelectedFrame.y + j;
							}
						}

						break;
					}
				}
			}
		}
	}
	else if (PtInRect(&rcSample, g_ptMouse))
	{
		// 마우스 왼쪽 버튼 클릭시 좌표 사용
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			// 2) 마우스 좌표로 인덱스 계산
			int posX = g_ptMouse.x - rcSample.left;
			int posY = g_ptMouse.y - rcSample.top;
			ptStartSelectedFrame.x = posX / TILESIZE;
			ptStartSelectedFrame.y = posY / TILESIZE;

			ptSelected[0] = g_ptMouse;
		}
		else if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			int posX = g_ptMouse.x - rcSample.left;
			int posY = g_ptMouse.y - rcSample.top;
			ptEndSelectedFrame.x = posX / TILESIZE;
			ptEndSelectedFrame.y = posY / TILESIZE;

			// 선택영역 초기화
			ptSelected[0].x = -1;
			ptSelected[0].y = -1;
			ptSelected[1].x = -1;
			ptSelected[1].y = -1;
		}
		else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			ptSelected[1] = g_ptMouse;
		}
	}

	CameraMoveKey();

	Camera::GetSingleton()->SetCameraPos(pos);

	CameraObject::UpdatePos();

	BackScreen();
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOLSIZE_X, TILEMAPTOOLSIZE_Y, WHITENESS);

	// 배경 이미지
	//BackGround->Render(hdc, 0 - Camera::GetSingleton()->GetCameraPos().x, 120);

	// 영상을 위한 배경 이미지
	black->Render(hdc);
	moon->Render(hdc, 0, 120);
	for (int i = 0; i < 5; i++)
	{
		backGroundImage[i]->Render(hdc, 0 - Camera::GetSingleton()->GetCameraPos().x, 120);
	}

	// 샘플타일 전체
	sampleTile->Render(hdc, TILEMAPTOOLSIZE_X - sampleTile->GetWidth(), 0);

	// 선택 영역 표시
	hOldSelectedBrush = (HBRUSH)SelectObject(hdc, hSelectedBrush);
	Rectangle(hdc, ptSelected[0].x, ptSelected[0].y, ptSelected[1].x, ptSelected[1].y);
	SelectObject(hdc, hOldSelectedBrush);

	// UI Button
	if (btnSave)    btnSave->Render(hdc);
	if (btnLoad)    btnLoad->Render(hdc);
	if (btnStage)	btnStage->Render(hdc);

	// 메인영역 전체
	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		if (tileInfo[i].rcTile.right < 800)
		{
			sampleTile->FrameRender(hdc,
				tileInfo[i].rcTile.left,
				tileInfo[i].rcTile.top,
				tileInfo[i].frameX,
				tileInfo[i].frameY);
		//Rectangle(hdc, tileInfo[i].rcTile.left, tileInfo[i].rcTile.top, tileInfo[i].rcTile.right, tileInfo[i].rcTile.bottom);
		}

		if (KeyManager::GetSingleton()->IsStayKeyDown('A'))
		{
			pixelTileImage->FrameRender(hdc,
				pixelTileInfo[i].rcTile.left,
				pixelTileInfo[i].rcTile.top,
				pixelTileInfo[i].frameX,
				pixelTileInfo[i].frameY);
		}
	}
	
	//Rectangle(hdc, rcMain.left, rcMain.top, rcMain.right, rcMain.bottom);
	
	// 선택된 타일
	if (ptStartSelectedFrame.x == ptEndSelectedFrame.x && ptStartSelectedFrame.y == ptEndSelectedFrame.y)
	{
		sampleTile->FrameRender(hdc,
			TILEMAPTOOLSIZE_X - sampleTile->GetWidth(),
			sampleTile->GetHeight() + 50,
			ptStartSelectedFrame.x,
			ptStartSelectedFrame.y,
			false, 3);
	}
	else
	{
		for (int i = 0; i <= ptEndSelectedFrame.y - ptStartSelectedFrame.y; i++)
		{
			for (int j = 0; j <= ptEndSelectedFrame.x - ptStartSelectedFrame.x; j++)
			{
				sampleTile->FrameRender(hdc,
					TILEMAPTOOLSIZE_X - sampleTile->GetWidth() + (j * TILESIZE),
					ptStartSelectedFrame.x + j,
					ptStartSelectedFrame.y + i,
					false, 1);
			}
		}
	}
}

void TilemapTool::Save(int stageNum)
{
	string fileName = "Save/saveMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD writtenBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &writtenBytes, NULL);

	CloseHandle(hFile);
}

void TilemapTool::Load(int stageNum)
{
	string fileName = "Save/saveMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD readBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, tileInfo, sizeof(TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}

void TilemapTool::PixelSave(int stageNum)
{
	string fileName = "Save/savePixelMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD writtenBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, pixelTileInfo, sizeof(PIXEL_TILE_INFO) * TILE_X * TILE_Y, &writtenBytes, NULL);

	CloseHandle(hFile);
}

void TilemapTool::PixelLoad(int stageNum)
{
	string fileName = "Save/savePixelMapData";
	fileName += to_string(stageNum) + ".map";

	DWORD readBytes;
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (ReadFile(hFile, pixelTileInfo, sizeof(PIXEL_TILE_INFO) * TILE_X * TILE_Y, &readBytes, NULL)) {}
	else MessageBox(g_hWnd, "저장파일 로드 실패", "실패", MB_OK);

	CloseHandle(hFile);
}

void TilemapTool::CameraMoveKey()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
	{
		pos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
	{
		pos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}
}

void TilemapTool::BackScreen()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
	{
		SceneManager::GetSingleton()->ChangeScene("Title");
	}
}
