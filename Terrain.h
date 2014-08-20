#pragma once

#include "Utilities.h"
#include "Nums.h"
#include "TerrainVectorModel.h"
#include "SuperFunctions.h"

#define FOREST_MEADOW_BORDER 0.7f
#define FOREST_SWAMP_BORDER -0.7f
#define INSTANCE_BUFFER_MODEL_SIZE 2000
#define MODEL_RADIUS 1.0f

class Mediator;

// ��������� ������������� �������
class DynamicReg {

public:

	// ������������ ������
	DynamicReg(); // �����������
	~DynamicReg(); // ����������

	// ������ �������
	bool Draw(); // ���������

	// �������
	static void FillStatic(int location, float** regionHeightMap, XMFLOAT3** regionNormalMap, XMFLOAT3** regionTangentMap, VertTerrain* vertexBuffer, UINT* vectInstanceAmount, std::vector<TerrainVectorModel*> TerrainVectorModels); // ��������� ����������� ����������
	bool Fill(NumXY<int> regionAdress, myXMFLOAT3 regionCenter); // ��������� ������
	CellFiller** getContent() { return content; }; // �������� ������ �� ���������� 

	// ����������
	bool busy; // ����� ������������
	NumXY<int> regionAdress; // ����� �������
	myXMFLOAT3 regionCenter; // ���������� ������

private:

	// ����������� ����������
	static float** regionHeightMap; // ����� �����
	static XMFLOAT3** regionNormalMap; // ����� �������
	static XMFLOAT3** regionTangentMap; // ����� ���������
	static VertTerrain* vertexBuffer; // ��� ��������� ������ ��� ������ ������
	static std::vector<TerrainVectorModel*> TerrainVectorModels; // ������������ ������� �������
	static UINT* vectInstanceAmount; // ���������� ����������� ��� ������� �������
	static int location; // ���������� ����� �������

	// ����������
	ID3D11Buffer* pVB; // ����� ������
	CellFiller** content; // ���������� �������

	// �������
	bool(DynamicReg::*CreateVertexBuffer)() = NULL; // ������� ����� ������
	bool CreateVBTestLocation(); // ������� ����� ������ ��� �������� �������
	bool CreateVBLocation0(); // ������� ����� ������ ��� ������� ������� (������)
	bool CreateVBLocation1(); // ������� ����� ������ ��� ������� ����� (������)
	bool(DynamicReg::*CreateContent)() = NULL; // ��������� ���������� �������
	bool CreateTestedLocationContent(); // ��������� ���������� ������� ��� �������� �������
	bool CreateContentL0(); // ��������� ���������� ������� ��� ������� ������� (������)
	bool CreateContentL1(); // ��������� ���������� ������� ��� ������� ����� (������)
	bool GenerateItem(NumXY<int> regionAdress, NumXY<UINT> cellAdress, CELL_ITEM cellItem); // ������� ������� � ������
	XMFLOAT2* getMask(UINT iHeight, UINT jHeight); // �������� ���������� ����� �� ����� �����
	void CreateMaps(); // ������� ����� �������� � ��������� �� ����� �����

	// ������������� ���������� ������
	bool SolveMeadowContent(int i, int j, float random, float treeFunction); // ��� �������� �����
	bool SolveForestContent(int i, int j, float random, float treeFunction); // ��� ������� �����
	bool SolveSwampContent(int i, int j, float random, float treeFunction); // ��� ��������� �����


};

// ����� ����������
class Terrain {

public:

	// ����������
	Terrain(int location, bool* result); // ����������� ����������
	~Terrain(); // ���������� ����������

	// �������� �������
	bool Draw(bool toShadow); // ��������� ����������
	bool Update(XMFLOAT3 camFocusPosition); // ���������� ����������

	// �������
	NumXY<float> GetShowPoint() { return showPoint; }; // ������� ����� ���������
	bool RemoveFromVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance* modelParams = NULL); // ������� ������� �� ������� ���������, true - ���� �������
	bool AddToVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance modelParams); // �������� ������� � ������ ���������
	bool TakePick(WIELD_EVENTS events, myXMFLOAT3 damagePoint); // ���� ������� � �����
	CellFiller* GetCellFiller(myXMFLOAT3 position, NumXY<int>* cellAdress = NULL, NumXY<int>* regAdress = NULL); // �������� ������ �� ���������� ���������� ������ (���������� ������������� ����� ������� � ����� ������)
	bool TakePickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult); // ���������� (true - ���� ��������)


private:

	// ����� ����������
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	ID3D11ShaderResourceView* pSRnormalMap = NULL; // ������ ��� ����� ��������

	// ����������
	ID3D11Buffer* pIndexBuffer = NULL; // ����� ��������
	float** regionHeightMap = NULL; // ����� ����� ��� ������������� �������
	XMFLOAT3** regionNormalMap = NULL; // ����� �������� ��� ������������� �������
	XMFLOAT3** regionTangentMap = NULL; // ����� ���������
	VertTerrain* vertexBuffer = NULL; // ���������� ������ ������
	NumXY<float> showPoint; // �����, ������������ ������� �������� �����
	DynamicReg* dynamicReg = NULL; // ������� ������������ �������
	UINT dynamicVBAmount = MathHelper::Pow2((UINT)(Const::showRadius * 2.0f) / Const::regionCellSize + 1); // ���������� ������������ �������
	UINT* vectInstanceAmount = NULL; // ���������� ����������� ��� ������� �������
	DynamicReg*** regionField = NULL; // ���� ���������� ��������
	std::vector<TerrainVectorModel*> TerrainVectorModels; // ������ �������� �������
	ID3D11Buffer* pInstanceBuffer = NULL; // ����� ����� �����������
	UINT* instStarts = NULL; // ������ ���������� ������ ������
	UINT* instAmounts = NULL; // ���������� ����������� ������ ������
	NumXY<int> cornerPoint; // ������������� ���� ���� � ����������� �������
	
	// ��������� ������� �������� �������
	int pickableFrom = 0;
	int pickableTo = 0;
	int chopableFrom = 0;
	int chopableTo = 0;

	// �������
	bool CreateIndexBuffer(); // �������� ������ ��������
	bool UpdateCurrentRegions(NumXY<float> newShowPoint); // �������� �������� �������
	bool FindFreeRegion(UINT* freeReg); // ����� ��������� ������
	bool RegenerateRegionField(); // ������������� ���� ����������
	bool CreateInstanceBuffer(); // ������� ����� ����� ����� �����������
	bool FillInstanceBuffer(float frustumRadius); // ����������� ����� ���� �������� � ������ frustumCulling
	wchar_t* MakeDDSAdress(wchar_t* name); // ������� ����� ��������
	wchar_t* MakeBinAdress(wchar_t* name); // ������� ����� ������
	bool GenerateTestLocation(); // ������������� ������� ��� �������� �������
	bool GenerateLocation0(); // ������������� ������� ��� ������� ������� (������)
	bool GenerateLocation1(); // ������������� ������� ��� ������� ����� (������)

};
