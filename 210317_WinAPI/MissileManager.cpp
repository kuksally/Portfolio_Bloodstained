#include "MissileManager.h"
#include "Missile.h"

HRESULT MissileManager::Init(Enemy* enemyOwner)
{
    this->enemyOwner = enemyOwner;

	vMissiles.resize(1);
	vector<Missile*>::iterator it;
	for (it = vMissiles.begin(); it != vMissiles.end(); it++)
	{
		(*it) = new Missile();
		(*it)->Init(this->enemyOwner);

		// 미사일 매니저를 들고 있는 적 객체의 정보를 전달
	}

    return S_OK;
}

HRESULT MissileManager::Init(Boss* bossOwner)
{
    this->bossOwner = bossOwner;

    vMissiles.resize(3);
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it) = new Missile();
        (*it)->Init(this->bossOwner);

        // 미사일 매니저를 들고 있는 적 객체의 정보를 전달
    }

    return S_OK;
}

void MissileManager::Release()
{
    vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        (*it)->Release();
        delete (*it);
        (*it) = nullptr;
    }
    vMissiles.clear();
}

void MissileManager::Update()
{
    for (int i = 0; i < vMissiles.size(); i++)
    {
        vMissiles[i]->Update();
    }
}

void MissileManager::Render(HDC hdc)
{
    for (int i = 0; i < vMissiles.size(); i++)
    {
        vMissiles[i]->Render(hdc);
    }
}

void MissileManager::Fire(string imgName, float posX, float posY, float angle, int frameY)
{
   /* vector<Missile*>::iterator it;
    for (it = vMissiles.begin(); it != vMissiles.end(); it++)
    {
        if ((*it)->GetIsFired() == false)
        {
            (*it)->SetImgName(imgName);
            (*it)->SetIsFired(true);
            (*it)->SetAngle(DegToRad (-180));            
            break;
        }
    }*/

    for (int i = 0; i < vMissiles.size(); i++)
    {
        if (vMissiles[i]->GetIsFired() == false)
        {
            vMissiles[i]->SetIsFired(true);
            vMissiles[i]->SetPos({ posX, posY });
            vMissiles[i]->SetFrameY(frameY);
            vMissiles[i]->SetImgName(imgName);
            vMissiles[i]->SetAngle(DegToRad(angle));
            break;
        }
    }
}