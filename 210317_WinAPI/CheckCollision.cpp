
#include "CheckCollision.h"
#include "CommonFunction.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Boss.h"
#include "Missile.h"
#include "MissileManager.h"

HRESULT CheckCollision::Init()
{
	return S_OK;
}

void CheckCollision::Release()
{
}

void CheckCollision::Update()
{
}

void CheckCollision::Render(HDC hdc)
{
}

void CheckCollision::PlayerEnemyCollisionCheck(Player* player, EnemyManager* enemyMgr)
{
		for (int i = 0; i < enemyMgr->GetEnemys().size(); i++)
		{
			// 플레이어랑 적 만났을 때
			if (RectInRect(enemyMgr->GetEnemys()[i]->GetEnemyRect(), player->GetPlayerRect()) && player->GetIsCollision() == false)
			{
				player->SetIsCollision(true);

				player->SetPlayerHp(player->GetPlayerHp() - enemyMgr->GetEnemys()[i]->GetEnemyAttack());
			}

			// 플레이어가 공격했을 때
			if (RectInRect(player->GetAttackRect(), enemyMgr->GetEnemys()[i]->GetEnemyRect()) && player->GetIsAttack() == true)
			{
				player->SetIsAttack(false);

				enemyMgr->GetEnemys()[i]->SetEnemyHp(enemyMgr->GetEnemys()[i]->GetEnemyHp() - player->GetPlayerAttack());
			}
		}

		// 미사일 시야
		for (int i = 0; i < enemyMgr->GetEnemys().size(); i++)
		{
			float deltaX = enemyMgr->GetEnemys()[i]->GetPos().x - player->GetPos().x;
			float deltaY = enemyMgr->GetEnemys()[i]->GetPos().y - player->GetPos().y;
			float length = sqrt(deltaX * deltaX + deltaY * deltaY);

			if (length < enemyMgr->GetEnemys()[i]->GetOutLookDistance())
			{
				if (player->GetPos().x < enemyMgr->GetEnemys()[i]->GetPos().x)
				{
					enemyMgr->GetEnemys()[i]->SetIsLeft(true);
				}

				if (player->GetPos().x > enemyMgr->GetEnemys()[i]->GetPos().x)
				{
					enemyMgr->GetEnemys()[i]->SetIsLeft(false);
				}

				if (enemyMgr->GetEnemys()[i]->GetIsFireBall() == false)
				{
					enemyMgr->GetEnemys()[i]->SetIsFireBall(true);
				}
			}
		}
}

void CheckCollision::PlayerBossCollisionCheck(Player* player, Boss* boss)
{
		// 플레이어랑 보스 만났을 때
		if (RectInRect(player->GetPlayerRect(), boss->GetBossRect()) && player->GetIsCollision() == false
			|| RectInRect(player->GetPlayerRect(), boss->GetLowerBodyRect()) && player->GetIsCollision() == false
			|| RectInRect(player->GetPlayerRect(), boss->GetAttackRect()) && player->GetIsCollision() == false
			|| RectInRect(player->GetPlayerRect(), boss->GetAttack2Rect()) && player->GetIsCollision() == false
			|| RectInRect(player->GetPlayerRect(), boss->GetAttack3Rect()) && player->GetIsCollision() == false)
		{
			player->SetIsCollision(true);

			player->SetPlayerHp(player->GetPlayerHp() - boss->GetBossAttack());
		}

		// 플레이어가 공격했을 때
		if (RectInRect(player->GetAttackRect(), boss->GetBossRect()) && player->GetIsAttack() == true
			|| RectInRect(player->GetAttackRect(), boss->GetLowerBodyRect()) && player->GetIsAttack() == true)
		{
			player->SetIsAttack(false);

			boss->SetBossHp(boss->GetBossHp() - player->GetPlayerAttack());
		}
}

void CheckCollision::Collision(Player* player, EnemyManager* enemyMgr)
{
	RECT playerRect = player->GetPlayerRect();
	
	vector<Enemy*> vEnemys = enemyMgr->GetEnemys();

	for (int i = 0; i < vEnemys.size(); i++)
	{
		Enemy* enemy = vEnemys[i];

		//RECT enemyRect = enemy->GetEnemyRect();
		if (enemy->GetMissileMgr() == nullptr)
			continue;
		MissileManager* missileMgr = enemy->GetMissileMgr();

		vector<Missile*> vMissiles = missileMgr->GetMissile();

		for (int j = 0; j < vMissiles.size(); j++)
		{
			Missile* missile = vMissiles[j];

			RECT missileRect = missile->GetMissileRect();

			if (RectInRect(playerRect, missileRect) && player->GetIsCollision() == false)
			{
				player->SetIsCollision(true);

				player->SetPlayerHp(player->GetPlayerHp() - missile->GetMissileDamage());
			}
		}
	}
}

void CheckCollision::Collision2(Player* player, Boss* boss)
{
	RECT playerRect = player->GetPlayerRect();

	MissileManager* missileMgr = boss->GetMissileMgr();

	vector<Missile*> vMissiles = missileMgr->GetMissile();

	for (int i = 0; i < vMissiles.size(); i++)
	{
		Missile* missile = vMissiles[i];

		RECT missileRect = missile->GetMissileRect();

		if (RectInRect(playerRect, missileRect) && player->GetIsCollision() == false)
		{
			player->SetIsCollision(true);

			player->SetPlayerHp(player->GetPlayerHp() - missile->GetMissileDamage());
		}
	}
}
