#include "GameData.h"

HRESULT GameData::Init()
{
    playerLives = 3;
    playerHp = 12;

    return S_OK;
}
