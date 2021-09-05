#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
    bg = ImageManager::GetSingleton()->AddImage("�ε�ȭ��1", "Image/loading.bmp", WINSIZE_X, WINSIZE_Y);

    return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
    {
        SceneManager::GetSingleton()->ChangeScene("����_1");
        return;
    }
    else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
    {
        SceneManager::GetSingleton()->ChangeScene("Ÿ�ϸ���", "�ε���_1");
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
