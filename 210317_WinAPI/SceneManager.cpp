#include "SceneManager.h"
#include "GameNode.h"

GameNode* SceneManager::currentScene = nullptr;
GameNode* SceneManager::loadingScene = nullptr;
GameNode* SceneManager::readyScene = nullptr;

DWORD CALLBACK LoadingThread(LPVOID pvParam)
{
    if (SUCCEEDED(SceneManager::readyScene->Init()))
    {
        SceneManager::currentScene = SceneManager::readyScene;

        SceneManager::loadingScene->Release();
        SceneManager::loadingScene = nullptr;
        SceneManager::readyScene = nullptr; // readyScene�� ��Ƴ��� �޸� �ּҸ� nullptr���ش�
    }

    return 0;
}

HRESULT SceneManager::Init()
{
    return S_OK;
}

void SceneManager::Release()
{
    map<string, GameNode*>::iterator it;
    for (it = mSceneDatas.begin(); it != mSceneDatas.end(); it++)
    {
        if (it->second)
        {
            SAFE_RELEASE(it->second);
        }
    }
    mSceneDatas.clear();

    ReleaseSingleton();
}

void SceneManager::Update()
{
    if (currentScene)
    {
        currentScene->Update();
    }
}

void SceneManager::Render(HDC hdc)
{
    if (currentScene)
    {
        currentScene->Render(hdc);
    }
}

GameNode* SceneManager::AddScene(string key, GameNode* scene)
{
    if (scene == nullptr)
    {
        return nullptr;
    }

    map<string, GameNode*>::iterator it = mSceneDatas.find(key);
    if (it != mSceneDatas.end())
    {
        return it->second;
    }

    mSceneDatas.insert(pair<string, GameNode*>(key, scene));

    return scene;
}

GameNode* SceneManager::AddLoadingScene(string key, GameNode* scene)
{
    if (scene == nullptr)
    {
        return nullptr;
    }

    map<string, GameNode*>::iterator it = mLoadingSceneDatas.find(key);
    if (it != mLoadingSceneDatas.end())
    {
        return it->second;
    }

    mLoadingSceneDatas.insert(pair<string, GameNode*>(key, scene));

    return scene;
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
    //try
    //{

    //}
    //catch (const std::exception&)
    //{

    //}

    map<string, GameNode*>::iterator it = mSceneDatas.find(sceneName);
    if (it == mSceneDatas.end())
    {
        return E_FAIL;
    }

    if (it->second == currentScene)
    {
        return S_OK;
    }

    if (SUCCEEDED(it->second->Init()))
    {
        // ���� �� -> Ÿ��Ʋ ��
        // �ٲٰ� ���� �� -> ��Ʋ ��
        if (currentScene)
        {
            currentScene->Release();
        }
        currentScene = it->second;

        return S_OK;
    }

    return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string LoadingSceneName)
{
    map<string, GameNode*>::iterator it = mSceneDatas.find(sceneName);
    if (it == mSceneDatas.end())
    {
        return E_FAIL;
    }

    if (it->second == currentScene)
    {
        return S_OK;
    }

    // �ε��� ã��
    map<string, GameNode*>::iterator itLoading = mLoadingSceneDatas.find(LoadingSceneName);

    if (itLoading == mLoadingSceneDatas.end())  // �ε��� �� ã���� ��
    {
        return ChangeScene(sceneName);
    }

    if (SUCCEEDED(itLoading->second->Init()))  // ã���� ��
    {
        // ���� �� -> Ÿ��Ʋ ��
        // �ٲٰ� ���� �� -> ��Ʋ ��
        if (currentScene)
        {
            currentScene->Release();
        }
        currentScene = itLoading->second;

        // ����
        readyScene = it->second;
        loadingScene = itLoading->second;

        // ���� ���� �ʱ�ȭ�� �����带 ����
        DWORD loadingThreadId;
        HANDLE hThread;
        // ���� ���� ->  null, ���� ������ 0 = �ִ�ġ, ���� ��ų �Լ�������, �Լ��� �� �Ű����� ���� -> ������ null, 0, id)
        hThread = CreateThread(NULL, 0,
            LoadingThread,       // ���� ��ų �Լ� ������
            NULL,               // ���� ��ų �Լ��� �� �Ű�����
            0,
            &loadingThreadId
        );  

        CloseHandle(hThread); // create������ close����

        return S_OK;
    }

    return E_FAIL;
}
