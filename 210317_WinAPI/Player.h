#pragma once
#include "CameraObject.h"

enum class STATE
{
	L_INTRO,
	L_IDLE,
	L_WALK,
	L_CROUCH,
	L_JUMP_UP,
	L_JUMP_DOWN,
	L_STAIRS_UP,
	L_STAIRS_DOWN,
	L_ATTACK_BASIC,
	L_ATTACK_CROUCH,
	L_ATTACK_JUMP,
	L_DEAD,
	R_INTRO,
	R_IDLE,
	R_WALK,
	R_CROUCH,
	R_JUMP_UP,
	R_JUMP_DOWN,
	R_STAIRS_UP,
	R_STAIRS_DOWN,
	R_ATTACK_BASIC,
	R_ATTACK_CROUCH,
	R_ATTACK_JUMP,
	R_DEAD,
	VICTORY,
	END
};

class Boss;
class Image;
class Player : public CameraObject
{
private:
	Image* playerImage;
	Image* pixelCollisionImage;
	Image* victoryAura;

	FPOINT pos;
	
	RECT rcPlayer;
	RECT rcAttack;

	RECT test;

	int size;
	int attackSize;

	int frameX;
	int frameY;
	int maxFrame;

	int playerLives;
	int playerHp;
	int playerAttack;

	float moveSpeed;			// 초당 이동거리
	float stairsSpeed;			// 계단 이동 속도

	float time;
	float maxTime;				// 모션별로 각각 타임 조절하기
	float collisionTime;

	float jumpPower;			// 점프하는 힘
	float startPosY;
	float prevJumpHight;
	float jumpHight;
	float jumpTime;
	float gravity;				// 중력

	bool isAlive;				// 살아있을 때
	bool isDying;				// 죽었을 때
	bool isLeft;				// 기본 = 오른쪽
	bool isAttack;				// 공격 상태
	bool isJump;				// 점프 상태
	bool isStairs;				// 계단 올라가기, 내려가기
	bool isCollision;			// 렉트 충돌
	bool isLeftDoor;			// 문이랑 충돌했을 때
	bool isRightDoor;			// 문이랑 충돌했을 때
	bool isVictory;
	bool isPixelCollsion;

	STATE state;

	Boss* boss;

	static PIXEL_TILE_INFO pixelTileInfo[TILE_X * TILE_Y];

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void OnDead();
	void Move();
	void Jump();

	void Animation();
	//void ChangeState(STATE nextState);

	void PixelCollision();

#pragma region Get, Set
	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return this->pos; }

	inline void SetPlayerLives(int playerLives) { this->playerLives = playerLives; GameData::GetSingleton()->SetPlayerLives(playerLives); }
	inline int GetPlayerLives() { return this->playerLives; }

	inline void SetPlayerHp(int playerHp) { this->playerHp = playerHp; GameData::GetSingleton()->SetPlayerHp(playerHp); }
	inline int GetPlayerHp() { return this->playerHp; }

	inline void SetIsDying(bool isDying) { this->isDying = isDying; }
	inline bool GetIsDying() { return this->isDying; }

	inline void SetPlayerRect(RECT rcPlayer) { this->rcPlayer = rcPlayer; }
	inline RECT GetPlayerRect() { return this->rcPlayer; }

	inline void SetAttackRect(RECT rcAttack) { this->rcAttack = rcAttack; }
	inline RECT GetAttackRect() { return this->rcAttack; }

	inline void SetPlayerAttack(int playerAttack) { this->playerAttack = playerAttack; }
	inline int GetPlayerAttack() { return this->playerAttack; }

	inline void SetIsAttack(bool isAttack) { this->isAttack = isAttack; }
	inline bool GetIsAttack() { return this->isAttack; }

	inline void SetPixelMap(Image* pixelCollisionImage) { this->pixelCollisionImage = pixelCollisionImage; }

	inline void SetIsCollision(bool isCollision) { this->isCollision = isCollision; }
	inline bool GetIsCollision() { return this->isCollision; }

	//inline STATE GetState() { return this->nextState; }

	inline bool GetIsRightDoor() { return this->isRightDoor; }
	inline bool GetIsLeftDoor() { return this->isLeftDoor; }

	inline void SetIsVictioy(bool isVictory) { this->isVictory = isVictory; }
#pragma endregion
};