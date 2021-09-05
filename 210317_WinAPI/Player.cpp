#include "Player.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Camera.h"
#include "GameOver.h"
#include "Boss.h"

PIXEL_TILE_INFO Player::pixelTileInfo[TILE_X * TILE_Y];

HRESULT Player::Init()
{
	SceneManager::GetSingleton()->AddScene("GameOver", new GameOver());

	pixelCollisionImage = ImageManager::GetSingleton()->FindImage("PixelTile");

	// 수정 좌표
	pos.x = 50;
	pos.y = 400;

	size = 39;
	attackSize = 30;

	frameX = 0;
	frameY = 1;
	maxFrame = 6;

	playerLives = GameData::GetSingleton()->GetPlayerLives();
	playerHp = GameData::GetSingleton()->GetPlayerHp();
	playerAttack = 10;

	moveSpeed = 150.0f;
	stairsSpeed = 40.0f;	// 계단 이동 속도

	time = 0.0f;
	maxTime = 20.0f;
	collisionTime = 0.0f;

	jumpPower = 72.0f;
	startPosY = 0.0f;
	jumpHight = 0.0f;
	prevJumpHight = 0.0f;
	jumpTime = 0.0f;
	gravity = 9.81f;

	isAlive = true;
	isDying = false;
	isLeft = false;		// 방향
	isAttack = false;
	isJump = false;
	isStairs = false;
	isCollision = false;
	isLeftDoor = false;
	isRightDoor = false;
	isVictory = false;
	isPixelCollsion = false;

	state = STATE::R_INTRO;

	return S_OK;
}

void Player::Release()
{
}

