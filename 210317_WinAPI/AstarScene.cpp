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

	// �⺻ Ÿ�� Ÿ��
	type = AstarTileType::None;

	// ���� ���
	parentTile = nullptr;

	// ��
	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);	// create������ ������
}

void AstarTile::Update()
{
}

void AstarTile::Render(HDC hdc)
{
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// �׸� ĭ �ȿ� ���� ä�� �־��ִ� �Լ�
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	SelectObject(hdc, hOldBrush);

	// F, G, H ���
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
	DeleteObject(hBrush);				// ������ �ִ��� ���ְ�

	hBrush = CreateSolidBrush(color);	// ���� �����
}

HRESULT AstarScene::Init()
{
	SetClientRect(g_hWnd, ASTARSIZE_X, ASTARSIZE_Y);

	for (int i = 0; i < ASTAR_TILE_COUNT; i++)		// ���� �ݺ� : y
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// ���� �ݺ� : x
		{
			map[i][j].Init(j, i);
		}
	}

	// �����Ͷ� �ּҸ� ��ƾ� �Ѵ� : &
	startTile = &(map[1][1]);
	startTile->SetColor(RGB(204, 102, 102));
	startTile->SetType(AstarTileType::Start);

	// ���� Ÿ���� startTile�� �ٷ� ����
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
	// ȭ�� ��ȯ �׽�Ʈ
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F1))
	{
		SceneManager::GetSingleton()->ChangeScene("����_1");
		return;
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_F2))
	{
		SceneManager::GetSingleton()->ChangeScene("Ÿ�ϸ���", "�ε���_1");
		return;
	}

	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
	{
		// g_ptMouse�� �ε����� ���
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		// ��ǥ ����ó��
		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// �����̳� �������� �ƴ� ��
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
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)		// ���� �ݺ� : y
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// ���� �ݺ� : x
		{
			map[i][j].Render(hdc);
		}
	}
}

void AstarScene::FindPath()
{
	if (currTile)
	{
		// ������ �ִ� �̵� ������ Ÿ�ϵ��� F�� ��� �ĺ��� �ִ´�. -> (a* ������ openlist���� �����Ѵ�)
		AddOpenList(currTile);	// �ĺ��� ����� -> �ĺ����� f���� ���ȴ�
		currTile->SetIsClosed(true);
		//DeleteTileInOpenlist(currTile);

		// �ĺ��� �� F���� ���� ���� Ÿ���� ���� currTile ����
		//currTile = GetMinTotalCostTile();
		currTile = GetMinTotalCostTileWithHeap();

		if (currTile == nullptr) return;

		currTile->SetColor(RGB(102, 153, 102));	// currTile�� �� �� ���� �� ��ȭ �����ֱ�

		// �������� �Ǵ�
		if (currTile == destTile)
		{
			MarkTileToType();
			return;
		}

		// �ݺ�
		//FindPath();
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	// x , y ��ǥ
	int currIdX = currTile->GetIdX();
	int currIdY = currTile->GetIdY();

	for (int i = -1; i <= 1; i++)		// y�� ��ȭ -1, 0, 1
	{
		for (int j = -1; j <= 1; j++)	// x�� ��ȭ -1, 0, 1
		{
			// ����ó��(Ÿ���� �������� ������ ��ĥ���� �ʴ´�)
			if (i == 0 && j == 0)	continue;

			if (currIdY + i < 0 || currIdY + i >= ASTAR_TILE_COUNT ||
				currIdX + j < 0 || currIdX + j >= ASTAR_TILE_COUNT)
				continue;

			// TODO : ���¸���Ʈ�� �߰��ϸ� �ȵǴ� ����
			// 1. Ÿ���� wall �� ��
			if (map[currIdY + i][currIdX + j].GetType() == AstarTileType::Wall)	continue;

			// 2. �̹� Ȯ�ε� Ÿ���� �� ( closeList�� ���� �� )
			if (map[currIdY + i][currIdX + j].GetIsClosed()) continue;

			// 3. �̹� ���¸���Ʈ�� ���� ��
			// ���ǿ� ���� �ٸ� ó�� ���ֱ� -> �� �� G ���� ���ŵǴ��� �Ǵ� (parentTile�� ����)
			// F, G, H�� �����Ѵ�.
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

					// parentNode�� �Բ� �����Ѵ�.
					map[currIdY + i][currIdX + j].SetParentTile(currTile);
				}

			}
			else 
			{
				// �밢������ �����¿����� �Ǵ�
				G = currTile->GetG() + CalcEdgeCost(j, i);
				H = CalcHeuristics(currIdX + j, currIdY + i);
				F = G + H;
				map[currIdY + i][currIdX + j].SetG(G);
				map[currIdY + i][currIdX + j].SetH(H);
				map[currIdY + i][currIdX + j].SetF(F);

				// parentNode�� �Բ� �����Ѵ�.
				map[currIdY + i][currIdX + j].SetParentTile(currTile);

				// openList�� �߰�
				map[currIdY + i][currIdX + j].SetIsInOpenlist(true);
				//openList.push_back(&(map[currIdY + i][currIdX + j]));

				// heap �ڷᱸ���� �߰�
				InsertOpenlistWithHeap(&(map[currIdY + i][currIdX + j]));
			}

			// �� �ٲ㼭 Ȯ��
			map[currIdY + i][currIdX + j].SetColor(RGB(250, 244, 192));
		}
	}
}

