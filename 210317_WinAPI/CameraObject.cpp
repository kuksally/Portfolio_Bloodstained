#include "CameraObject.h"
#include "Camera.h"

void CameraObject::UpdatePos()
{
	// 플레이어 이외 적용
	//scPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	//scPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

	scPos.x = worldPos.x + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x;
	scPos.y = worldPos.y + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y;
}