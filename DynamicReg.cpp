#include "Terrain.h"
#include "Mediator.h"

// ����������� ����������
float** DynamicReg::regionHeightMap = NULL; // ����� �����
XMFLOAT3** DynamicReg::regionNormalMap = NULL; // ����� �������
XMFLOAT3** DynamicReg::regionTangentMap = NULL; // ����� ���������
VertTerrain* DynamicReg::vertexBuffer = NULL; // ��� ��������� ������ ��� ������ ������
UINT* DynamicReg::vectInstanceAmount = NULL; // ���������� ����������� ��� ������� �������
std::vector<TerrainVectorModel*> DynamicReg::TerrainVectorModels; // ������������ ������� �������
int DynamicReg::location = 0; // ����� �������

// �����������
DynamicReg::DynamicReg() {

	// ����������� ������� ��������� ����������
	switch (location) {
	case 0:
		CreateContent = &DynamicReg::CreateContentL0;
		CreateVertexBuffer = &DynamicReg::CreateVBLocation0;
		break;
	case 1:
		CreateContent = &DynamicReg::CreateContentL1;
		CreateVertexBuffer = &DynamicReg::CreateVBLocation1;
		break;
	}
	if (Const::testFileName) CreateContent = &DynamicReg::CreateTestedLocationContent;

	// ������ �����
	VertTerrain* vertexBuffer = new VertTerrain[Const::regionVertexAmount];
	ZeroMemory(vertexBuffer, sizeof(VertTerrain)* Const::regionVertexAmount);

	// �������� ������� �����������
	content = new CellFiller*[Const::regionCellSize];
	for (int i(0); i < Const::regionCellSize; i++) {
		content[i] = new CellFiller[Const::regionCellSize];
		for (int j(0); j < Const::regionCellSize; j++) {
			content[i][j] = CellFiller(CI_NONE, 0);
		}
	}

	// ��������� ������ ������
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(VertTerrain)* Const::regionVertexAmount;
	bufferVertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_DYNAMIC;

	// �������� ������� ������ ������
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = vertexBuffer;
	if (FAILED(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVB))) {
		Mediator::errors->Push(L"dynamic regions creating failed");
	}
	delete[] vertexBuffer;

	busy = false;

}

// ���������� ����������� ����������
void DynamicReg::FillStatic(int location, float** regionHeightMap, XMFLOAT3** regionNormalMap, XMFLOAT3** regionTangentMap, VertTerrain* vertexBuffer, UINT* vectInstanceAmount, std::vector<TerrainVectorModel*> TerrainVectorModels) {

	DynamicReg::location = location;
	DynamicReg::regionHeightMap = regionHeightMap;
	DynamicReg::regionNormalMap = regionNormalMap;
	DynamicReg::regionTangentMap = regionTangentMap;
	DynamicReg::vectInstanceAmount = vectInstanceAmount;
	DynamicReg::TerrainVectorModels = TerrainVectorModels;
	DynamicReg::vertexBuffer = vertexBuffer;

}

// ���������
bool DynamicReg::Draw() {

	UINT vertexStride = sizeof(VertTerrain);
	UINT vertexOffset = 0;

	// ���������� ���������
	myXMFLOAT3 positionJP = myXMFLOAT3(regionAdress.x * (float)Const::regionCellSize, 0.0f, regionAdress.y * (float)Const::regionCellSize) - Mediator::camera->GetPosition(); // ����� ��������
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVB, &vertexStride, &vertexOffset);

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(W_JP);

	// ���������
	Mediator::pDevContext->DrawIndexed(Const::regionIndexAmount, 0, 0);

	return true;

}

// ������� ������� � ������
bool DynamicReg::GenerateItem(NumXY<int> regionAdress, NumXY<UINT> cellAdress, CELL_ITEM cellItem) {

	content[cellAdress.x][cellAdress.y].item = cellItem;
	float x = regionAdress.x * Const::regionCellSize + (int)cellAdress.x + 0.5f;
	float z = regionAdress.y * Const::regionCellSize + (int)cellAdress.y + 0.5f;
	float y = 0.5f * (regionHeightMap[cellAdress.x + 2][cellAdress.y + 1] + regionHeightMap[cellAdress.x + 1][cellAdress.y + 2]);
	myXMFLOAT3 position(x, y, z);
	BR(TerrainVectorModels[cellItem]->Spawn(&position));
	content[cellAdress.x][cellAdress.y].itemCenter = NumXY<float>(position.x, position.z); // ��������� ���������� �������
	vectInstanceAmount[cellItem]++;

	return true;

}

// ������������� ���������� ������ ��� ������� �����
bool DynamicReg::SolveForestContent(int i, int j, float random, float treeFunction) {

	// ������������� ����
	if (treeFunction > 0.1f) {
		if (random < 0.5f) {
			BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_FIR)); // ������������� ������� ���
		}
		else {
			BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_BOLDFIR)); // ������������� ����� ���
		}
		content[i][j].blocked = 1;
		content[i][j].additional = 3; // ����� ������
	}
	else if(treeFunction < -0.4f) {
		BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_WHEAT)); // ������������� �������
	}
	else {
		// ��������� ��������
		if (random > 0.98f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_FERN)); } // ������������� ���������
		else if (random > 0.94f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_MUSHROOM)) } // ������������� ����
	}

	content[i][j].biomType = B_FOREST;
	return true;

}

// ������������� ���������� ������ ��� ��������� �����
bool DynamicReg::SolveSwampContent(int i, int j, float random, float treeFunction) {

	if (random > 0.75f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_TUSSOCK)) } // ������������� �������� ����
	else if (random > 0.6f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_REED)) } // ������������� �����
	else if (random > 0.5f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_CAMOMILE)) } // ������������� �������

	content[i][j].biomType = B_SWAMP;
	return true;

}

// ������������� ���������� ������ ��� �������� �����
bool DynamicReg::SolveMeadowContent(int i, int j, float random, float treeFunction) {

	if (treeFunction > 0.2f) {
		BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_TREE)); // ������
		content[i][j].blocked = 1;
		content[i][j].additional = 3; // ����� ������
	}
	else if (treeFunction < -0.49f) {
		BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_BOULDER)); // ��������
		content[i][j].blocked = 1;
		content[i][j].additional = 3; // ����� �����
	}
	else {

		// ������������� ��������� ��������
		if (random > 0.95f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_BRUSH)); } // ����� �����
		else if (random > 0.80f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_SUNFLOWER)) } // ���������
		else if (random > 0.6f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_DANDELION)) } // ������������� ���������
	}

	content[i][j].biomType = B_MEADOW;
	return true;

}

// ����� ������ ������ � ��������� ���, false - ���� ���������
bool DynamicReg::Fill(NumXY<int> regionAdress, myXMFLOAT3 regionCenter) {

	// ���������� �������
	this->regionAdress = regionAdress;
	this->regionCenter = regionCenter;
	busy = true;

	(this->*CreateVertexBuffer)(); // ������� ����� ������
	(this->*CreateContent)(); // ������� ���������� �������

	// ������� � ����������
	Mediator::SendEventNewRegion(this);

	return true;

}

// ����������
DynamicReg::~DynamicReg() {

	ReleaseCOM(pVB);
	for (int i(0); i < Const::regionCellSize; i++) {
		delete[] content[i];
	}
	ReleaseNULLS(content);

};