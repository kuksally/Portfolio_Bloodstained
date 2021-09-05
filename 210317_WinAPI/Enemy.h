#pragma once
#include "CameraObject.h"

class MissileManager;
class Image;
class Enemy : public CameraObject
{
protected:
	Image* enemy;
	Image* enemyMove;
	Image* enemyShoot;

	FPOINT pos;

	RECT rcEnemy;
	RECT rcEnemyAttack;

	int size;

	int enemyHp;
	int enemyAttack;

	int frameX;
	int frameY;
	int maxFrame;
	int updateCount;

	int fireCount;

	float moveSpeed;
	float time;
	float moveTime;

	float outLookDistance;

	bool isAlive;
	bool isLeft;
	bool isMove;
	bool isAttack;
	bool isFireBall;

	MissileManager* missileMgr;

public:
	virtual HRESULT Init(int posX = 0, int posY = 0);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void Move();

	// get, set
	virtual inline void SetPos(FPOINT pos) { this->pos = pos; }
	virtual inline FPOINT GetPos() { return this->pos; }

	virtual inline int GetSize() { return this->size; }

	virtual inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	virtual inline bool GetIsAlive() { return this->isAlive; }

	virtual inline void SetEnemyRect(RECT rcEnemy) { this->rcEnemy = rcEnemy; }
	virtual inline RECT GetEnemyRect() { return this->rcEnemy; }

	virtual inline void SetEnemyHp(int enemyHP) { this->enemyHp = enemyHP; }
	virtual inline int GetEnemyHp() { return this->enemyHp; }

	virtual inline int GetEnemyAttack() { return this->enemyAttack; }

	virtual inline MissileManager* GetMissileMgr() { return this->missileMgr; }

	virtual inline float GetOutLookDistance() { return this->outLookDistance; }

	inline void SetIsLeft(bool isLeft) { this->isLeft = isLeft; }

	inline void SetIsFireBall(bool isFireBall) { this->isFireBall = isFireBall; }
	inline bool GetIsFireBall() { return this->isFireBall; }
};

