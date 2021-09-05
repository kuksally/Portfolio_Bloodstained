#pragma once
#include "CameraObject.h"

class Boss;
class Enemy;
class Image;
class Missile : public CameraObject
{
private:
	Image* fire;

	FPOINT pos;

	RECT rc;

	int size;

	int damage;

	int fireIndex;
	int fireStep;

	int frameX;
	int frameY;
	int maxFrame;

	float fireCount;

	float moveSpeed;
	float moveTime;

	float angle;
	float destAngle;

	bool isFired;

	float missileTime;

	string imgName;

	Enemy* enemyOwner;

	Boss* bossOwner;

public:
	HRESULT Init(Enemy* enemyOwner);
	HRESULT Init(Boss* bossOwner);
	void Release();		
	void Update();		
	void Render(HDC hdc);

	inline void SetPos(FPOINT pos) { this->pos = pos; }
	inline FPOINT GetPos() { return this->pos; }

	void SetIsFired(bool isFired);
	inline bool GetIsFired() { return this->isFired; }

	inline void SetAngle(float angle) { this->angle = angle; }

	inline void SetFireIndex(int fireIndex) { this->fireIndex = fireIndex; }

	inline int GetSize() { return this->size; }

	inline void SetImgName(string imgName) { this->imgName = imgName; }

	inline RECT GetMissileRect() { return this->rc; }

	inline int GetMissileDamage() { return this->damage; }

	inline void SetFrameY(int frameY) { this->frameY = frameY; }
};

