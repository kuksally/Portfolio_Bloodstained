#include "CameraObject.h"
#include "Camera.h"

void CameraObject::UpdatePos()
{
	// �÷��̾� �̿� ����
	//scPos.x = worldPos.x - Camera::GetSingleton()->GetCameraPos().x;
	//scPos.y = worldPos.y - Camera::GetSingleton()->GetCameraPos().y;

	scPos.x = worldPos.x + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x;
	scPos.y = worldPos.y + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y;
}