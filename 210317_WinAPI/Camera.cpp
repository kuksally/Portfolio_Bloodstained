#include "Camera.h"

void Camera::SetCameraPos(FPOINT cameraPos)
{
	// 카메라 예외처리
	if (cameraPos.x < minPos.x) { cameraPos.x = minPos.x; }
	if (cameraPos.x > maxPos.x) { cameraPos.x = maxPos.x; }

	if (cameraPos.y > maxPos.y) { cameraPos.y = maxPos.y; }
	if (cameraPos.y < minPos.y) { cameraPos.y = minPos.y; }

	this->cameraPos.x = cameraPos.x;
	this->cameraPos.y = cameraPos.y;
}

FPOINT Camera::GetCameraPos()
{
	return this->cameraPos;
}
