#pragma once
#include "GameNode.h"

class Image;
class TitleScene : public GameNode
{
private:
	Image* titleBackGround;
	Image* titleLogo;
	Image* titlePressButton;

	int frameY;
	int maxFrame;

	float pressButtonBlink;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void PressKey();

	virtual ~TitleScene() {};
};