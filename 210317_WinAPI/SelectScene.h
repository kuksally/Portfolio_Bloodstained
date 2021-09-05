#pragma once
#include "GameNode.h"

class Image;
class SelectScene : public GameNode
{
private:
	Image* selectBackGround;
	Image* selectLogo;
	Image* selectList[3];
	Image* selectArrow;

	FPOINT arrowPos;

	int selectIndex;

	int frameY;
	int maxFrameY;

	float time;
	float maxTime;

	bool isPressKey;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void PressKey();

	virtual ~SelectScene() {};
};