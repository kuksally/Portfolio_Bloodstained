#include "AstarScene.h"
#include "CommonFunction.h"

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	isInOpenlist = false;
	isClosed = false;

	this->idX = idX;
	this->idY = idY;

	center.x = idX * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);
	center.y = idY * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);

	rc.left = idX * ASTAR_TILE_SIZE;
	rc.right = rc.left + ASTAR_TILE_SIZE;
	rc.top = idY * ASTAR_TILE_SIZE;
	rc.bottom = rc.top + ASTAR_TILE_SIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	// 기본 타일 타입
	type = AstarTileType::None;

	// 이전 노드
	parentTile = nullptr;

	// 색
	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);	// create했으니 지워줌
}

void AstarTile::Update()
{
}

void AstarTile::Render(HDC hdc)
{
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// 네모 칸 안에 색을 채워 넣어주는 함수
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	SelectObject(hdc, hOldBrush);

	// F, G, H 출력
	wsprintf(szText, "F : %d", totalCost);
	TextOut(hdc, rc.left + 5, rc.top + 5, szText, strlen(szText));

	wsprintf(szText, "G : %d", costFromStart);
	TextOut(hdc, rc.left + 5, rc.top + 25, szText, strlen(szText));

	wsprintf(szText, "H : %d", costToGoal);
	TextOut(hdc, rc.left + 5, rc.top + 45, szText, strlen(szText));
}

void AstarTile::SetColor(COLORREF color)
{
	this->color = color;				
	DeleteObject(hBrush);				// 기존에 있던거 없애고

	hBrush = CreateSolidBrush(color);	// 새로 만든다
}

HRESULT AstarScene::Init()
{
	SetClientRect(g_hWnd, ASTARSIZE_X, ASTARSIZE_Y);

	for (int i = 0; i < ASTAR_TILE_COUNT; i++)		// 세로 반복 : y
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로 반복 : x
		{
			map[i][j].Init(j, i);
		}
	}

	// 포인터라서 주소를 담아야 한다 : &
	startTile = &(map[1][1]);
	startTile->SetColor(RGB(204, 102, 102));
	startTile->SetType(AstarTileType::Start);

	// 현재 타일을 startTile로 바로 지정
	currTile = startTile;

	destTile = &(map[ASTAR_TILE_COUNT - 2][ASTAR_TILE_COUNT - 2]);
	destTile->SetColor(RGB(102, 153, 255));
	destTile->SetType(AstarTileType::End);

	return S_OK;
}

void AstarScene::Release()
{
}

void AstarScene::Update()
{
	// 화면 전환 테스트
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
	{
		SceneManager::GetSingleton()->ChangeScene("전투_1");
		return;
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
	{
		SceneManager::GetSingleton()->ChangeScene("타일맵툴", "로딩씬_1");
		return;
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		// 좌표 예외처리
		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// 시작이나 목적지가 아닐 때
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(43, 0, 102));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
	}
}

void AstarScene::Render(HDC hdc)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)		// 세로 반복 : y
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로 반복 : x
		{
			map[i][j].Render(hdc);
		}
	}
}

