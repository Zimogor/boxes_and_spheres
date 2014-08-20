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

// структура динамического региона
class DynamicReg {

public:

	// динамический регион
	DynamicReg(); // конструктор
	~DynamicReg(); // деструктор

	// важные функции
	bool Draw(); // рисование

	// функции
	static void FillStatic(int location, float** regionHeightMap, XMFLOAT3** regionNormalMap, XMFLOAT3** regionTangentMap, VertTerrain* vertexBuffer, UINT* vectInstanceAmount, std::vector<TerrainVectorModel*> TerrainVectorModels); // заполнить статические переменные
	bool Fill(NumXY<int> regionAdress, myXMFLOAT3 regionCenter); // заполнить регион
	CellFiller** getContent() { return content; }; // получить ссылку на содержимое 

	// переменные
	bool busy; // буфер используется
	NumXY<int> regionAdress; // адрес региона
	myXMFLOAT3 regionCenter; // координаты центра

private:

	// статические переменные
	static float** regionHeightMap; // карта высот
	static XMFLOAT3** regionNormalMap; // карта номалей
	static XMFLOAT3** regionTangentMap; // карта тангентов
	static VertTerrain* vertexBuffer; // для генерации вершин для буфера вершин
	static std::vector<TerrainVectorModel*> TerrainVectorModels; // используемые векторы моделей
	static UINT* vectInstanceAmount; // количество экземпляров для каждого вектора
	static int location; // порядковый номер локации

	// переменные
	ID3D11Buffer* pVB; // буфер вершин
	CellFiller** content; // содержимое региона

	// функции
	bool(DynamicReg::*CreateVertexBuffer)() = NULL; // создать буфер вершин
	bool CreateVBTestLocation(); // создать буфер вершин для тестовой локации
	bool CreateVBLocation0(); // создать буфер вершин для базовой локации (первая)
	bool CreateVBLocation1(); // создать буфер вершин для локации входа (вторая)
	bool(DynamicReg::*CreateContent)() = NULL; // заполнить содержимое региона
	bool CreateTestedLocationContent(); // заполнить содержимое региона для тестовой локации
	bool CreateContentL0(); // заполнить содержимое региона для базовой локации (первая)
	bool CreateContentL1(); // заполнить содержимое региона для локации входа (вторая)
	bool GenerateItem(NumXY<int> regionAdress, NumXY<UINT> cellAdress, CELL_ITEM cellItem); // создать предмет в ячейке
	XMFLOAT2* getMask(UINT iHeight, UINT jHeight); // получить координаты маски по карте высот
	void CreateMaps(); // создать карту нормалей и тангентов по карте высот

	// сгенерировать содержимое ячейки
	bool SolveMeadowContent(int i, int j, float random, float treeFunction); // для лугового биома
	bool SolveForestContent(int i, int j, float random, float treeFunction); // для лесного биома
	bool SolveSwampContent(int i, int j, float random, float treeFunction); // для болотного биома


};

// класс территории
class Terrain {

public:

	// территория
	Terrain(int location, bool* result); // конструктор территории
	~Terrain(); // деструктор территории

	// полезные функции
	bool Draw(bool toShadow); // рисование территории
	bool Update(XMFLOAT3 camFocusPosition); // обновление территории

	// функции
	NumXY<float> GetShowPoint() { return showPoint; }; // вернуть точку рисования
	bool RemoveFromVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance* modelParams = NULL); // удалить предмет из вектора элементов, true - если удалось
	bool AddToVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance modelParams); // добавить предмет в вектор элементов
	bool TakePick(WIELD_EVENTS events, myXMFLOAT3 damagePoint); // удар орудием в точку
	CellFiller* GetCellFiller(myXMFLOAT3 position, NumXY<int>* cellAdress = NULL, NumXY<int>* regAdress = NULL); // получить ссылку на содержимое конкретной ячейки (возвращает относительный адрес региона и адрес ячейка)
	bool TakePickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult); // подбирание (true - если подобрал)


private:

	// копии указателей
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	ID3D11ShaderResourceView* pSRnormalMap = NULL; // ресурс для карты нормалей

	// переменные
	ID3D11Buffer* pIndexBuffer = NULL; // буфер индексов
	float** regionHeightMap = NULL; // карта высот для генерируемого региона
	XMFLOAT3** regionNormalMap = NULL; // карта нормалей для генерируемого региона
	XMFLOAT3** regionTangentMap = NULL; // карта тангентов
	VertTerrain* vertexBuffer = NULL; // содержимое буфера вершин
	NumXY<float> showPoint; // точка, относительно которой рисуется карта
	DynamicReg* dynamicReg = NULL; // цепочка динамических буферов
	UINT dynamicVBAmount = MathHelper::Pow2((UINT)(Const::showRadius * 2.0f) / Const::regionCellSize + 1); // количество динамических буферов
	UINT* vectInstanceAmount = NULL; // количество экземпляров для каждого вектора
	DynamicReg*** regionField = NULL; // поле указателей регионов
	std::vector<TerrainVectorModel*> TerrainVectorModels; // вектор векторов моделей
	ID3D11Buffer* pInstanceBuffer = NULL; // общий буфер экземпляров
	UINT* instStarts = NULL; // начало экземпляра каждой модели
	UINT* instAmounts = NULL; // количество экземпляров каждой модели
	NumXY<int> cornerPoint; // относительный угол поля в координатах региона
	
	// описывает порядок векторов моделей
	int pickableFrom = 0;
	int pickableTo = 0;
	int chopableFrom = 0;
	int chopableTo = 0;

	// функции
	bool CreateIndexBuffer(); // создание буфера индексов
	bool UpdateCurrentRegions(NumXY<float> newShowPoint); // обновить активыне регионы
	bool FindFreeRegion(UINT* freeReg); // найти свободный регион
	bool RegenerateRegionField(); // сгенерировать поле указателей
	bool CreateInstanceBuffer(); // создать путой общий буфер экземпляров
	bool FillInstanceBuffer(float frustumRadius); // пересоздать буфер всех объектов с учётом frustumCulling
	wchar_t* MakeDDSAdress(wchar_t* name); // собрать адрес текстуры
	wchar_t* MakeBinAdress(wchar_t* name); // собрать адрес модели
	bool GenerateTestLocation(); // сгенерировать ресурсы для тестовой локации
	bool GenerateLocation0(); // сгенерировать ресурсы для базовой локации (первая)
	bool GenerateLocation1(); // сгенерировать ресурсы для локации входа (вторая)

};
