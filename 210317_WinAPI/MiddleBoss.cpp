#include "MiddleBoss.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"
#include "MissileManager.h"

HRESULT MiddleBoss::Init(int PosX, int PosY)
{
	boss = ImageManager::GetSingleton()->FindImage("Middle_Boss");

	pos.x = PosX;
	pos.y = PosY;

	size = 200;
	attackSize = 70;

	bossHp = 100;
	bossPower = 3;

	frame = 0;
	updateCount = 0;

	isAlive = true;
	isAttack = false;
	
	bossState = BOSSSTATE::IDLE;

	missileMgr = new MissileManager();
	missileMgr->Init(this);

	return S_OK;
}

void MiddleBoss::Release()
{
	SAFE_RELEASE(missileMgr);
}

void MiddleBoss::Update()
{
	worldPos.x = pos.x;
	worldPos.y = pos.y;

	switch (bossState)
	{
	case BOSSSTATE::IDLE:
		boss = ImageManager::GetSingleton()->FindImage("Middle_Boss");
		maxFrame = 11;
		break;
	case BOSSSTATE::ATTACK:
		boss = ImageManager::GetSingleton()->FindImage("Middle_Boss_Attack");
		maxFrame = 8;
		break;
	case BOSSSTATE::DEAD:
		boss = ImageManager::GetSingleton()->FindImage("Middle_Boss_Dead");
		break;
	case BOSSSTATE::END:
		break;
	}

	if (isAlive)
	{
		if (missileMgr)
		{

			fireDelay += TimerManager::GetSingleton()->GetElapsedTime();

			if (fireDelay > 1.5)
			{
				missileMgr->Fire("Middle_Boss_Attack", pos.x, pos.y, -180, 0);
				fireDelay = 0;
			}
			
			missileMgr->Update();
		}

		updateCount ++;

		if (updateCount >= 25.0f)
		{
			frame++;
			if (frame >= maxFrame)
			{
				frame = 0;
			}

			updateCount = 0;
		}

		rcBoss = GetRectToCenter(pos.x, pos.y + 75, size, size);
	}

	if (bossHp <= 0)
	{
		isAlive = false;
		isAttack = false;
		rcBoss = GetRectToCenter(pos.x - 1000, pos.y - 1000, size, size);
	}

	Attack();

	if (isAttack == true)
	{
		rcBossAttack = GetRectToCenter(pos.x, pos.y, attackSize, attackSize);
	}
	if (isAttack == false)
	{
		rcBossAttack = GetRectToCenter(pos.x - 1000, pos.y - 1000, attackSize, attackSize);
	}

	CameraObject::UpdatePos();
}

void MiddleBoss::Render(HDC hdc)
{
	if (isAlive)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown('R'))
		{
			Rectangle(hdc, rcBoss.left, rcBoss.top, rcBoss.right, rcBoss.bottom);

			if (isAttack == true)
			{
				Rectangle(hdc, rcBossAttack.left, rcBossAttack.top, rcBossAttack.right, rcBossAttack.bottom);
			}
		}

		boss->FrameRender(hdc, scPos.x, scPos.y + 5, frame, 0, true);

		if (missileMgr)
		{
			missileMgr->Render(hdc);
		}
	}
}

void MiddleBoss::Attack()
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('W'))
	{
		isAttack = true;
		//missileMgr->Fire("Middle_Boss_Attack");
	}
	if (KeyManager::GetSingleton()->IsOnceKeyUp('W'))
	{
		isAttack = false;
	}
}
