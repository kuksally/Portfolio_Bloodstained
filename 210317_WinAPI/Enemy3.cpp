#include "Enemy3.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"
#include "MissileManager.h"

HRESULT Enemy3::Init(int posX, int posY)
{
	enemy = ImageManager::GetSingleton()->FindImage("Enemy3");

	pos.x = posX;
	pos.y = posY;

	size = 40;

	enemyHp = 30;
	enemyAttack = 1;

	frameX = 0;
	frameY = 1;
	maxFrame = 5;
	updateCount = 0;

	fireCount = 0;

	moveSpeed = 150.0f;

	outLookDistance = 250;

	isAlive = true;
	isLeft = true;
	isAttack = false;
	isFireBall = false;

	// 미사일 매니저
	missileMgr = new MissileManager();
	missileMgr->Init(this);

	rcEnemy = GetRectToCenter(pos.x, pos.y, size, size);

	return S_OK;
}

void Enemy3::Release()
{
	SAFE_RELEASE(missileMgr);
}

void Enemy3::Update()
{
	worldPos.x = pos.x;
	worldPos.y = pos.y;

	if (isAlive)
	{
		if (missileMgr)
		{
			if (isFireBall)
			{
				if (frameX == 3)
				{
					if (isLeft == true)
					{
						missileMgr->Fire("Enemy3_Shoot", pos.x - 25, pos.y - 15, -180, 1);
					}
					else if (isLeft == false)
					{
						missileMgr->Fire("Enemy3_Shoot", pos.x + 25, pos.y + 15, 0, 0);
					}
				}
			}


		}

		missileMgr->Update();

		// 애니메이션 프레임
		updateCount++;
		if (updateCount == 20)
		{
			frameX++;
			if (frameX >= maxFrame)
			{
				frameX = 0;
			}

			updateCount = 0;
		}

		rcEnemy = GetRectToCenter(pos.x, pos.y - 13, size, size * 1.7);
	}

	if (enemyHp == 0)
	{
		isAlive = false;
		isAttack = false;
		rcEnemy = GetRectToCenter(pos.x - 1000, pos.y - 1000, size, size);
	}

	CameraObject::UpdatePos();
}

void Enemy3::Render(HDC hdc)
{
	if (isAlive)
	{
		if (enemy)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown('R'))
			{
				Rectangle(hdc, rcEnemy.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcEnemy.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rcEnemy.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcEnemy.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
			}

			enemy->FrameRender(hdc, scPos.x, scPos.y - 12, frameX, frameY, true);
		}

		if (missileMgr)
		{
			missileMgr->Render(hdc);
		}
	}
}
