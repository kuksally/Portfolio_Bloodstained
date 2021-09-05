#include "GameOver.h"
#include "Image.h"
#include "TitleScene.h"
#include "Stage1.h"

HRESULT GameOver::Init()
{
	SceneManager::GetSingleton()->AddScene("Stage1", new Stage1());
	SceneManager::GetSingleton()->AddScene("Title", new TitleScene());

	gameOver = ImageManager::GetSingleton()->FindImage("GameOverBackGround");
	gameOverArrow = ImageManager::GetSingleton()->FindImage("GameOverArrow");
	
	for (int i = 0; i < 3; i++)
	{
		gameOverList[i] = ImageManager::GetSingleton()->FindImage("GameOverList");
	}
	
	arrowPos.x = WINSIZE_X / 2 - 175;
	arrowPos.y = WINSIZE_Y / 2 - 65;

	gameOverListIndex = 0;

	frameY = 0;
	maxFrameY = 3;

	time = 0.0f;
	maxTime = 700.0f;

    return S_OK;
}

void GameOver::Release()
{
}

void GameOver::Update()
{
	PressKey();

	time++;
	if (time >= maxTime)
	{
		frameY++;

		if (frameY >= maxFrameY)
		{
			frameY = 0;

			if (frameY == maxFrameY)
			{
				frameY = 0;
			}
		}

		time = 0;
	}
}

void GameOver::Render(HDC hdc)
{
	gameOver->Render(hdc);

	for (int i = 0; i < 3; i++)
	{
		gameOverList[i]->FrameRender(hdc, WINSIZE_X / 2 - 125, (WINSIZE_Y / 2 - 50) + (80 * i), 0, i);
	}

	if (gameOverListIndex == 0)
	{
		gameOverList[0]->FrameRender(hdc, WINSIZE_X / 2 - 125, WINSIZE_Y / 2 - 50, 0, 3);
	}
	
	if (gameOverListIndex == 1)
	{
		gameOverList[1]->FrameRender(hdc, WINSIZE_X / 2 - 125, WINSIZE_Y / 2 + 30, 0, 4);
	}

	if (gameOverListIndex == 2)
	{
		gameOverList[2]->FrameRender(hdc, WINSIZE_X / 2 - 125, WINSIZE_Y / 2 + 110, 0, 5);
	}

	gameOverArrow->Render(hdc, arrowPos.x, arrowPos.y);
}

void GameOver::PressKey()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
	{
		arrowPos.y -= 80;

		gameOverListIndex--;
		
		if (arrowPos.y < WINSIZE_Y / 2 - 65)
		{
			arrowPos.y = WINSIZE_Y / 2 + 100;
		}

		if (gameOverListIndex < 0)
		{
			gameOverListIndex = 2;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
	{
		arrowPos.y += 80;

		gameOverListIndex++;

		if (arrowPos.y > WINSIZE_Y / 2 + 100)
		{
			arrowPos.y = WINSIZE_Y / 2 - 65;
		}

		if (gameOverListIndex > 2)
		{
			gameOverListIndex = 0;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
	{
		if (gameOverListIndex == 0)
		{
			SceneManager::GetSingleton()->ChangeScene("Stage1");
		}

		if (gameOverListIndex == 1 || gameOverListIndex == 2)
		{
			SceneManager::GetSingleton()->ChangeScene("Title");
		}
	}
}