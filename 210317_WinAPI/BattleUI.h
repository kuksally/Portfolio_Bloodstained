#pragma once
#include "GameNode.h"

class Player;
class Image;
class BattleUI : public GameNode
{
private:
	Image* playerInit;
	Image* playerPicture;
	Image* playerWeapon;
	Image* playerScore;
	Image* playerScorePoint;
	Image* playerLivesCount[3];
	Image* playerHpCount[12];

	string livesCount = "PlayerLivesCount";

	Player* player;

public:
	virtual HRESULT Init(Player* player);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~BattleUI() {};
};