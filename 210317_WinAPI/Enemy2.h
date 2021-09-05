#pragma once
#include "Enemy.h"

class Enemy2 : public Enemy
{
private:

public:
	HRESULT Init(int posX = 0, int posY = 0);
	void Release();
	void Update();
	void Render(HDC hdc);

	void Move();
};

