#pragma once
#include "GameNode.h"

#define ASTAR_TILE_SIZE		60
#define ASTAR_TILE_COUNT	(ASTARSIZE_Y / ASTAR_TILE_SIZE)

enum class AstarTileType { Start, End, Wall, None };

class AstarTile : public GameNode
{
private:
	int idX, idY;	// tile의 x, y index값을 저장할 변수
	POINT center;	// point로 center의 값을 저장(좌표값)
	RECT rc;		// 직접 클릭해서 start지점이나 장애물을 세팅할 수 있도록하기 위해
	AstarTileType type;

	int costFromStart;	// g : 시작점부터 현재 노드까지의 비용
	int costToGoal;		// h : 현재 노드부터 목적지까지의 예상비용
	int totalCost;		// f : g + h

	// 포인터로 이전 노드 저장
	AstarTile* parentTile;	// g가 갱신될 때마다 이전 노드를 갱신

	bool isInOpenlist;	// 현재 오픈리스트에 있는지
	bool isClosed;		// 더이상 체크 할 필요가 없는 애인지

	// 클릭하면 색 변화
	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

	char szText[128];

	int heapIndex;

public:
	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetColor(COLORREF color);
	void SetType(AstarTileType type) { this->type = type; }
	AstarTileType GetType() { return this->type; }
	int GetIdX() { return this->idX; }
	int GetIdY() { return this->idY; }

	void SetParentTile(AstarTile* parent) { this->parentTile = parent; }
	AstarTile* GetParentTile() { return this->parentTile; }

	void SetG(int g) { this->costFromStart = g; }
	int GetG() { return this->costFromStart; }

	void SetH(int h) { this->costToGoal = h; }
	int GetH() { return this->costToGoal; }

	void SetF(int f) { this->totalCost = f; }
	int GetF() { return this->totalCost; }

	void SetIsInOpenlist(bool b) { this->isInOpenlist = b; }
	bool GetIsInOpenlist() { return this->isInOpenlist; }
	void SetIsClosed(bool b) { this->isClosed = b; }
	bool GetIsClosed() { return this->isClosed; }

	void SetHeapIndex(int id) { this->heapIndex = id; }
	int GetHeapIndex() { return this->heapIndex; }

	virtual ~AstarTile() {};

};

class AstarScene : public GameNode
{
	// 이차원 배열 맵을 구성
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];

	AstarTile* startTile;	// 시작 타일
	AstarTile* destTile;	// 도착 타일

	AstarTile* currTile;	// (알고리즘이 돌아가는 중에)후보타일을 선정할 때의 기준이 되는 타일

	vector<AstarTile*> openList;	// 후보군이 들어갈 오픈리스트
	vector<AstarTile*> closeList;	// 계산이 끝난 아이들은 빼준다
	vector<AstarTile*> heap;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void FindPath();
	void AddOpenList(AstarTile* currTile);

	AstarTile* GetMinTotalCostTile();

	// heap 사용
	AstarTile* GetMinTotalCostTileWithHeap();
	void UpdateLower(AstarTile* tile);
	void InsertOpenlistWithHeap(AstarTile* tile);
	void UpdateUpper(AstarTile* tile);
	// 수학 함수
	void Swap(AstarTile* tile1, AstarTile* tile2);

	void DeleteTileInOpenlist(AstarTile* tile);

	int CalcEdgeCost(int x, int y);
	int CalcHeuristics(int x, int y);

	void MarkTileToType();	// 타일에 따라 색을 정해주는 것?

	virtual ~AstarScene() {};
};