void Player::Update()
{
	worldPos.x = pos.x;
	worldPos.y = pos.y;

	if (isCollision == true)
	{
		collisionTime += TimerManager::GetSingleton()->GetElapsedTime();

		if (collisionTime >= 3.0f)
		{
			isCollision = false;
			collisionTime = 0.0f;
		}
	}

	//if (KeyManager::GetSingleton()->IsOnceKeyDown('M'))
	//{
	//	playerHp = playerHp - 1;
	//}
	if (playerHp <= 0)
	{
		playerLives = playerLives - 1;

		if (playerLives != 0)
		{
			playerHp = 12;
		}
	}

	//if (playerHp == 0 && playerLives != 0)
	//{
	//	if (isLeft == true)
	//	{
	//		state = STATE::L_DEAD;
	//	}
	//	else
	//	{
	//		state = STATE::R_DEAD;
	//	}
	//}

	if (playerLives == 0)
	{
		isAlive = false;
		isDying = true;
	}

	if (isAlive && KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
	{
		isAlive = false;
		isDying = true;
	}

	if (isDying)
	{
		//OnDead();
	}
	else
	{
		Move();
		Animation();
	}

	if (pos.x < 0)
	{
		pos.x = 0;
	}

	if (isVictory == true)
	{
		state = STATE::VICTORY;
		pos.x = WINSIZE_X / 2;
	}

	time += TimerManager::GetSingleton()->GetElapsedTime();

	if (time >= maxTime)
	{
		frameX++;
		if (frameX >= maxFrame)
		{
			if (state == STATE::R_INTRO)
			{
				state = STATE::R_IDLE;
			}

			if (state == STATE::R_ATTACK_BASIC)
			{
				state = STATE::R_IDLE;
			}
			else if (state == STATE::L_ATTACK_BASIC)
			{
				state = STATE::L_IDLE;
			}

			if (state == STATE::R_ATTACK_CROUCH)
			{
				state = STATE::R_CROUCH;
			}
			else if (state == STATE::L_ATTACK_CROUCH)
			{
				state = STATE::L_CROUCH;
			}

			if (state == STATE::R_ATTACK_JUMP)
			{
				state = STATE::R_JUMP_UP;
			}
			else if (state == STATE::L_ATTACK_JUMP)
			{
				state = STATE::L_JUMP_UP;
			}

			if (state == STATE::L_DEAD && playerLives == 0 && state == STATE::R_DEAD && playerLives == 0)
			{
				SceneManager::GetSingleton()->ChangeScene("GameOver");
			}

			isAttack = false;

			frameX = 0;
		}

		time = 0.0f;
	}

	rcPlayer = GetRectToCenter(pos.x, pos.y + 20, size, size * 2);

	if (state == STATE::L_CROUCH || state == STATE::R_CROUCH || state == STATE::L_ATTACK_CROUCH || state == STATE::R_ATTACK_CROUCH)
	{
		rcPlayer = GetRectToCenter(pos.x, pos.y + 20, size, size);
	}

	if (frameX == 3 && isAttack == true && isLeft == false)
	{
		if (state == STATE::R_ATTACK_BASIC)
		{
			rcAttack = GetRectToCenter(pos.x + 55, pos.y + 10, attackSize, attackSize);
		}
		else if (state == STATE::R_ATTACK_CROUCH)
		{
			rcAttack = GetRectToCenter(pos.x + 55, pos.y + 25, attackSize, attackSize);
		}
		else if (state == STATE::R_ATTACK_JUMP)
		{
			rcAttack = GetRectToCenter(pos.x + 55, pos.y + 5, attackSize, attackSize);
		}
	}
	else if (frameX == 3 && isAttack == true && isLeft == true)
	{
		if (state == STATE::L_ATTACK_BASIC)
		{
			rcAttack = GetRectToCenter(pos.x - 50, pos.y + 5, attackSize, attackSize);
		}
		else if (state == STATE::L_ATTACK_CROUCH)
		{
			rcAttack = GetRectToCenter(pos.x - 50, pos.y + 25, attackSize, attackSize);
		}
		else if (state == STATE::L_ATTACK_JUMP)
		{
			rcAttack = GetRectToCenter(pos.x - 50, pos.y + 10, attackSize, attackSize);
		}
	}
	else
	{
		rcAttack = GetRectToCenter(pos.x - 100000, pos.y + 100000, attackSize, attackSize);
	}

	PixelCollision();

	Camera::GetSingleton()->SetCameraPos(pos);	// 기준점 찾는 것

	CameraObject::UpdatePos();
}

void Player::Render(HDC hdc)
{
	if (KeyManager::GetSingleton()->IsStayKeyDown('E'))
	{
		Rectangle(hdc, rcPlayer.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcPlayer.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
			rcPlayer.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcPlayer.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);

		if (frameX == 3 && isAttack == true && isLeft == false)
		{
			Rectangle(hdc, rcAttack.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcAttack.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
				rcAttack.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcAttack.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
		}
		else if (frameX == 3 && isAttack == true && isLeft == true)
		{
			Rectangle(hdc, rcAttack.left - Camera::GetSingleton()->GetCameraPos().x, rcAttack.top - Camera::GetSingleton()->GetCameraPos().y,
				rcAttack.right - Camera::GetSingleton()->GetCameraPos().x, rcAttack.bottom - Camera::GetSingleton()->GetCameraPos().y);
		}
	}

	if (playerImage)
	{
		playerImage->FrameRender(hdc, scPos.x, scPos.y, frameX, frameY, true);
	}

	//for (int i = 0; i < TILE_X * TILE_Y; i++)
	//{
	//	pixelCollisionImage->FrameRender(hdc,
	//		pixelTileInfo[i].rcTile.left - Camera::GetSingleton()->GetCameraPos().x,
	//		pixelTileInfo[i].rcTile.top,
	//		pixelTileInfo[i].frameX,
	//		pixelTileInfo[i].frameY);
	//}

	//Rectangle(hdc, test.left, test.top, test.right, test.bottom);
}

void Player::OnDead()
{
}

void Player::Move()
{
	// 플레이어 낙하
	if (/*pos.y < WINSIZE_Y  &&*/ isJump == false)
	{
		pos.y += gravity * TimerManager::GetSingleton()->GetElapsedTime() * 8;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LEFT))
	{
		isLeft = true;
		state = STATE::L_WALK;

	}
	else if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LEFT))
	{
		state = STATE::L_IDLE;
	}

	if (state == STATE::L_WALK || state == STATE::L_STAIRS_UP || state == STATE::L_STAIRS_DOWN)
	{
		pos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}

	if (isLeft == true && state == STATE::L_WALK)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
		{
			isStairs = true;
			state = STATE::L_STAIRS_UP;
		}
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_UP))
		{
			isStairs = false;
		}
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
		{
			isStairs = true;
			state = STATE::L_STAIRS_DOWN;
		}
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_DOWN))
		{
			isStairs = false;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RIGHT))
	{
		isLeft = false;
		state = STATE::R_WALK;
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_RIGHT))
	{
		state = STATE::R_IDLE;
	}

	if (state == STATE::R_WALK || state == STATE::R_STAIRS_UP || state == STATE::R_STAIRS_DOWN)
	{
		pos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
	}

	if (isLeft == false && state == STATE::R_WALK)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
		{
			isStairs = true;
			state = STATE::R_STAIRS_UP;
		}
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_UP))
		{
			isStairs = false;
		}
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
		{
			isStairs = true;
			state = STATE::R_STAIRS_DOWN;
		}
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_DOWN))
		{
			isStairs = false;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
	{
		if (isLeft == true)
		{
			state = STATE::L_CROUCH;
		}
		else
		{
			state = STATE::R_CROUCH;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_DOWN))
	{
		if (isLeft == true)
		{
			state = STATE::L_IDLE;
		}
		else
		{
			state = STATE::R_IDLE;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Z'))
	{
		isAttack = true;

		if (isLeft == true)
		{
			state = STATE::L_ATTACK_BASIC;
		}
		else
		{
			state = STATE::R_ATTACK_BASIC;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('X'))
	{
		isAttack = true;

		if (isLeft == true)
		{
			state = STATE::L_ATTACK_CROUCH;
		}
		else
		{
			state = STATE::R_ATTACK_CROUCH;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown('C'))
	{
		isAttack = true;

		if (isLeft == true)
		{
			state = STATE::L_ATTACK_JUMP;
		}
		else if (isLeft == false)
		{
			state = STATE::R_ATTACK_JUMP;
		}
	}

	if (isJump == false)
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
		{
			isJump = true;

			if (isLeft == true)
			{
				state = STATE::L_JUMP_UP;
			}
			else if (isLeft == false)
			{
				state = STATE::R_JUMP_UP;
			}

			startPosY = pos.y; // 처음 점프 위치 저장
		}
	}
	Jump();
}

void Player::Jump()
{
	if (isJump == true)
	{
		//점프 포물선 공식
		jumpHight = jumpPower * (sinf(PI / 2.0f) * jumpTime) - (gravity * jumpTime * jumpTime);

		pos.y = startPosY - jumpHight;

		jumpTime += TimerManager::GetSingleton()->GetElapsedTime() * 8;

		if (jumpHight < prevJumpHight)
		{
			if (isLeft == true)
			{
				state = STATE::L_JUMP_DOWN;
			}
			else if (isLeft == false)
			{
				state = STATE::R_JUMP_DOWN;
			}

			//if (jumpHight < 0)
			//{
			//	isJump = false;

			//	jumpTime = 0;

			//	if (isLeft == true)
			//	{
			//		state = STATE::L_IDLE;
			//	}
			//	else
			//	{
			//		state = STATE::R_IDLE;
			//	}
			//}

			//pos.y = startPosY;
		}

		prevJumpHight = jumpHight;
	}
}

void Player::Animation()
{
	switch (state)
	{
	case STATE::L_INTRO:
		playerImage = ImageManager::GetSingleton()->FindImage("Intro");
		frameY = 0;
		maxFrame = 6;
		maxTime = 0.4f;
		break;
	case STATE::L_IDLE:
		playerImage = ImageManager::GetSingleton()->FindImage("Idle");
		frameY = 0;
		maxFrame = 1;
		maxTime = 0.0f;
		break;
	case STATE::L_WALK:
		playerImage = ImageManager::GetSingleton()->FindImage("Walk");
		frameY = 0;
		maxFrame = 4;
		maxTime = 0.4f;
		break;
	case STATE::L_CROUCH:
		playerImage = ImageManager::GetSingleton()->FindImage("Crouch");
		frameY = 0;
		maxFrame = 1;
		maxTime = 0.0f;
		break;
	case STATE::L_JUMP_UP:
		playerImage = ImageManager::GetSingleton()->FindImage("Jump");
		frameY = 0;
		maxFrame = 2;
		maxTime = 0.4f;
		break;
	case STATE::L_STAIRS_UP:
		playerImage = ImageManager::GetSingleton()->FindImage("Stairs");
		frameY = 0;
		maxFrame = 2;
		maxTime = 0.4f;
		break;
	case STATE::L_STAIRS_DOWN:
		playerImage = ImageManager::GetSingleton()->FindImage("Stairs");
		frameY = 2;
		maxFrame = 2;
		maxTime = 0.4f;
		break;
	case STATE::L_ATTACK_BASIC:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 0;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::L_ATTACK_CROUCH:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 2;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::L_ATTACK_JUMP:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 4;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::L_DEAD:
		playerImage = ImageManager::GetSingleton()->FindImage("Dead");
		frameY = 0;
		maxFrame = 2;
		maxTime = 0.3f;
	case STATE::R_INTRO:
		playerImage = ImageManager::GetSingleton()->FindImage("Intro");
		frameY = 1;
		maxFrame = 6;
		maxTime = 0.4f;
		break;
	case STATE::R_IDLE:
		playerImage = ImageManager::GetSingleton()->FindImage("Idle");
		frameY = 1;
		maxFrame = 1;
		maxTime = 0.0f;
		break;
	case STATE::R_WALK:
		playerImage = ImageManager::GetSingleton()->FindImage("Walk");
		frameY = 1;
		maxFrame = 4;
		maxTime = 0.4f;
		break;
	case STATE::R_CROUCH:
		playerImage = ImageManager::GetSingleton()->FindImage("Crouch");
		frameY = 1;
		maxFrame = 1;
		maxTime = 0.0f;
		break;
	case STATE::R_JUMP_UP:
		playerImage = ImageManager::GetSingleton()->FindImage("Jump");
		frameY = 1;
		maxFrame = 2;
		maxTime = 0.4;
		break;
	case STATE::R_STAIRS_UP:
		playerImage = ImageManager::GetSingleton()->FindImage("Stairs");
		frameY = 1;
		maxFrame = 2;
		maxTime = 0.4f;
		break;
	case STATE::R_STAIRS_DOWN:
		playerImage = ImageManager::GetSingleton()->FindImage("Stairs");
		frameY = 3;
		maxFrame = 2;
		maxTime = 0.4f;
		break;
	case STATE::R_ATTACK_BASIC:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 1;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::R_ATTACK_CROUCH:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 3;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::R_ATTACK_JUMP:
		playerImage = ImageManager::GetSingleton()->FindImage("Attack");
		frameY = 5;
		maxFrame = 4;
		maxTime = 0.1f;
		break;
	case STATE::R_DEAD:
		playerImage = ImageManager::GetSingleton()->FindImage("Dead");
		frameY = 1;
		maxFrame = 2;
		maxTime = 0.3f;
	case STATE::VICTORY:
		playerImage = ImageManager::GetSingleton()->FindImage("Victory");
		victoryAura = ImageManager::GetSingleton()->FindImage("Aura");
		frameY = 0;
		maxFrame = 5;
		maxTime = 0.3f;
	case STATE::END:
		break;
	}
}

void Player::PixelCollision()
{
	if (playerImage)
	{
		// 픽셀 충돌 확인
		COLORREF color;
		int R, G, B;
		float currPosY = pos.y + playerImage->GetFrameHeight();
		float currPosX = pos.x;

		// Y축
		for (int i = currPosY - 10; i < currPosY + 10; i++)
		{
			color = GetPixel(pixelCollisionImage->GetMemDC(), pos.x, i);

			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);

			test.left = pos.x;
			test.top = i;
			test.right = pos.x + 10;
			test.bottom = i + 10;

			// 바닥, 벽 타일
			if (R == 255 && G == 0 && B == 0)  // 떨어지지 않게 처리
			{
				// 위치 고정
				if (isJump == true)
				{
					int a = 0;
				}

				pos.y = i - playerImage->GetFrameHeight();

				if (state == STATE::L_JUMP_DOWN || state == STATE::R_JUMP_DOWN)
				{
					isJump = false;
					jumpTime = 0;

					if (isLeft == true)
					{
						state = STATE::L_IDLE;
					}
					else if (isLeft == false)
					{
						state = STATE::R_IDLE;
					}
				}

				break;
			}

			// 계단
			if (R == 0 && G == 0 && B == 255)
			{
				isPixelCollsion = true;

				if (isStairs == true)
				{
					pos.y = i - playerImage->GetFrameHeight();
				}

				break;
			}
		}

		// X축
		for (int i = currPosX - 10; i < currPosX + 10; i++)
		{
			color = GetPixel(pixelCollisionImage->GetMemDC(), i, pos.y + 100);

			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);

			if (R == 0 && G == 0 && B == 128)
			{
				if (isLeft == false)
				{
					pos.x = i - playerImage->GetFrameWidth() / 4;
				}
				else if (isLeft == true)
				{
					pos.x = i + playerImage->GetFrameWidth() / 4;
				}

				break;
			}

			if (R == 0 && G == 255 && B == 0)
			{
				// 문이랑 픽셀 충돌 했을 때 (다음 스테이지)
				isRightDoor = true;
				isLeftDoor = false;
				break;
			}

			else if (R == 255 && G == 255 && B == 0)
			{
				// 문이랑 픽셀 충돌 했을 때 (전 스테이지)
				isLeftDoor = true;
				isRightDoor = false;
				break;
			}
		}
	}
}