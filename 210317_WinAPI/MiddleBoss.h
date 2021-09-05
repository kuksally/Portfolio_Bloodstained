#pragma once
#include "Boss.h"

class MiddleBoss : public Boss
{
private:

public:
	HRESULT Init(int PosX, int PosY);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Attack();
};