#include "ImageManager.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
#pragma region UI + Button
    // title_UI
    GetSingleton()->AddImage("TitleBackGround", "Image/UI/Title&Select/TitleBackGround.bmp", 1080, 720);
    GetSingleton()->AddImage("TitleLogo", "Image/UI/Title&Select/TitleLogo.bmp", 1000, 1235, 1, 4, true, RGB(0, 0, 0));
    GetSingleton()->AddImage("TitlePressButton", "Image/UI/Title&Select/PressButton.bmp", 378, 54, 1, 2);
    // select_UI
    GetSingleton()->AddImage("SelectList", "Image/UI/Title&Select/SelectList.bmp", 270, 324, 1, 6, true, RGB(0, 0, 0));
    GetSingleton()->AddImage("SelectArrow", "Image/UI/Title&Select/SelectArrow.bmp", 460, 40, true, RGB(0, 0, 0));
    // battle_PlayerUI
    GetSingleton()->AddImage("PlayerInit", "Image/UI/PlayerUI/PlayerInit.bmp", 126, 33, true, RGB(0, 0, 0));
    GetSingleton()->AddImage("PlayerLivesCount1","Image/UI/PlayerUI/LivesCount1.bmp", 32, 18, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerLivesCount2","Image/UI/PlayerUI/LivesCount2.bmp", 32, 18, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerLivesCount3","Image/UI/PlayerUI/LivesCount3.bmp", 32, 18, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerHpCount", "Image/UI/PlayerUI/HpCount.bmp", 8, 10, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerPicture", "Image/UI/PlayerUI/Picture.bmp", 53, 68, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerWeapon", "Image/UI/PlayerUI/Weapon.bmp", 24, 24);
    GetSingleton()->AddImage("PlayerScore", "Image/UI/PlayerUI/Score.bmp", 101, 18, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("PlayerScorePoint", "Image/UI/PlayerUI/ScorePoint.bmp", 128, 18, true, RGB(255, 0, 255));
    // gameOver_UI
    GetSingleton()->AddImage("GameOverBackGround", "Image/UI/GameOver/GameOver.bmp", 1080, 720);
    GetSingleton()->AddImage("GameOverList", "Image/UI/GameOver/GameOverList.bmp", 260, 432, 1, 6, true, RGB(0, 0, 0));
    GetSingleton()->AddImage("GameOverArrow", "Image/UI/GameOver/GameOverArrow.bmp", 356, 45, true, RGB(255, 0, 255));
    // UI_Button
    GetSingleton()->AddImage("SaveButton", "Image/Button/SaveButton.bmp", 100, 267, 1, 2);
    GetSingleton()->AddImage("LoadButton", "Image/Button/LoadButton.bmp", 100, 267, 1, 2);
    GetSingleton()->AddImage("StageButton", "Image/Button/StageButton3.bmp", 500, 203, 5, 2);
#pragma endregion

#pragma region Tile + BackGround
    // sampleTile
    GetSingleton()->AddImage("SampleTile", "Image/Tile/Tile.bmp", 280, 400, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
    // pixelTile
    GetSingleton()->AddImage("PixelTile", "Image/Tile/PixelCollisionTile.bmp", 280, 400, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
    // background - stage 1
    GetSingleton()->AddImage("Black", "Image/BackGround/Black.bmp", 1080, 720);
    GetSingleton()->AddImage("Moon", "Image/BackGround/Moon.bmp", 1080, 600);
    GetSingleton()->AddImage("BackGround1", "Image/BackGround/BackGround1.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("BackGround2", "Image/BackGround/BackGround2.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("BackGround3", "Image/BackGround/BackGround3.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("BackGround4", "Image/BackGround/BackGround4.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("BackGround5", "Image/BackGround/BackGround5.bmp", 2000, 600, true, RGB(255, 0, 255));
    // backgroung - stage 2
    GetSingleton()->AddImage("BackGround6", "Image/BackGround/BackGround6.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("BackGround7", "Image/BackGround/BackGround7.bmp", 2000, 600, true, RGB(255, 0, 255));
    // background - middleboss
    GetSingleton()->AddImage("MB_BackGround", "Image/BackGround/MB_BackGround.bmp", 1080, 600, true, RGB(255, 0, 255));
    // background - boss
    GetSingleton()->AddImage("B_BackGround1", "Image/BackGround/B_BackGround1.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("B_BackGround2", "Image/BackGround/B_BackGround2.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("B_BackGround3", "Image/BackGround/B_BackGround3.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("B_BackGround4", "Image/BackGround/B_BackGround4.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("B_BackGround5", "Image/BackGround/B_BackGround5.bmp", 2000, 600, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("B_BackGround6", "Image/BackGround/B_BackGround6.bmp", 1080, 600, true, RGB(255, 0, 255));
#pragma endregion

#pragma region Player : Move, Attack
    GetSingleton()->AddImage("Intro", "Image/Player/Intro.bmp", 720, 240, 6, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Idle", "Image/Player/Idle.bmp", 120, 240, 1, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Walk", "Image/Player/Walk.bmp", 480, 240, 4, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Crouch", "Image/Player/Crouch.bmp", 120, 240, 1, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Jump", "Image/Player/Jump.bmp", 240, 240, 2, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Stairs", "Image/Player/Stairs.bmp", 240, 480, 2, 4, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Attack", "Image/Player/Attack.bmp", 480, 720, 4, 6, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Dead", "Image/Player/Dead.bmp", 240, 240, 2, 2, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Victory", "Image/Player/Victory.bmp", 600, 120, 5, 1, true, RGB(255, 0, 255));
    GetSingleton()->AddImage("Aura", "Image/Player/Aura.bmp", 600, 120, 5, 1, true, RGB(255, 0, 255));
#pragma endregion

#pragma region Enemy
    // enemy1
    GetSingleton()->AddImage("Enemy1", "Image/Enemy/Enemy1.bmp", 378, 132, 7, 2, true, RGB(0, 0, 255));
    GetSingleton()->AddImage("Enemy1_Move", "Image/Enemy/Enemy1_Move.bmp", 108, 132, 2, 2, true, RGB(0, 0, 255));
    // enemy2
    GetSingleton()->AddImage("Enemy2", "Image/Enemy/Enemy2.bmp", 217 * 2, 32 * 2, 7, 2, true, RGB(0, 0, 255));
    GetSingleton()->AddImage("Enemy2_Move", "Image/Enemy/Enemy2_Move.bmp", 62 * 2, 32 * 2, 2, 2, true, RGB(0, 0, 255));
    // enemy3
    GetSingleton()->AddImage("Enemy3", "Image/Enemy/Enemy3.bmp", 80 * 2, 64 * 2, 5, 2, true, RGB(0, 0, 255));
    GetSingleton()->AddImage("Enemy3_Shoot", "Image/Enemy/Enemy3_Shoot.bmp", 80 * 2, 32 * 2, 5, 2, true, RGB(0, 0, 255));
#pragma endregion

#pragma region Boss
    // middleBoss
    GetSingleton()->AddImage("Middle_Boss", "Image/Boss/MBoss/Middle_Boss.bmp", 924 * 2.5, 74 * 2.5, 11, 1, true, RGB(0, 0, 255));
    GetSingleton()->AddImage("Middle_Boss_Attack", "Image/Boss/MBoss/Middle_Boss_Attack.bmp", 256 * 2, 39 * 2, 8, 1, true, RGB(0, 0, 255));
    GetSingleton()->AddImage("Middle_Boss_Dead", "Image/Boss/MBoss/Middle_Boss_Dead.bmp", 128, 16, 8, 1, true, RGB(0, 0, 255));
    // boss _ Upper
    GetSingleton()->AddImage("Boss_UpperBody_Idle", "Image/Boss/Boss/Boss_UpperBody_Idle.bmp", 688 * 2.7, 168 * 2.7, 4, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_UpperBody_Roar", "Image/Boss/Boss/Boss_UpperBody_Roar.bmp", 1204 * 2.7, 168 * 2.7, 7, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_UpperBody_Bazooka", "Image/Boss/Boss/Boss_UpperBody_Quick_Bazooka.bmp", 1204 * 2.7, 168 * 2.7, 7, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_UpperBody_Volcano_Hazard", "Image/Boss/Boss/Boss_UpperBody_Volcano_Hazard.bmp", 1720 * 2.7, 168 * 2.7, 10, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_UpperBody_Cannon", "Image/Boss/Boss/Boss_UpperBody_Volcano.bmp", 688 * 2.7, 168 * 2.7, 4, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_UpperBody_Dead", "Image/Boss/Boss/Boss_UpperBody_Dead.bmp", 860 * 2.7, 168 * 2.7, 5, 1, true, RGB(0, 255, 0));
    // boss _ Lower
    GetSingleton()->AddImage("Boss_LowerBody_Idle", "Image/Boss/Boss/Boss_LowerBody_Idle.bmp", 208 * 2.5, 64 * 2.7, 2, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_LowerBody_Roar", "Image/Boss/Boss/Boss_LowerBody_Roar.bmp", 416 * 2.5, 64 * 2.7, 4, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_LowerBody_Heated", "Image/Boss/Boss/Boss_LowerBody_Heated.bmp", 208 * 2.5, 64 * 2.7, 2, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_LowerBody_Dead", "Image/Boss/Boss/Boss_LowerBody_Dead.bmp", 104 * 2.5, 64 * 2.7, true, RGB(0, 255, 0));
    // boss _ Arm
    GetSingleton()->AddImage("Boss_Arm", "Image/Boss/Boss/Boss_Arm.bmp", 288 * 2.5, 40 * 2.5, 6, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm1", "Image/Boss/Boss/Arm1.bmp", 48 * 2.5, 32 * 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm2", "Image/Boss/Boss/Arm2.bmp", 48 * 2.5, 32 * 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm3", "Image/Boss/Boss/Arm3.bmp", 48 * 2.5, 32 * 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm4", "Image/Boss/Boss/Arm4.bmp", 32* 2.5, 40* 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm5", "Image/Boss/Boss/Arm5.bmp", 32* 2.5, 40* 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Arm6", "Image/Boss/Boss/Arm6.bmp", 32* 2.5, 40* 2.5, true, RGB(0, 255, 0));
    // boss _ Hand
    GetSingleton()->AddImage("Boss_Hand1", "Image/Boss/Boss/Hand_Idle.bmp", 130 * 2.7, 64 * 2.7, true, RGB(0, 255, 0));   
    GetSingleton()->AddImage("Boss_Hand2", "Image/Boss/Boss/Hand_Grab_Crazy_Slam.bmp", 130 * 2.5, 64 * 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Hand3", "Image/Boss/Boss/Hand_Raised.bmp", 112 * 2.5, 85 * 2.5, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Hand3_1", "Image/Boss/Boss/Hand_Raised_f.bmp", 31 * 2.5, 26 * 2.5, true, RGB(0, 255, 0));
    // boss _ Attack
    GetSingleton()->AddImage("Boss_CoalStack_Back", "Image/Boss/Boss/Coal_Stack_Back.bmp", 120 * 2.5, 56 * 2.5, 3, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_CoalStack_Mid", "Image/Boss/Boss/Coal_Stack_Mid.bmp", 120 * 2.5, 40 * 2.5, 3, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_CoalStack_Front", "Image/Boss/Boss/Coal_Stack_Front.bmp", 120 * 2.5, 24 * 2.5, 3, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Bazooka", "Image/Boss/Boss/Quick_Bazooka.bmp", 1476, 80, 12, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Bazooka_Mini", "Image/Boss/Boss/Quick_Bazooka_Mini.bmp", 560, 43, 10, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Hazard", "Image/Boss/Boss/Hazard.bmp", 180 * 2.5, 52 * 2.5, 6, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Boss_Cannon", "Image/Boss/Boss/Volcano_Cannon.bmp", 2865 * 3, 169 * 2.5, 7, 1, true, RGB(0, 255, 0));
    // boss + Effect
    GetSingleton()->AddImage("Heat_Burst", "Image/Boss/Boss/Heat_Burst.bmp", 160 * 2, 40 * 2, 4, 1, true, RGB(0, 255, 0));
    GetSingleton()->AddImage("Steam_Burst", "Image/Boss/Boss/Steam_Burst.bmp", 160 * 2, 40 * 2, 4, 1, true, RGB(0, 255, 0));
#pragma endregion

    return S_OK;
}

void ImageManager::Release()
{
    // 모든 이미지에 대한 메모리 해제가 이루어 진다.
    map<string, Image*>::iterator it;
    for (it = mImageDatas.begin(); it != mImageDatas.end(); it++)
    {
        if ((it->second))
        {
            (it->second)->Release();
            delete (it->second);
            (it->second) = nullptr;
        }
    }
    mImageDatas.clear();

    ReleaseSingleton();
}

Image* ImageManager::AddImage(string key, const char* fileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->Init(fileName, width, height, isTransparent, transColor)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

Image* ImageManager::AddImage(string key, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->Init(fileName, width, height,
        maxFrameX, maxFrameY, isTransparent, transColor)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

void ImageManager::DeleteImage(string key)
{
    map<string, Image*>::iterator it;
    it = mImageDatas.find(key);

    if (it == mImageDatas.end())
    {
        return;
    }

    // 맵에 key, value pair가 있는 상태
    // value에 해당하는 Image* 데이터도 메모리 해제해야 한다.
    (it->second)->Release();
    delete (it->second);
    (it->second) = nullptr;

    mImageDatas.erase(it);
}

Image* ImageManager::FindImage(string key)
{
    map<string, Image*>::iterator it;
    it = mImageDatas.find(key);

    if (it == mImageDatas.end())
    {
        return nullptr;
    }

    return it->second;
}
