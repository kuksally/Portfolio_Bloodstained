#include "LastBoss.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"
#include "MissileManager.h"

HRESULT LastBoss::Init(int PosX, int PosY)
{
	/*
	0531
	보스 처치 시 플레이어 승리 동작 추가
	플레이어 hp가 0일 때 죽는 동작 추가 -> 실패
	보스 이미지 띄우기
	보스 공격 시 프레임 문제 해결하기
	충돌 체크 위해 전체적인 좌표 수정 -> 완료
	적 추가 어떻게 할 것인가 다시 생각해보기
	*/

	/*
	0601
	1. 보스 팔 이미지 다 띄우기
	2. 보스 마지막 공격 추가
	3. 보스 타일 끌어오는 이미지 추가
	0602->플레이어 키 입력 부분 수정 -> 점프 + 방향키 눌렀을 시 걷기 동작으로 바뀌는 것 수정
	0602->적 픽셀충돌 하기
	0602->플레이어 new 안 하고 정보 넘겨줄 방법 찾기
	*/

	/*
	0602
	boss 코드 수정 -> 프레임 함수 만들어서 각각 돌려주기
	플레이어 데이터 넘겨 줄 방법 -> singleton으로 게임데이터 만들어서 넘겨주기
	미사일 충돌체크
	카메라 y좌표 예외처리
	플레이어 움직임 추가 -> 앉아서 공격 추가
	적 랜덤으로 띄우기
	미사일 추가
	보스 프레임 처리
	*/

	/*
	  랜더에서 카메라 처리 한 후 전체적인 좌표 수정 (플레이어, 적, 보스 렉트)
	  플레이어 점프 공격 추가
	  적 좌표 지정해서 띄우기
	  미사일 렉트 수정
	*/

	bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Roar");
	bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Roar");

	boss = ImageManager::GetSingleton()->FindImage("Boss_Cannon");

	bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm");
	bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm");
	bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm");

	bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
	//bossHandRaised = ImageManager::GetSingleton()->FindImage("Boss_Hand3_1");

	bossCoalStackBack = ImageManager::GetSingleton()->FindImage("Boss_CoalStack_Back");
	bossCoalStackMid = ImageManager::GetSingleton()->FindImage("Boss_CoalStack_Mid");
	bossCoalStackFront = ImageManager::GetSingleton()->FindImage("Boss_CoalStack_Front");

	for (int i = 0; i < 4; i++)
	{
		heatBurst[i] = ImageManager::GetSingleton()->FindImage("Heat_Burst");
	}

	for (int i = 0; i < 4; i++)
	{
		steamBurst[i] = ImageManager::GetSingleton()->FindImage("Steam_Burst");
	}

	pos.x = PosX;
	pos.y = PosY;

	size = 200;
	attackSize = 70;
	cannonSize = 250;
	lowerBodySize = 200;
	moveSpeed = 100;

	bossHp = 150;
	bossPower = 4;

	frame = 0;
	updateCount = 0;

	upperFrame = 0;
	upperCount = 0;

	lowerFrame = 0;
	lowerCount = 0;

	handFrame = 0;
	handCount = 0;

	isAlive = true;
	isAttack = false;		// 계단 끌어오기
	isLastAttack = false;	// 죽기 전 공격

	bossState = BOSSSTATE::ROAR;

	missileMgr = new MissileManager();
	missileMgr->Init(this);

	return S_OK;
}

void LastBoss::Release()
{
	SAFE_RELEASE(missileMgr);
}

