#include "EnemyManager.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"

HRESULT EnemyManager::Init(int stage)
{
    if (stage == 2)
    {
        for (int i = 0; i < enemy1Count; i++)
        {
            vEnemys.push_back(new Enemy1());
        }

        vEnemys[0]->Init(450, 550);
        vEnemys[1]->Init(800, 350);
        vEnemys[2]->Init(1100, 550);
        vEnemys[3]->Init(1600, 550);
    }
    
    if (stage == 3)
    {
        for (int i = 0; i < enemy2Count; i++)
        {
            vEnemys.push_back(new Enemy2());
        }

        vEnemys[0]->Init(390, 523);
        vEnemys[1]->Init(490, 363);
        vEnemys[2]->Init(730, 523);
    }

    if (stage == 4)
    {
        for (int i = 0; i < enemy3Count; i++)
        {
            vEnemys.push_back(new Enemy3());
        }

        vEnemys[0]->Init(570, 518);
        vEnemys[1]->Init(1300, 238);
        vEnemys[2]->Init(1980, 359);
    }

    return S_OK;
}

void EnemyManager::Release()
{
    // 반복자(iterator) : STL 자료구조를 구성하는 원소의 메모리를 저장하는 객체
    vector<Enemy*>::iterator it;
    for (it = vEnemys.begin(); it != vEnemys.end(); it++)
    {
        (*it)->Release();   // (*it) -> Enemy* 데이터타입확인
        delete (*it);
        (*it) = nullptr;
    }
}

void EnemyManager::Update()
{
    for (int i = 0; i < vEnemys.size(); i++)
    {
        vEnemys[i]->Update(); 
    }
}

void EnemyManager::Render(HDC hdc)
{
    vector<Enemy*>::iterator it;
    for (it = vEnemys.begin(); it != vEnemys.end(); it++)
    {
        (*it)->Render(hdc);
    }
}