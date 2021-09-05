#pragma once
#include "GameNode.h"

class Boss;
class Enemy;
class Missile;
class MissileManager : public GameNode
{
private:
	vector<Missile*> vMissiles;
	vector<Missile*>::iterator itMissiles;

	Enemy* enemyOwner;

	Boss* bossOwner;

public:
	HRESULT Init(Enemy* enemyOwner);
	HRESULT Init(Boss* bossOwner);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Fire(string imgName, float posX, float posY, float angle, int frameY);

	vector<Missile*> GetMissile() { return vMissiles; }
};

