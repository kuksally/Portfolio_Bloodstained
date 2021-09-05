#pragma once
#include "GameNode.h"

class Enemy;
class EnemyManager : public GameNode
{
private:
	vector<Enemy*> vEnemys;

	vector<FPOINT>* vPos;

	int enemy1Count;
	int enemy2Count;
	int enemy3Count;


public:
	HRESULT Init(int stage);
	void Release();
	void Update();
	void Render(HDC hdc);

	//void AddEnemy1(int size, int x, int y);
	//void AddEnemy2(int size, int x, int y);
	//void AddEnemy3(int size, int x, int y);

	vector<Enemy*> GetEnemys() { return vEnemys; }
	
	void SetPos(vector<FPOINT>* vPos) { this->vPos = vPos; }
	void SetEnemy1Count(int count) { this->enemy1Count = count; }
	void SetEnemy2Count(int count) { this->enemy2Count = count; }
	void SetEnemy3Count(int count) { this->enemy3Count = count; }
};