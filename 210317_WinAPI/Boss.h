#pragma once
#include "CameraObject.h"

enum class BOSSSTATE
{
	IDLE,
	ROAR,	// ����
	ATTACK,		// ���������� ���� �������
	BAZOOKA,	// ���� ���뿡�� �̻���
	HAZARD,	// ���� �ϴÿ��� �Ʒ��� �������� �̻���
	CANNON,	// ���� �ױ� �� ������ ����
	DEAD,
	END
};

class MissileManager;
class Image;
class Boss : public CameraObject
{
protected:
	Image* boss;

	FPOINT pos;

	RECT rcBoss;		// ���������� UpperBoyd RECT
	RECT rcBossAttack;
	RECT rcBossAttack2;
	RECT rcBossAttack3;
	RECT rcLowerBody;

	int size;
	int attackSize;
	int lowerBodySize;

	int bossHp;
	int bossPower;

	int frame;			// ���������� IDLE ������ �� ������
	int maxFrame;
	int updateCount;

	int fireCount;

	float moveSpeed;
	float moveTime;

	float fireDelay;

	bool isAlive;
	bool isAttack;

	BOSSSTATE bossState;

	MissileManager* missileMgr;

public:
	virtual HRESULT Init(int PosX, int PosY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void Attack();

	// get, set
	virtual inline void SetPos(FPOINT pos) { this->pos = pos; }
	virtual inline FPOINT GetPos() { return this->pos; }

	virtual inline int GetSize() { return this->size; }

	virtual inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	virtual inline bool GetIsAlive() { return this->isAlive; }

	virtual inline void SetBossRect(RECT rcBoss) { this->rcBoss = rcBoss; }
	virtual inline RECT GetBossRect() { return this->rcBoss; }
	virtual inline RECT GetLowerBodyRect() { return this->rcLowerBody; }
	virtual inline RECT GetAttackRect() { return this->rcBossAttack; }
	virtual inline RECT GetAttack2Rect() { return this->rcBossAttack2; }
	virtual inline RECT GetAttack3Rect() { return this->rcBossAttack3; }

	virtual inline void SetBossHp(int bossHp) { this->bossHp = bossHp; }
	virtual inline int GetBossHp() { return this->bossHp; }

	virtual inline int GetBossAttack() { return this->bossPower; }

	virtual inline BOSSSTATE GetBossState() { return this->bossState; }

	virtual inline MissileManager* GetMissileMgr() { return this->missileMgr; }
};