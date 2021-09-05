#include "Missile.h"
#include "Enemy.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Boss.h"
#include "Camera.h"

HRESULT Missile::Init(Enemy* enemyOwner)
{
	this->enemyOwner = enemyOwner;

	pos = {-100, -100};

	size = 30;

	damage = 1;

	fireStep = 0;

	frameX = 0;
	frameY = 1;
	maxFrame = 5;

	fireCount = 0.0f;

	moveSpeed = 500.0f;
	moveTime = 10.0f;

	angle = 0.0f;
	destAngle = 0.0f;

	isFired = false;

	missileTime = 0.0f;

    return S_OK;
}

HRESULT Missile::Init(Boss* bossOwner)
{
	this->bossOwner = bossOwner;

	pos = { -100, -100 };

	size = 60;

	damage = 3;

	fireStep = 0;

	frameX = 0;
	frameY = 0;
	maxFrame = 8;

	fireCount = 0.0f;

	moveSpeed = 300.0f;
	moveTime = 10.0f;

	angle = 0.0f;
	destAngle = 0.0f;

	isFired = false;

	return S_OK;
}

void Missile::Release()
{

}

void Missile::Update()
{
	worldPos = pos;

	fire = ImageManager::GetSingleton()->FindImage(imgName);

	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	missileTime += TimerManager::GetSingleton()->GetElapsedTime();

	// 위치 이동
	if (isFired)
	{
		if (enemyOwner)
		{
			pos.x += cosf(angle) * moveSpeed * elapsedTime;
			pos.y -= sinf(angle) * moveSpeed * elapsedTime;

			if (missileTime >= 2.5f)
			{
				isFired = false;
				missileTime = 0;
			}

			rc = GetRectToCenter(pos.x, pos.y - 15, size, size);

			fireCount += TimerManager::GetSingleton()->GetElapsedTime();

			if (fireCount >= 0.2f)
			{
				frameX++;

				if (frameX >= maxFrame)
				{
					frameX = 0;
				}

				fireCount = 0.0f;
			}
		}

		if (bossOwner)
		{
			pos.x += cosf(angle) * moveSpeed * elapsedTime;
			pos.y -= sinf(angle) * moveSpeed * elapsedTime;

			if (missileTime >= 6.0f)
			{
				isFired = false;
				missileTime = 0;
			}

			rc = GetRectToCenter(pos.x, pos.y + 60, size, size);

			fireCount += TimerManager::GetSingleton()->GetElapsedTime();

			if (fireCount >= 0.2f)
			{
				frameX++;
				if (frameX >= maxFrame)
				{
					frameX = 0;
				}

				fireCount = 0.0f;
			}
		}
	}

	CameraObject::UpdatePos();
}

void Missile::Render(HDC hdc)
{
	if (isFired)
	{
		if (enemyOwner)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown('T'))
			{
				Rectangle(hdc, rc.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rc.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rc.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rc.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
			}
		
			fire->FrameRender(hdc, scPos.x, scPos.y - 15, frameX, frameY, true);
		}

		if (bossOwner)
		{
			if (KeyManager::GetSingleton()->IsStayKeyDown('T'))
			{
				Rectangle(hdc, rc.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rc.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rc.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rc.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
			}
			fire->FrameRender(hdc, scPos.x, scPos.y + 50, frameX, frameY, true);
		}
	}
}

void Missile::SetIsFired(bool isFired)
{
	this->isFired = isFired;

	if (enemyOwner)
	{
		pos.x = enemyOwner->GetPos().x;
		pos.y = enemyOwner->GetPos().y;
	}

	if (bossOwner)
	{
		pos.x = bossOwner->GetPos().x;
		pos.y = bossOwner->GetPos().y;
	}
}