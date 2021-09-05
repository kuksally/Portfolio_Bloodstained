#include "Enemy1.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Camera.h"

HRESULT Enemy1::Init(int posX, int posY)
{
    enemy = ImageManager::GetSingleton()->FindImage("Enemy1");

    pos.x = posX;
    pos.y = posY;

    size = 33;

    enemyHp = 10;
    enemyAttack = 1;

    frameX = 0;
    frameY = 1;
    maxFrame = 7;
    updateCount = 0;

    moveSpeed = 150.0f;

    moveTime = 0;

    isAlive = true;
    isLeft = true;
    isMove = false;

    rcEnemy = GetRectToCenter(pos.x, pos.y, size, size);

    return S_OK;
}

void Enemy1::Release()
{
}

void Enemy1::Update()
{
    worldPos.x = pos.x;
    worldPos.y = pos.y;
   
    if (isAlive)
    {
        // 애니메이션 프레임
        updateCount++;
        if (updateCount == 30)
        {
            frameX++;
            if (frameX >= maxFrame)
            {
                frameX = 5;
                
            }

            updateCount = 0;
        }

        rcEnemy = GetRectToCenter(pos.x + 5, pos.y - 42, size, size * 2);
    }

    if (enemyHp == 0)
    {
        isAlive = false;
        rcEnemy = GetRectToCenter(pos.x - 1000, pos.y -1000, size, size);
    }

    if (frameX >= 5)
    {
        Move();
    }
   

    CameraObject::UpdatePos();
}

void Enemy1::Render(HDC hdc)
{
    if (isAlive)
    {
        if (enemy)
        {
            if (KeyManager::GetSingleton()->IsStayKeyDown('R'))
            {
                Rectangle(hdc, rcEnemy.left + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcEnemy.top + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y,
               rcEnemy.right + (WINSIZE_X / 2) - Camera::GetSingleton()->GetCameraPos().x, rcEnemy.bottom + (WINSIZE_Y / 2) - Camera::GetSingleton()->GetCameraPos().y);
            }

            enemy->FrameRender(hdc, scPos.x, scPos.y - 42, frameX, frameY, true);
        }
    }
}

void Enemy1::Move()
{
    time = TimerManager::GetSingleton()->GetElapsedTime();

    moveTime += TimerManager::GetSingleton()->GetElapsedTime();


    if (isLeft)
    {
        pos.x -= moveSpeed * time;
    }

    else if (!isLeft)
    {
        pos.x += moveSpeed * time;
    }

    if (moveTime > 1.5f)
    {
        if (isLeft)
        {
            isLeft = false;
            moveTime = 0;
            frameY = 0;
        }
        else if (!isLeft)
        {
            isLeft = true;
            moveTime = 0;
            frameY = 1;
        }
    }
}