#pragma once
#include "GameNode.h"

class MissileManager;
class Boss;
class EnemyManager;
class Player;
class CheckCollision : public GameNode
{
private:
	//bool beShot;
	bool isCollision;

	Player* player;

	EnemyManager* enemyMgr;

	Boss* boss;

	//MissileManager* missileMgr;

public:
	virtual HRESULT Init();
	virtual void Release();			
	virtual void Update();			
	virtual void Render(HDC hdc);	

	void PlayerEnemyCollisionCheck(Player* player, EnemyManager* enemyMgr);
	void PlayerBossCollisionCheck(Player* player, Boss* boss);
	//void PlayerMissileCollisionCheck(Player* player, MissileManager* missileMgr);
	
	void Collision(Player* player, EnemyManager* enemyMgr);
	void Collision2(Player* player, Boss* boss);

	virtual ~CheckCollision() {};
};