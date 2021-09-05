#include "SelectScene.h"
#include "Image.h"
#include "TitleScene.h"
#include "TilemapTool.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5_Boss.h"

HRESULT SelectScene::Init()
{
	SceneManager::GetSingleton()->AddScene("Title", new TitleScene());
	SceneManager::GetSingleton()->AddScene("TileMapTool", new TilemapTool());
	SceneManager::GetSingleton()->AddScene("Stage1", new Stage1());
	SceneManager::GetSingleton()->AddScene("Stage2", new Stage2());
	SceneManager::GetSingleton()->AddScene("Stage3", new Stage3());
	SceneManager::GetSingleton()->AddScene("Stage4", new Stage4());
	SceneManager::GetSingleton()->AddScene("Stage5", new Stage5_Boss());

	arrowPos.x = WINSIZE_X / 2 - 230;
	arrowPos.y = WINSIZE_Y / 1.53;

	selectIndex = 0;

	frameY = 0;
	maxFrameY = 4;

	time = 0.0f;
	maxTime = 150.0f;

	isPressKey = false;

	selectBackGround = ImageManager::GetSingleton()->FindImage("TitleBackGround");
	selectLogo = ImageManager::GetSingleton()->FindImage("TitleLogo");
	selectArrow = ImageManager::GetSingleton()->FindImage("SelectArrow");
	
	for (int i = 0; i < 3; i++)
	{
		selectList[i] = ImageManager::GetSingleton()->FindImage("SelectList");
	}

	return S_OK;
}

void SelectScene::Release()
{
}

void SelectScene::Update()
{	
	PressKey();

	if (isPressKey == true)
	{
		time++;
		if (time >= maxTime)
		{
			frameY++;

			if (frameY >= maxFrameY)
			{
				if (selectIndex == 0)
				{
					SceneManager::GetSingleton()->ChangeScene("Stage1");
					//SceneManager::GetSingleton()->ChangeScene("Stage2");
					//SceneManager::GetSingleton()->ChangeScene("Stage3");
					//SceneManager::GetSingleton()->ChangeScene("Stage4");
					//SceneManager::GetSingleton()->ChangeScene("Stage5");
				}

				if (selectIndex == 1)
				{
					SceneManager::GetSingleton()->ChangeScene("TileMapTool");
				}

				if (selectIndex == 2)
				{
					SceneManager::GetSingleton()->ChangeScene("Title");
				}

				frameY = 0;
			}

			time = 0;
		}
	}
}

void SelectScene::Render(HDC hdc)
{
	selectBackGround->Render(hdc);

	selectLogo->FrameRender(hdc, WINSIZE_X / 2, WINSIZE_Y / 2.5, 0, frameY, true);

	selectArrow->Render(hdc, arrowPos.x, arrowPos.y);

	for (int i = 0; i < 3; i++)
	{
		selectList[i]->FrameRender(hdc, WINSIZE_X / 2 - 135, (WINSIZE_Y / 1.5) + (50 * i), 0, i);
	}

	if (selectIndex == 0)
	{
		selectList[0]->FrameRender(hdc, WINSIZE_X / 2 - 135, WINSIZE_Y / 1.5, 0, 3);
	}

	if (selectIndex == 1)
	{
		selectList[1]->FrameRender(hdc, WINSIZE_X / 2 - 135, WINSIZE_Y / 1.5 + 50, 0, 4);
	}

	if (selectIndex == 2)
	{
		selectList[2]->FrameRender(hdc, WINSIZE_X / 2 - 135, WINSIZE_Y / 1.5 + 100, 0, 5);
	}
}

void SelectScene::PressKey()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
	{
		arrowPos.y -= 53;

		selectIndex--;

		if (arrowPos.y < WINSIZE_Y / 1.53)
		{
			arrowPos.y = WINSIZE_Y / 1.5 + 100;
		}

		if (selectIndex < 0)
		{
			selectIndex = 2;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
	{
		arrowPos.y += 53;

		selectIndex++;

		if (arrowPos.y > WINSIZE_Y / 1.5 + 100)
		{
			arrowPos.y = WINSIZE_Y / 1.53;
		}

		if (selectIndex > 2)
		{
			selectIndex = 0;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
	{
		isPressKey = true;
	}
}
