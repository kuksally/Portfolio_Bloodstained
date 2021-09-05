#pragma once
#include "GameNode.h"

enum class ButtonState
{
	NONE, DOWN, UP
};

typedef void (*func_t)(int);

class Image;
class Button : public GameNode
{
private:
	Image* img;

	POINT pos;

	RECT rc;
	
	int func_arg;

	ButtonState state;

	func_t func;

	POINT ptSelectedFrame;

public:
	HRESULT Init(const char* fileName, int posX, int posY);
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFunc(func_t func, int funcArg) { this->func = func; this->func_arg = funcArg; }

	inline void SetCurrStageIndex(POINT ptSelectedFrame) { this->ptSelectedFrame = ptSelectedFrame; }
	inline POINT GetCurrStageIndex() { return ptSelectedFrame; }

	virtual ~Button() {};
};