void LastBoss::Update()
{
	worldPos.x = pos.x;
	worldPos.y = pos.y;

#pragma region state
	switch (bossState)
	{
	case BOSSSTATE::IDLE:
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Idle");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Idle");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		if (bossUpperBody) upperMaxFrame = 4;
		if (bossLowerBody) lowerMaxFrame = 2;
		break;
	case BOSSSTATE::ROAR:		// 초반 소리치기
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Roar");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Roar");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		if (bossUpperBody) 	upperMaxFrame = 7;
		if (bossLowerBody) 	lowerMaxFrame = 4;
		break;
	case BOSSSTATE::ATTACK:		// 계단 끌어오기
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Idle");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Idle");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		bossHandRaised = ImageManager::GetSingleton()->FindImage("Boss_Hand3_1");
		if (bossUpperBody) 	upperMaxFrame = 4;
		if (bossLowerBody) 	lowerMaxFrame = 2;
		break;
	case BOSSSTATE::BAZOOKA:	// 몸통에서 미사일
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Bazooka");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Idle");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		if (bossUpperBody) upperMaxFrame = 7;
		if (bossLowerBody) lowerMaxFrame = 2;
		if (heatBurst) attackMaxFrame = 4;
		break;
	case BOSSSTATE::HAZARD:	// 하늘에서 미사일
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Volcano_Hazard");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Heated");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		if (bossUpperBody) upperMaxFrame = 10;
		if (bossLowerBody) lowerMaxFrame = 2;
		if (heatBurst) attackMaxFrame = 4;	// effect
		break;
	case BOSSSTATE::CANNON:		// 죽기 전 마지막 공격
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Cannon");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Heated");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand2");
		boss = ImageManager::GetSingleton()->FindImage("Boss_Cannon");		// 공격 이미지
		if (bossUpperBody) upperMaxFrame = 4;
		if (bossLowerBody) lowerMaxFrame = 2;
		if (boss) attackMaxFrame = 7;
		break;
	case BOSSSTATE::DEAD:
		bossUpperBody = ImageManager::GetSingleton()->FindImage("Boss_UpperBody_Dead");
		bossLowerBody = ImageManager::GetSingleton()->FindImage("Boss_LowerBody_Dead");
		bossArmTop = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmMid = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossArmBottom = ImageManager::GetSingleton()->FindImage("Boss_Arm3");
		bossHand = ImageManager::GetSingleton()->FindImage("Boss_Hand1");
		if (bossUpperBody) upperMaxFrame = 5;
		if (steamBurst) attackMaxFrame = 4;
		break;
	case BOSSSTATE::END:
		break;
	}
#pragma endregion

	if (isAlive)
	{
		if (bossState == BOSSSTATE::HAZARD)
		{
			if (upperFrame == 5)
			{
				int num = rand() % WINSIZE_X;

				missileMgr->Fire("Boss_Hazard", num, 100, -90, 0);
			}
		}

		if (bossState == BOSSSTATE::BAZOOKA)
		{
			if (upperFrame == 4)
			{
				//int num = rand() %  ((WINSIZE_Y - 120) + 1 - 300) + 300;

				int num[6] = { 150 ,200, 250, 300, 350, 400 };
				int num2 = rand() % 6;
				num[num2];

				missileMgr->Fire("Boss_Bazooka", WINSIZE_X, num[num2], -180, 0);
			}
		}

		missileMgr->Update();

		rcBoss = GetRectToCenter(pos.x + 410, pos.y - 100, size * 2, size);
		rcLowerBody = GetRectToCenter(pos.x + 430, pos.y + 90, lowerBodySize + 20, lowerBodySize - 20);

		if (isAttack == true)
		{
			rcBossAttack = GetRectToCenter(pos.x, pos.y, attackSize, attackSize);
		}
		else if (isAttack == false)
		{
			rcBoss = GetRectToCenter(pos.x - 1000, pos.y - 1000, size, size);
		}

		if (isLastAttack == true)
		{
			rcBossAttack = GetRectToCenter(pos.x - 100, pos.y - 100, cannonSize * 4, cannonSize + 50);
			rcBossAttack2 = GetRectToCenter(pos.x + 240, pos.y + 90, cannonSize * 1.5, cannonSize / 3);
			rcBossAttack3 = GetRectToCenter(pos.x + 280, pos.y + 140, cannonSize * 1.2, cannonSize / 3);
		}
		else if (isLastAttack == false)
		{
			rcBossAttack = GetRectToCenter(pos.x - 1000, pos.y - 1000, cannonSize, cannonSize);
			rcBossAttack2 = GetRectToCenter(pos.x - 1000, pos.y - 1000, cannonSize, cannonSize);
			rcBossAttack3 = GetRectToCenter(pos.x - 1000, pos.y - 1000, cannonSize, cannonSize);
		}

		Attack();
	}

	if (bossHp <= 0)
	{
		isAttack = false;
		isLastAttack = false;

		bossState = BOSSSTATE::DEAD;

		rcLowerBody = GetRectToCenter(pos.x - 1000, pos.y - 1000, lowerBodySize, lowerBodySize);
		rcBossAttack = GetRectToCenter(pos.x - 1000, pos.y - 1000, attackSize, attackSize);
	}

	UpperFrameWork();
	AttackFrameWork();
	LowerFrameWork();
	HandFrameWork();

	CameraObject::UpdatePos();
}

