#include "BattleUI.h"
#include "Image.h"
#include "Player.h"

HRESULT BattleUI::Init(Player* player)
{
	this->player = player;

	// 플레이어 관련 UI
	playerInit = ImageManager::GetSingleton()->FindImage("PlayerInit");
	playerPicture = ImageManager::GetSingleton()->FindImage("PlayerPicture");
	playerWeapon = ImageManager::GetSingleton()->FindImage("PlayerWeapon");
	//playerScore = ImageManager::GetSingleton()->FindImage("PlayerScore");
	//playerScorePoint = ImageManager::GetSingleton()->FindImage("PlayerScorePoint");

	for (int i = 0; i </* GameData::GetSingleton()->GetPlayerLives() */player->GetPlayerLives(); i++)
	{
		livesCount += to_string(i + 1);
		playerLivesCount[i] = ImageManager::GetSingleton()->FindImage(livesCount);
		livesCount.pop_back();
	}

	for (int i = 0; i < /*GameData::GetSingleton()->GetPlayerHp()*/ player->GetPlayerHp(); i++)
	{
		playerHpCount[i] = ImageManager::GetSingleton()->FindImage("PlayerHpCount");
	}

	return S_OK;
}

void BattleUI::Release()
{
}

void BattleUI::Update()
{
}

void BattleUI::Render(HDC hdc)
{
	playerInit->Render(hdc, 5, 85);
	playerPicture->Render(hdc, 160, 50);
	playerWeapon->Render(hdc, 215, 93);
	//playerScore->Render(hdc, 849, 100);
	//playerScorePoint->Render(hdc, 950, 100);

	for (int i = 0; i < /*GameData::GetSingleton()->GetPlayerLives()*/player->GetPlayerLives(); i++)
	{
		playerLivesCount[i]->Render(hdc, 123, 84);
	}

	for (int i = 0; i < /*GameData::GetSingleton()->GetPlayerHp()*/player->GetPlayerHp(); i++)
	{
		playerHpCount[i]->Render(hdc, 29 + (i * 8), 104);
	}
}
