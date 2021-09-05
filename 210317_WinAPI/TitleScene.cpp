#include "TitleScene.h"
#include "Image.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "CommonFunction.h"
#include "SelectScene.h"

HRESULT TitleScene::Init()
{
    SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

    SceneManager::GetSingleton()->AddScene("Select", new SelectScene());

    frameY = 0;
    maxFrame = 2;

    pressButtonBlink = 0.0f;


    titleBackGround = ImageManager::GetSingleton()->FindImage("TitleBackGround");
    titleLogo = ImageManager::GetSingleton()->FindImage("TitleLogo");
    titlePressButton = ImageManager::GetSingleton()->FindImage("TitlePressButton");

    return S_OK;
}

void TitleScene::Release()
{
}

void TitleScene::Update()
{
    pressButtonBlink += TimerManager::GetSingleton()->GetElapsedTime();

    if (pressButtonBlink >= 0.5f)
    {
        frameY++;
        
        if (frameY >= maxFrame)
        {
            frameY = 0;
        }

        pressButtonBlink = 0;
    }

    PressKey();
}

void TitleScene::Render(HDC hdc)
{
    titleBackGround->Render(hdc);
	titleLogo->FrameRender(hdc, WINSIZE_X / 2, WINSIZE_Y / 2.5, 0, 0, true);
	titlePressButton->FrameRender(hdc, WINSIZE_X / 2, WINSIZE_Y / 1.35, 0, frameY, true);
}

void TitleScene::PressKey()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
    {
        SceneManager::GetSingleton()->ChangeScene("Select");
    }
}
