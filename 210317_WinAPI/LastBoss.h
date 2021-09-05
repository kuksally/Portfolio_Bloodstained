#pragma once
#include "Boss.h"

class LastBoss : public Boss
{
private:
	Image* bossUpperBody;
	Image* bossLowerBody;

	Image* bossArmTop;
	Image* bossArmMid;
	Image* bossArmBottom;

	Image* bossHand;
	Image* bossHandRaised;

	Image* bossCoalStackBack;
	Image* bossCoalStackMid;
	Image* bossCoalStackFront;

	// effect image
	Image* furnaceCharging;
	Image* steamBurst[4];
	Image* heatBurst[4];

	int cannonSize;

	int upperFrame;
	int upperMaxFrame;
	int upperCount;

	int lowerFrame;
	int lowerMaxFrame;
	int lowerCount;

	int handFrame;
	int handMaxFrame;
	int handCount;

	int attackFrame;
	int attackMaxFrame;
	int attackCount;

	bool isLastAttack;

public:
	HRESULT Init(int PosX, int PosY);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Attack();

	void UpperFrameWork();
	void LowerFrameWork();
	void HandFrameWork();
	void AttackFrameWork();
};

