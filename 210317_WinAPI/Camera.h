#pragma once
#include "config.h"
#include "Singleton.h"

class Camera : public Singleton<Camera>
{
private:
	FPOINT cameraPos;
	FPOINT maxPos;
	FPOINT minPos;

public:
	void SetMapMaxPos(FPOINT maxPos) { this->maxPos = maxPos; };
	void SetMapMinPos(FPOINT minPos) { this->minPos = minPos; };

	void SetCameraPos(FPOINT cameraPos);
	FPOINT GetCameraPos();
};