AstarTile* AstarScene::GetMinTotalCostTile()
{
	// TODO : heap ������ ���� ȿ���� ����
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
	if (heap.empty()) return nullptr;	// �ƹ��͵� �ȵ���ִ� ��� ����ó��

	AstarTile* tile = heap[0];	// ���� ���� ���� Ÿ��

	// ���� ���� ���� ���� Ÿ���� ���� ���� �η��� ���� ū ���� ���� Ÿ�ϰ� ��
	heap[0] = heap.back(); // ���� �������� �ִ� Ÿ���� ���� �ö�´�
	heap[0]->SetHeapIndex(0);
	heap.pop_back();	// ���� ���� �÷����ϱ� �������ش�

	// �ڽİ� ���ϸ鼭 �����Ѵ�
	if (heap.empty() == false)
	{
		UpdateLower(heap[0]);
	}

	return tile;
}

void AstarScene::UpdateLower(AstarTile* tile)	// �ε��� : 3
{
	int lChildIndex = tile->GetHeapIndex() * 2 + 1;	
	// �ε����� �� ũ�⺸�� ũ�� �ȵǴϱ� ����ó��
	if (lChildIndex >= heap.size()) return; // ���� �ڽ��� ������ �ϳ��� ����, ������ �ڽ��� ������ ������ �ڽ��� �״��
	AstarTile* lChild = heap[lChildIndex];	// �� -> �θ��� �ε��� * 2
	
	// ���� �ڽ��� ������
	AstarTile* minChild = lChild;
	int rChildIndex = tile->GetHeapIndex() * 2 + 2;
	if (rChildIndex < heap.size())
	{
		AstarTile* rChild = heap[rChildIndex];	// �� - >�θ��� �ε��� * 2 + 1

		// �� -> ��, �� ���ؼ� �� �����ֶ� ��?
		if (lChild->GetF() > rChild->GetF())
		{
			minChild = rChild;
		}
	}

	if (tile->GetF() > minChild->GetF())	// �θ�Ÿ���� �ڽ�Ÿ�Ϻ��� ũ�� �ٲ۴�
	{
		Swap(tile, minChild);
		UpdateLower(tile);
		// ���� �ڽ��� ���� �� ���� ������
	}
}

void AstarScene::InsertOpenlistWithHeap(AstarTile* tile)
{
	tile->SetHeapIndex(heap.size());
	heap.push_back(tile);

	// �θ� Ÿ�ϰ� ���ϸ鼭 ����
	UpdateUpper(tile);
}

void AstarScene::UpdateUpper(AstarTile* tile)
{
	// �ε��� ����� �� �ؾ� �� ��� : ���� ����� ��
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
	// TODO : heap ������ ���ؼ� ����
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
	// �밢������ �����¿����� �Ǵ�
	int edgeCost = 14;
	int temp = abs(x + y);	// abs() -> ���밪
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
