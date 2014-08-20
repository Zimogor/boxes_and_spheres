#pragma once

#include "Utilities.h"
#include "Structures.h"
#include "Terrain.h"

const int patchVertSize = Const::patchCellSize + 1; // размер лоскута карты в вершинах

// клас патча
class Patch {

public:

	// класс пачта
	Patch(NumXY<int> bottomLeftCorner, bool* result); // конструктор
	~Patch(); // деструктор

	// важные функции
	bool Draw(); // рисование

	// функции
	bool Recreate(); // обновить содержимое патча
	static void FillStatic(int indicesAmount) { Patch::indicesAmount = indicesAmount; }; // заполнить статические переменные

	// переменные
	VertMap* verticesData = NULL; // данные цветов у вершин карты

private:

	// переменные
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин для патча

	// статические переменны
	static int indicesAmount; // количество индексов

};

// класс карты
class Map {

public:

	// карта
	Map(bool* result); // конструктор
	~Map(); // деструктор

	// важные функции
	bool Draw(); // рисование
	bool Update(); // обновление

	// функции
	bool NewRegion(DynamicReg* dynamicRegion); // создался новый регион

private:

	// переменные
	ID3D11Buffer* pIndexBuffer = NULL; // буфер индексов для патча
	std::vector<DynamicReg*> newRegions; // новые регионы для обработки
	Patch*** patchField = NULL; // поле указателей патча

};