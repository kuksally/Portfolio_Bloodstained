#pragma once
#include "GameNode.h"

class Image;
class GameOver : public GameNode
{
private:
	Image* gameOver;
	Image* gameOverList[3];
	Image* gameOverArrow;

	FPOINT arrowPos;

	int gameOverListIndex;

	int frameY;
	int maxFrameY;

	float time;
	float maxTime;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void PressKey();

	virtual ~GameOver() {};
};