void LastBoss::Render(HDC hdc)
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('R'))
	{
		Rectangle(hdc, rcBoss.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBoss.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
			rcBoss.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBoss.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
		Rectangle(hdc, rcLowerBody.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcLowerBody.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
			rcLowerBody.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcLowerBody.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
	}

	if (isAlive)
	{
		if (bossState == BOSSSTATE::IDLE)
		{
			bossHand->Render(hdc, scPos.x - 35, scPos.y + 9);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);
		}

		//bossCoalStackBack->FrameRender(hdc, scPos.x - 560, scPos.y + 40, 0, 0);
		//bossCoalStackMid->FrameRender(hdc, scPos.x - 480, scPos.y + 80, 0, 0);
		//bossCoalStackFront->FrameRender(hdc, scPos.x - 400, scPos.y + 120, 0, 0);

		if (bossState == BOSSSTATE::ROAR)
		{
			bossHand->Render(hdc, scPos.x - 35, scPos.y + 9);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);
		}

		if (bossState == BOSSSTATE::ATTACK)
		{
			bossHand->Render(hdc, (scPos.x - 35), scPos.y + 9);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);

			bossCoalStackBack->FrameRender(hdc, scPos.x - 560, scPos.y + 39, 0, 0);
			bossCoalStackMid->FrameRender(hdc, scPos.x - 480, scPos.y + 79, 0, 0);
			bossCoalStackFront->FrameRender(hdc, scPos.x - 400, scPos.y + 119, 0, 0);
		}

		if (bossState == BOSSSTATE::HAZARD)
		{
			bossHand->Render(hdc, scPos.x - 35, scPos.y + 9);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);

			heatBurst[0]->FrameRender(hdc, scPos.x + 270, scPos.y - 160, attackFrame, 0);
			heatBurst[1]->FrameRender(hdc, scPos.x + 380, scPos.y, attackFrame, 0);
			heatBurst[2]->FrameRender(hdc, scPos.x + 430, scPos.y - 90, attackFrame, 0);
			heatBurst[3]->FrameRender(hdc, scPos.x + 470, scPos.y - 230, attackFrame, 0);
		}

		if (bossState == BOSSSTATE::BAZOOKA)
		{
			bossHand->Render(hdc, scPos.x - 35, scPos.y + 9);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);

			heatBurst[0]->FrameRender(hdc, scPos.x + 270, scPos.y - 160, attackFrame, 0);
			heatBurst[1]->FrameRender(hdc, scPos.x + 380, scPos.y, attackFrame, 0);
			heatBurst[2]->FrameRender(hdc, scPos.x + 430, scPos.y - 90, attackFrame, 0);
			heatBurst[3]->FrameRender(hdc, scPos.x + 470, scPos.y - 230, attackFrame, 0);
		}

		missileMgr->Render(hdc);

		if (bossState == BOSSSTATE::CANNON)
		{
			bossHand->Render(hdc, scPos.x - 155, scPos.y + 22);
			bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
			bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
			bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
			bossLowerBody->FrameRender(hdc, scPos.x + 280, scPos.y + 7, lowerFrame, 0);
			bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);
			boss->FrameRender(hdc, scPos.x - 660, scPos.y - 243, attackFrame, 0); // cannon image

			bossCoalStackBack->FrameRender(hdc, scPos.x - 130, scPos.y + 39, 0, 0); // 계단은 80씩 띄우기
			bossCoalStackMid->FrameRender(hdc, scPos.x - 50, scPos.y + 79, 0, 0);
			bossCoalStackFront->FrameRender(hdc, scPos.x + 30, scPos.y + 119, 0, 0);

			if (KeyManager::GetSingleton()->IsStayKeyDown('R'))
			{
				Rectangle(hdc, rcBossAttack.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rcBossAttack.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
				Rectangle(hdc, rcBossAttack2.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack2.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rcBossAttack2.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack2.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
				Rectangle(hdc, rcBossAttack3.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack3.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
					rcBossAttack3.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcBossAttack3.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
			}
		}
	}

	if (bossState == BOSSSTATE::DEAD)
	{
		bossHand->Render(hdc, scPos.x - 35, scPos.y + 9);
		bossArmBottom->Render(hdc, scPos.x + 170, scPos.y + 50);
		bossArmMid->Render(hdc, scPos.x + 180, scPos.y + 10);
		bossArmTop->Render(hdc, scPos.x + 190, scPos.y - 30);
		bossLowerBody->Render(hdc, scPos.x + 280, scPos.y + 7);
		bossUpperBody->FrameRender(hdc, scPos.x + 165, scPos.y - 270, upperFrame, 0);

		steamBurst[0]->FrameRender(hdc, scPos.x + 270, scPos.y - 160, attackFrame, 0);
		steamBurst[1]->FrameRender(hdc, scPos.x + 380, scPos.y, attackFrame, 0);
		steamBurst[2]->FrameRender(hdc, scPos.x + 430, scPos.y - 90, attackFrame, 0);
		steamBurst[3]->FrameRender(hdc, scPos.x + 470, scPos.y - 230, attackFrame, 0);
	}
}