void AstarScene::FindPath()
{
	if (currTile)
	{
		// 주위에 있는 이동 가능한 타일들을 F값 계산 후보에 넣는다. -> (a* 에서는 openlist에서 관리한다)
		AddOpenList(currTile);	// 후보가 생기고 -> 후보들의 f값이 계산된다
		currTile->SetIsClosed(true);
		//DeleteTileInOpenlist(currTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		//currTile = GetMinTotalCostTile();
		currTile = GetMinTotalCostTileWithHeap();

		if (currTile == nullptr) return;

		currTile->SetColor(RGB(102, 153, 102));	// currTile이 될 때 마다 색 변화 시켜주기

		// 도착여부 판단
		if (currTile == destTile)
		{
			MarkTileToType();
			return;
		}

		// 반복
		//FindPath();
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	// x , y 좌표
	int currIdX = currTile->GetIdX();
	int currIdY = currTile->GetIdY();

	for (int i = -1; i <= 1; i++)		// y쪽 변화 -1, 0, 1
	{
		for (int j = -1; j <= 1; j++)	// x쪽 변화 -1, 0, 1
		{
			// 예외처리(타일이 시작점과 같으면 색칠하지 않는다)
			if (i == 0 && j == 0)	continue;

			if (currIdY + i < 0 || currIdY + i >= ASTAR_TILE_COUNT ||
				currIdX + j < 0 || currIdX + j >= ASTAR_TILE_COUNT)
				continue;

			// TODO : 오픈리스트에 추가하면 안되는 조건
			// 1. 타입이 wall 일 때
			if (map[currIdY + i][currIdX + j].GetType() == AstarTileType::Wall)	continue;

			// 2. 이미 확인된 타일일 때 ( closeList에 있을 때 )
			if (map[currIdY + i][currIdX + j].GetIsClosed()) continue;

			// 3. 이미 오픈리스트에 있을 때
			// 조건에 따라 다른 처리 해주기 -> 이 때 G 값이 갱신되는지 판단 (parentTile도 갱신)
			// F, G, H를 갱신한다.
			int F, G, H;
			if (map[currIdY + i][currIdX + j].GetIsInOpenlist()) 
			{
				G = map[currIdY + i][currIdX + j].GetG();
				if (G > currTile->GetG() + CalcEdgeCost(j, i))
				{
					G = currTile->GetG() + CalcEdgeCost(j, i);
					map[currIdY + i][currIdX + j].SetG(G);

					F = G + map[currIdY + i][currIdX + j].GetH();
					map[currIdY + i][currIdX + j].SetF(F);

					// parentNode도 함께 갱신한다.
					map[currIdY + i][currIdX + j].SetParentTile(currTile);
				}

			}
			else 
			{
				// 대각선인지 상하좌우인지 판단
				G = currTile->GetG() + CalcEdgeCost(j, i);
				H = CalcHeuristics(currIdX + j, currIdY + i);
				F = G + H;
				map[currIdY + i][currIdX + j].SetG(G);
				map[currIdY + i][currIdX + j].SetH(H);
				map[currIdY + i][currIdX + j].SetF(F);

				// parentNode도 함께 갱신한다.
				map[currIdY + i][currIdX + j].SetParentTile(currTile);

				// openList에 추가
				map[currIdY + i][currIdX + j].SetIsInOpenlist(true);
				//openList.push_back(&(map[currIdY + i][currIdX + j]));

				// heap 자료구조에 추가
				InsertOpenlistWithHeap(&(map[currIdY + i][currIdX + j]));
			}

			// 색 바꿔서 확인
			map[currIdY + i][currIdX + j].SetColor(RGB(250, 244, 192));
		}
	}
}

AstarTile* AstarScene::GetMinTotalCostTile()
{
	// TODO : heap 정렬을 통한 효율성 증대
	int minF = INT_MAX;
	AstarTile* minFTile = nullptr;
	for (int i = 0; i < openList.size(); i++)
	{
		if (minF > openList[i]->GetF())
		{
			minF = openList[i]->GetF();
			minFTile = openList[i];
		}
	}

	return minFTile;
}

AstarTile* AstarScene::GetMinTotalCostTileWithHeap()
{
	if (heap.empty()) return nullptr;	// 아무것도 안들어있는 경우 예외처리

	AstarTile* tile = heap[0];	// 가장 값이 작은 타일

	// 가장 작은 값을 가진 타일을 제일 위로 두려면 가장 큰 값을 가진 타일과 비교
	heap[0] = heap.back(); // 가장 마지막에 있는 타일이 위로 올라온다
	heap[0]->SetHeapIndex(0);
	heap.pop_back();	// 제일 위로 올렸으니까 삭제해준다

	// 자식과 비교하면서 정렬한다
	if (heap.empty() == false)
	{
		UpdateLower(heap[0]);
	}

	return tile;
}

void AstarScene::UpdateLower(AstarTile* tile)	// 인덱스 : 3
{
	int lChildIndex = tile->GetHeapIndex() * 2 + 1;	
	// 인덱스가 힙 크기보다 크면 안되니까 예외처리
	if (lChildIndex >= heap.size()) return; // 왼쪽 자식이 없으면 하나도 없다, 오른쪽 자식이 있으면 오른쪽 자식은 그대로
	AstarTile* lChild = heap[lChildIndex];	// 왼 -> 부모의 인덱스 * 2
	
	// 왼쪽 자식이 있으면
	AstarTile* minChild = lChild;
	int rChildIndex = tile->GetHeapIndex() * 2 + 2;
	if (rChildIndex < heap.size())
	{
		AstarTile* rChild = heap[rChildIndex];	// 오 - >부모의 인덱스 * 2 + 1

		// 비교 -> 왼, 오 비교해서 더 작은애랑 비교?
		if (lChild->GetF() > rChild->GetF())
		{
			minChild = rChild;
		}
	}

	if (tile->GetF() > minChild->GetF())	// 부모타일이 자식타일보다 크면 바꾼다
	{
		Swap(tile, minChild);
		UpdateLower(tile);
		// 왼쪽 자식이 없을 때 까지 돌린다
	}
}

void AstarScene::InsertOpenlistWithHeap(AstarTile* tile)
{
	tile->SetHeapIndex(heap.size());
	heap.push_back(tile);

	// 부모 타일과 비교하면서 갱신
	UpdateUpper(tile);
}

void AstarScene::UpdateUpper(AstarTile* tile)
{
	// 인덱스 계산을 안 해야 할 경우 : 가장 상단일 때
	if (tile->GetHeapIndex() == 0) return;

	int parentIndex = (tile->GetHeapIndex() - 1) / 2;

	AstarTile* parentTile = heap[parentIndex];

	if (parentTile->GetF() > tile->GetF())
	{
		Swap(parentTile, tile);
		UpdateUpper(tile);
	}
}

void AstarScene::Swap(AstarTile* tile1, AstarTile* tile2)
{
	heap[tile1->GetHeapIndex()] = tile2;
	heap[tile2->GetHeapIndex()] = tile1;

	int temp = tile1->GetHeapIndex();
	tile1->SetHeapIndex(tile2->GetHeapIndex());
	tile2->SetHeapIndex(temp);
}

void AstarScene::DeleteTileInOpenlist(AstarTile* tile)
{
	// TODO : heap 구조를 통해서 개선
	vector<AstarTile*>::iterator it;
	for (it = openList.begin(); it != openList.end();)
	{
		if ((*it) == tile)
		{
			it = openList.erase(it);
			break;
		}
		else
		{
			it++;
		}
	}
}

int AstarScene::CalcEdgeCost(int x, int y)
{
	// 대각선인지 상하좌우인지 판단
	int edgeCost = 14;
	int temp = abs(x + y);	// abs() -> 절대값
	if (temp == 1)
	{
		edgeCost = 10;
	}
	return edgeCost;
}

int AstarScene::CalcHeuristics(int x, int y)
{
	int distX = destTile->GetIdX() - x;
	int distY = destTile->GetIdY() - y;

	return sqrt(distX * distX + distY * distY) * 10;
}

void AstarScene::MarkTileToType()
{
	AstarTile* parentTile = destTile;
	while (parentTile)
	{
		parentTile->SetColor(RGB(255, 102, 0));
		parentTile = parentTile->GetParentTile();
	}
}
