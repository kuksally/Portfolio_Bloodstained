#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
    bg = ImageManager::GetSingleton()->AddImage("로딩화면1", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

    return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
    {
        SceneManager::GetSingleton()->ChangeScene("전투_1");
        return;
    }
    else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
    {
        SceneManager::GetSingleton()->ChangeScene("타일맵툴", "로딩씬_1");
        return;
    }
}

void LoadingScene::Render(HDC hdc)
{
    if (bg)
    {
        bg->Render(hdc);
    }
}
