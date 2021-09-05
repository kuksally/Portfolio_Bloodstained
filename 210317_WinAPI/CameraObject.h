#pragma once
#include "GameNode.h"

class CameraObject : public GameNode
{
protected:
	FPOINT scPos;
	FPOINT worldPos;

public:
	virtual void UpdatePos();
};