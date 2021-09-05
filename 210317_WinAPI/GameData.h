#pragma once
#include "config.h"
#include "Singleton.h"

class GameData : public Singleton<GameData>
{
private:
	int playerLives;
	int playerHp;

public:
	HRESULT Init();

	inline void SetPlayerLives(int playerLives) { this->playerLives = playerLives;}
	inline int GetPlayerLives() { return this->playerLives; }

	inline void SetPlayerHp(int playerHp) { this->playerHp = playerHp; }
	inline int GetPlayerHp() { return this->playerHp; }
};