void LastBoss::Attack()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown('F'))
	{
		isLastAttack = true;
		bossState = BOSSSTATE::CANNON;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('V'))
	{
		bossState = BOSSSTATE::ROAR;
	}

	//if (KeyManager::GetSingleton()->IsOnceKeyDown('B'))
	//{
	//	bossState = BOSSSTATE::ATTACK;
	//}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('N'))
	{
		bossState = BOSSSTATE::HAZARD;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('O'))
	{
		bossState = BOSSSTATE::BAZOOKA;
	}
}

void LastBoss::UpperFrameWork()
{
	upperCount++;

	if (upperCount >= 30.0f)
	{
		upperFrame++;

		if (upperFrame >= upperMaxFrame)
		{
			if (bossState == BOSSSTATE::ROAR)
			{
				bossState = BOSSSTATE::IDLE;
				lowerFrame = 0;
			}

			if (bossState == BOSSSTATE::HAZARD)
			{
				bossState = BOSSSTATE::IDLE;
			}


			if (bossState == BOSSSTATE::BAZOOKA)
			{
				bossState = BOSSSTATE::IDLE;
			}

			upperFrame = 0;
		}

		if (bossState == BOSSSTATE::DEAD)
		{
			upperFrame = upperMaxFrame - 1;
		}

		upperCount = 0;
	}
}

void LastBoss::LowerFrameWork()
{
	lowerCount++;

	if (lowerCount >= 30.0f)
	{
		lowerFrame++;

		if (lowerFrame >= lowerMaxFrame)
		{
			lowerFrame = 0;
		}

		lowerCount = 0;
	}
}

void LastBoss::HandFrameWork()
{
	handCount++;

	if (handCount >= 30.0f)
	{
		handFrame++;

		if (handFrame >= handMaxFrame)
		{
			handFrame = 0;
		}

		handCount = 0;
	}
}

void LastBoss::AttackFrameWork()
{
	attackCount++;

	if (attackCount >= 30.0f)
	{
		attackFrame++;

		if (attackFrame >= attackMaxFrame)
		{
			if (bossState == BOSSSTATE::CANNON)
			{
				bossState = BOSSSTATE::IDLE;
			}

			isLastAttack = false;

			attackFrame = 0;
		}

		attackCount = 0;
	}
}
