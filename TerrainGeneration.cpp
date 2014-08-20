#include "Terrain.h"
#include "Mediator.h"

#define dX 0.0078125f // смещение дублирующео слоя пикселей диффуза и карты нормалей
#define dx 0.00390625f // смещение диблирующего слоя пикселей альфа-масок

// текстурные координаты масок
XMFLOAT2 filledMask[4] = { XMFLOAT2(0.0f + dx, 0.0f + dx), XMFLOAT2(0.25f - dx, 0.0f + dx), XMFLOAT2(0.0f + dx, 0.25f - dx), XMFLOAT2(0.25f - dx, 0.25f - dx) };
XMFLOAT2 emptyMask[4] = { XMFLOAT2(0.25f + dx, 0.0f + dx), XMFLOAT2(0.5f - dx, 0.0f + dx), XMFLOAT2(0.25f + dx, 0.25f - dx), XMFLOAT2(0.5f - dx, 0.25f - dx) };
XMFLOAT2 gsMaskS1[4] = { XMFLOAT2(0.25f - dx, 0.25f + dx), XMFLOAT2(0.0f + dx, 0.25f + dx), XMFLOAT2(0.25f - dx, 0.5f - dx), XMFLOAT2(0.0f + dx, 0.5f - dx) };
XMFLOAT2 gsMaskS2[4] = { XMFLOAT2(0.0f + dx, 0.25f + dx), XMFLOAT2(0.25f - dx, 0.25f + dx), XMFLOAT2(0.0f + dx, 0.5f - dx), XMFLOAT2(0.25f - dx, 0.5f - dx) };
XMFLOAT2 gsMaskS3[4] = { XMFLOAT2(0.25f - dx, 0.5f - dx), XMFLOAT2(0.25f - dx, 0.25f + dx), XMFLOAT2(0.0f + dx, 0.5f - dx), XMFLOAT2(0.0f + dx, 0.25f + dx) };
XMFLOAT2 gsMaskS4[4] = { XMFLOAT2(0.0f + dx, 0.5f - dx), XMFLOAT2(0.0f + dx, 0.25f + dx), XMFLOAT2(0.25f - dx, 0.5f - dx), XMFLOAT2(0.25f - dx, 0.25f + dx) };
XMFLOAT2 gsMaskT1[4] = { XMFLOAT2(0.25f + dx, 0.25f + dx), XMFLOAT2(0.25f + dx, 0.5f - dx), XMFLOAT2(0.5f - dx, 0.25f + dx), XMFLOAT2(0.5f - dx, 0.5f - dx) };
XMFLOAT2 gsMaskT2[4] = { XMFLOAT2(0.5f - dx, 0.25f + dx), XMFLOAT2(0.5f - dx, 0.5f - dx), XMFLOAT2(0.25f + dx, 0.25f + dx), XMFLOAT2(0.25f + dx, 0.5f - dx) };
XMFLOAT2 gsMaskT3[4] = { XMFLOAT2(0.5f - dx, 0.5f - dx), XMFLOAT2(0.5f - dx, 0.25f + dx), XMFLOAT2(0.25f + dx, 0.5f - dx), XMFLOAT2(0.25f + dx, 0.25f + dx) };
XMFLOAT2 gsMaskT4[4] = { XMFLOAT2(0.25f + dx, 0.5f - dx), XMFLOAT2(0.25f + dx, 0.25f + dx), XMFLOAT2(0.5f - dx, 0.5f - dx), XMFLOAT2(0.5f - dx, 0.25f + dx) };
XMFLOAT2 gsMaskA1[4] = { XMFLOAT2(0.75f - dx, 0.25f - dx), XMFLOAT2(0.75f - dx, 0.0f + dx), XMFLOAT2(0.5f + dx, 0.25f - dx), XMFLOAT2(0.5f + dx, 0.0f + dx) };
XMFLOAT2 gsMaskA2[4] = { XMFLOAT2(0.5f + dx, 0.25f - dx), XMFLOAT2(0.5f + dx, 0.0f + dx), XMFLOAT2(0.75f - dx, 0.25f - dx), XMFLOAT2(0.75f - dx, 0.0f + dx) };
XMFLOAT2 gsMaskA3[4] = { XMFLOAT2(0.5f + dx, 0.0f + dx), XMFLOAT2(0.5f + dx, 0.25f - dx), XMFLOAT2(0.75f - dx, 0.0f + dx), XMFLOAT2(0.75f - dx, 0.25f - dx) };
XMFLOAT2 gsMaskA4[4] = { XMFLOAT2(0.75f - dx, 0.0f + dx), XMFLOAT2(0.75f - dx, 0.25f - dx), XMFLOAT2(0.5f + dx, 0.0f + dx), XMFLOAT2(0.5f + dx, 0.25f - dx) };

// сгенерировать ресурсы для базовой локации (первая)
bool Terrain::GenerateLocation0() {

	bool result;

	// определение названий объектов
	wchar_t* brushName, *fernName, *mushroomName, *firName, *boldFirName, *treeName, *boulderName, *reedName, *sunflowerName, *tussockName, *dandelionName, *wheatName, *camomileName, *stumpName;
	brushName = L"brush";
	mushroomName = L"mushroom";
	boldFirName = L"fir_gray";
	boulderName = L"boulder";
	reedName = L"reed";
	sunflowerName = L"sunflower";
	tussockName = L"tussock";
	dandelionName = L"dandelion";
	wheatName = L"wheat";
	camomileName = L"camomile";
	stumpName = L"stump";
	if (Const::detalization) {
		treeName = L"tree_highpoly";
		fernName = L"fern_highpoly";
		firName = L"fir_bold_highpoly";
	}
	else {
		fernName = L"fern_lowpoly";
		firName = L"fir_bold_lowpoly";
		treeName = L"tree_lowpoly";
	}

	// создание векторов моделей (должно соотвествовать структуре CELL_ITEM) !!!
	BRRC1(TerrainVectorModel* brush = new TerrainVectorModel(MakeDDSAdress(brushName), MakeBinAdress(brushName), &result)); // кусты
	BRRC1(TerrainVectorModel* fern = new TerrainVectorModel(MakeDDSAdress(fernName), MakeBinAdress(fernName), &result)); // папоротник
	BRRC1(TerrainVectorModel* mushroom = new TerrainVectorModel(MakeDDSAdress(mushroomName), MakeBinAdress(mushroomName), &result)); // грибы
	BRRC1(TerrainVectorModel* fir = new TerrainVectorModel(MakeDDSAdress(firName), MakeBinAdress(firName), &result)); // ель
	BRRC1(TerrainVectorModel* boldFir = new TerrainVectorModel(MakeDDSAdress(firName), MakeBinAdress(boldFirName), &result)); // лысая ель
	BRRC1(TerrainVectorModel* tree = new TerrainVectorModel(MakeDDSAdress(treeName), MakeBinAdress(treeName), &result)); // деревья
	BRRC1(TerrainVectorModel* boulder = new TerrainVectorModel(MakeDDSAdress(boulderName), MakeBinAdress(boulderName), &result)); // камни
	BRRC1(TerrainVectorModel* reed = new TerrainVectorModel(MakeDDSAdress(reedName), MakeBinAdress(reedName), &result)); // камыши
	BRRC1(TerrainVectorModel* sunflower = new TerrainVectorModel(MakeDDSAdress(sunflowerName), MakeBinAdress(sunflowerName), &result)); // подсолнухи
	BRRC1(TerrainVectorModel* tussock = new TerrainVectorModel(MakeDDSAdress(tussockName), MakeBinAdress(tussockName), &result)); // болотная кочка
	BRRC1(TerrainVectorModel* dandelion = new TerrainVectorModel(MakeDDSAdress(dandelionName), MakeBinAdress(dandelionName), &result)); // одуванчик
	BRRC1(TerrainVectorModel* wheat = new TerrainVectorModel(MakeDDSAdress(wheatName), MakeBinAdress(wheatName), &result)); // пшеница
	BRRC1(TerrainVectorModel* camomile = new TerrainVectorModel(MakeDDSAdress(camomileName), MakeBinAdress(camomileName), &result)); // ромашки
	BRRC1(TerrainVectorModel* stump = new TerrainVectorModel(MakeDDSAdress(firName), MakeBinAdress(stumpName), &result)); // пни

	// можно подбирать
	TerrainVectorModels.push_back(brush);
	TerrainVectorModels.push_back(fern);
	TerrainVectorModels.push_back(mushroom);
	// можно рубить топором
	TerrainVectorModels.push_back(fir);
	TerrainVectorModels.push_back(boldFir);
	TerrainVectorModels.push_back(tree);
	// прочее
	TerrainVectorModels.push_back(boulder);
	TerrainVectorModels.push_back(reed);
	TerrainVectorModels.push_back(sunflower);
	TerrainVectorModels.push_back(tussock);
	TerrainVectorModels.push_back(dandelion);
	TerrainVectorModels.push_back(wheat);
	TerrainVectorModels.push_back(camomile);
	TerrainVectorModels.push_back(stump);

	return true;

}

// сгенерировать ресурсы для локации входа (вторая)
bool Terrain::GenerateLocation1() {

	bool result;

	// определение названий объектов
	wchar_t* tree_type1, *tree_type2, *tree_type3;
	tree_type1 = L"tree_type1";
	tree_type2 = L"tree_type2";
	tree_type3 = L"tree_type3";

	// создание векторов моделей (должно соотвествовать структуре CELL_ITEM) !!!
	BRRC1(TerrainVectorModel* tt1 = new TerrainVectorModel(MakeDDSAdress(tree_type1), MakeBinAdress(tree_type1), &result)); // дерево тип 1
	BRRC1(TerrainVectorModel* tt2 = new TerrainVectorModel(MakeDDSAdress(tree_type2), MakeBinAdress(tree_type2), &result)); // дерево тип 2
	BRRC1(TerrainVectorModel* tt3 = new TerrainVectorModel(MakeDDSAdress(tree_type3), MakeBinAdress(tree_type3), &result)); // дерево тип 3

	// можно подбирать
	TerrainVectorModels.push_back(tt1);
	TerrainVectorModels.push_back(tt2);
	TerrainVectorModels.push_back(tt3);

	return true;

}

// сгенерировать ресурсы для локации входа (вторая)
bool Terrain::GenerateTestLocation() {

	bool result;

	// создание векторов моделей
	BRRC1(TerrainVectorModel* testedObject = new TerrainVectorModel(MakeDDSAdress(Const::testFileName), MakeBinAdress(Const::testFileName), &result)); // тестируемый объект
	TerrainVectorModels.push_back(testedObject);

	return true;

}

// получить координаты маски по карте высот
XMFLOAT2* DynamicReg::getMask(UINT iHeight, UINT jHeight) {

	char byteIJ = (FOREST_SWAMP_BORDER < regionHeightMap[iHeight][jHeight]) && (regionHeightMap[iHeight][jHeight] < FOREST_MEADOW_BORDER) ? 0 : 1;
	char byteI1J = (FOREST_SWAMP_BORDER < regionHeightMap[iHeight + 1][jHeight]) && (regionHeightMap[iHeight + 1][jHeight] < FOREST_MEADOW_BORDER) ? 0 : 2;
	char byteIJ1 = (FOREST_SWAMP_BORDER < regionHeightMap[iHeight][jHeight + 1]) && (regionHeightMap[iHeight][jHeight + 1] < FOREST_MEADOW_BORDER) ? 0 : 4;
	char byteI1J1 = (FOREST_SWAMP_BORDER < regionHeightMap[iHeight + 1][jHeight + 1]) && (regionHeightMap[iHeight + 1][jHeight + 1] < FOREST_MEADOW_BORDER) ? 0 : 8;

	switch (byteIJ + byteI1J + byteIJ1 + byteI1J1) {
	case 0: return filledMask;
	case 15: return emptyMask;
	case 12: return gsMaskS1;
	case 3: return gsMaskS2;
	case 10: return gsMaskS3;
	case 5: return gsMaskS4;
	case 2: return gsMaskT1;
	case 1: return gsMaskT2;
	case 4: return gsMaskT3;
	case 8: return gsMaskT4;
	case 11: return gsMaskA1;
	case 7: return gsMaskA2;
	case 13: return gsMaskA3;
	case 14: return gsMaskA4;
	default: return filledMask;
	}

}

// заполнить содержимое региона для базовой локации (первая)
bool DynamicReg::CreateContentL0() {

	// обнулить количество экземпляров
	ZeroMemory(vectInstanceAmount, sizeof(UINT)* TerrainVectorModels.size());

	// перезаписать содержимое
	for (int i(0); i < Const::regionCellSize; i++) {
		for (int j(0); j < Const::regionCellSize; j++) {
			content[i][j] = CellFiller();
			float xAdress = regionAdress.x * (float)Const::regionCellSize + i;
			float yAdress = regionAdress.y * (float)Const::regionCellSize + j;
			float treeFunction = Mediator::superFunct->TreeFunction(xAdress, yAdress);
			float random = Mediator::superFunct->FullRandom0(xAdress, yAdress); // полная случайность

			// генерация содержимого
			if (regionHeightMap[i + 1][j + 1] >= FOREST_MEADOW_BORDER) {
				BR(SolveMeadowContent(i, j, random, treeFunction)); // для лугового биома
			}
			else if (regionHeightMap[i + 1][j + 1] <= FOREST_SWAMP_BORDER) {
				BR(SolveSwampContent(i, j, random, treeFunction)); // для болотного биома
			}
			else {
				BR(SolveForestContent(i, j, random, treeFunction)); // для лесного биома
			}

		} // для каждой ячейки
	}

	// обновить описатели
	for (UINT i(0); i < TerrainVectorModels.size(); i++) {
		if (vectInstanceAmount[i]) BR(TerrainVectorModels[i]->AddDescription(regionAdress, vectInstanceAmount[i]));
	}

	return true;

}

// заполнить содержимое региона для локации входа (вторая)
bool DynamicReg::CreateContentL1() {

	// обнулить количество экземпляров
	ZeroMemory(vectInstanceAmount, sizeof(UINT)* TerrainVectorModels.size());

	// перезаписать содержимое
	for (int i(0); i < Const::regionCellSize; i++) {
		for (int j(0); j < Const::regionCellSize; j++) {
			content[i][j] = CellFiller();
			content[i][j].biomType = B_DREAMFOREST;
			float xAdress = regionAdress.x * (float)Const::regionCellSize + i;
			float yAdress = regionAdress.y * (float)Const::regionCellSize + j;
			float treeFunction = Mediator::superFunct->TreeFunction(xAdress, yAdress);
			float random = Mediator::superFunct->FullRandom0(xAdress, yAdress); // полная случайность

			// генерация содержимого
			if (treeFunction > 0.0f) {
				if (random > 0.3f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_TT1)); }
				else if (random > -0.3f) { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_TT2)); }
				else { BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), CI_TT3)); }
			}

		} // для каждой ячейки
	}

	// обновить описатели
	for (UINT i(0); i < TerrainVectorModels.size(); i++) {
		if (vectInstanceAmount[i]) BR(TerrainVectorModels[i]->AddDescription(regionAdress, vectInstanceAmount[i]));
	}

	return true;

}

// заполнить содержимое региона для тестовой локации
bool DynamicReg::CreateTestedLocationContent() {

	// обнулить количество экземпляров
	ZeroMemory(vectInstanceAmount, sizeof(UINT)* TerrainVectorModels.size());

	// перезаписать содержимое
	for (int i(0); i < Const::regionCellSize; i++) {
		for (int j(0); j < Const::regionCellSize; j++) {
			content[i][j] = CellFiller();
			content[i][j].biomType = B_TESTED;
			float xAdress = regionAdress.x * (float)Const::regionCellSize + i;
			float yAdress = regionAdress.y * (float)Const::regionCellSize + j;

			// режим отладки одиночного объекта
			BR(GenerateItem(regionAdress, NumXY<UINT>(i, j), 0)); // тестируемый объект


		} // для каждой ячейки
	}

	// обновить описатели
	for (UINT i(0); i < TerrainVectorModels.size(); i++) {
		if (vectInstanceAmount[i]) BR(TerrainVectorModels[i]->AddDescription(regionAdress, vectInstanceAmount[i]));
	}

	return true;

}

// создать буфер вершин для базовой локации (первая)
bool DynamicReg::CreateVBLocation0() {

	// вспомогательные переменные
	VertTerrain* pointer = vertexBuffer;
	const float forestSquadInTile = 1.0f / Const::tilesInSquadForest;
	const float meadowSquadInTile = 1.0f / Const::tilesInSquadMeadow;
	const float swampSquadInTile = 1.0f / Const::tilesInSquadSwamp;
	const float halfForestSquadInTile = forestSquadInTile * 0.5f;
	const float halfMeadowSquadInTile = meadowSquadInTile * 0.5f;
	const float halfSwampSquadInTile = swampSquadInTile * 0.5f;

	// создание карты высот и нормалей
	CreateMaps();

	// содержимое буфера
	for (int i(0); i < Const::regionCellSize; i++)
		for (int j(0); j < Const::regionCellSize; j++) {

			// определение текстурных координат маски смешивания
			XMFLOAT2* maskTile = getMask(i + 1, j + 1);

			// определение полных координат тайлов
			UINT xPosition = regionAdress.x * Const::regionCellSize + i;
			UINT yPosition = regionAdress.y * Const::regionCellSize + j;

			// определение текстурных координат тайлов
			float left1, right1, bottom1, top1, left2, right2, bottom2, top2;
			if (Const::showTiles) {
				// если нужно показывать обычные тайлы
				left1 = left2 = top1 = top2 = 0.03f;
				right1 = right2 = bottom1 = bottom2 = 0.97f;
				if (xPosition % Const::regionCellSize == 0) left1 = left2 = 0.0f;
				if ((xPosition + 1) % Const::regionCellSize == 0) right1 = right2 = 1.0f;
				if ((yPosition + 1) % Const::regionCellSize == 0) top1 = top2 = 0.0f;
				if (yPosition % Const::regionCellSize == 0) bottom1 = bottom2 = 1.0f;
			}
			// стандартный режим
			else {
				// правый верхний угол (лес)
				left2 = (xPosition % Const::tilesInSquadMeadow) * halfMeadowSquadInTile + 0.5f;
				right2 = left2 + halfMeadowSquadInTile;
				bottom2 = (yPosition % Const::tilesInSquadMeadow) * meadowSquadInTile * 0.5f;
				top2 = bottom2 + meadowSquadInTile * 0.5f;
				if (left2 == 0.5f) left2 = 0.5f + dX;
				if (right2 == 1.0f) right2 = 1.0f - dX;
				if (bottom2 == 0.0f) bottom2 = dX;
				if (top2 == 0.5f) top2 = 0.5f - dX;
				if (regionHeightMap[i + 1][j + 1] < ((FOREST_SWAMP_BORDER + FOREST_MEADOW_BORDER) * 0.5f)) {
					// левый нижний угол (болото)
					left1 = (xPosition % Const::tilesInSquadSwamp) * halfSwampSquadInTile;
					right1 = left1 + halfSwampSquadInTile;
					bottom1 = (yPosition % Const::tilesInSquadSwamp) * halfSwampSquadInTile + 0.5f;
					top1 = bottom1 + halfSwampSquadInTile;
					if (top1 == 1.0f) top1 = 1.0f - dX;
					if (bottom1 == 0.5f) bottom1 = 0.5f + dX;
					if (left1 == 0.0f) left1 = dX;
					if (right1 == 0.5f) right1 = 0.5f - dX;
				}
				else {
					// левый верхний угол (луг)
					left1 = (xPosition % Const::tilesInSquadForest) * halfForestSquadInTile;
					right1 = left1 + halfForestSquadInTile;
					bottom1 = (yPosition % Const::tilesInSquadForest) * forestSquadInTile * 0.5f;
					top1 = bottom1 + forestSquadInTile * 0.5f;
					if (top1 == 0.5f) top1 = 0.5f - dX;
					if (bottom1 == 0.0f) bottom1 = dX;
					if (left1 == 0.0f) left1 = dX;
					if (right1 == 0.5f) right1 = 0.5f - dX;
				}

			}

			// для каждого квадрата
			VertTerrain vert4[4] = {

				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 1], (float)j), regionNormalMap[i][j], regionTangentMap[i][j], XMFLOAT2(left1, bottom1), XMFLOAT2(left2, bottom2), maskTile[0] },
				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 2], j + 1.0f), regionNormalMap[i][j + 1], regionTangentMap[i][j + 1], XMFLOAT2(left1, top1), XMFLOAT2(left2, top2), maskTile[1] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 1], (float)j), regionNormalMap[i + 1][j], regionTangentMap[i + 1][j], XMFLOAT2(right1, bottom1), XMFLOAT2(right2, bottom2), maskTile[2] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 2], j + 1.0f), regionNormalMap[i + 1][j + 1], regionTangentMap[i + 1][j + 1], XMFLOAT2(right1, top1), XMFLOAT2(right2, top2), maskTile[3] },

			};

			memcpy(pointer, vert4, sizeof(VertTerrain)* 4);
			pointer += 4;

		};

	// изменить динамический буфер
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)); // экспериментировать
	VertTerrain* v = reinterpret_cast<VertTerrain*>(mappedData.pData);
	memcpy(v, vertexBuffer, sizeof(VertTerrain)* Const::regionVertexAmount);
	Mediator::pDevContext->Unmap(pVB, 0);

	return true;

};

// создать буфер вершин для локации входа (вторая)
bool DynamicReg::CreateVBLocation1() {

	// вспомогательные переменные
	VertTerrain* pointer = vertexBuffer;
	const float squadInTile = 0.5f;
	const float halfSquadInTile = 0.25f;

	// создание карты нормалей
	CreateMaps();

	// содержимое буфера
	for (int i(0); i < Const::regionCellSize; i++)
		for (int j(0); j < Const::regionCellSize; j++) {

			// определение текстурных координат маски смешивания
			XMFLOAT2* maskTile = emptyMask;

			// определение полных координат тайлов
			UINT xPosition = regionAdress.x * Const::regionCellSize + i;
			UINT yPosition = regionAdress.y * Const::regionCellSize + j;

			// определение текстурных координат тайлов
			float left1, right1, bottom1, top1;
			if (Const::showTiles) {
				// если нужно показывать обычные тайлы
				left1 = top1 = 0.03f;
				right1 = bottom1 = 0.97f;
				if (xPosition % Const::regionCellSize == 0) left1 = 0.0f;
				if ((xPosition + 1) % Const::regionCellSize == 0) right1 = 1.0f;
				if ((yPosition + 1) % Const::regionCellSize == 0) top1 = 0.0f;
				if (yPosition % Const::regionCellSize == 0) bottom1 = 1.0f;
			}
			// стандартный режим
			else {
				// левый верхний угол 
				left1 = (xPosition % Const::tilesInSquadForest) * halfSquadInTile;
				right1 = left1 + halfSquadInTile;
				bottom1 = (yPosition % Const::tilesInSquadForest) * squadInTile * 0.5f;
				top1 = bottom1 + squadInTile * 0.5f;
				if (top1 == 0.5f) top1 = 0.5f - dX;
				if (bottom1 == 0.0f) bottom1 = dX;
				if (left1 == 0.0f) left1 = dX;
				if (right1 == 0.5f) right1 = 0.5f - dX;

			}

			// для каждого квадрата
			VertTerrain vert4[4] = {

				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 1], (float)j), regionNormalMap[i][j], regionTangentMap[i][j], XMFLOAT2(left1, bottom1), XMFLOAT2(), maskTile[0] },
				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 2], j + 1.0f), regionNormalMap[i][j + 1], regionTangentMap[i][j + 1], XMFLOAT2(left1, top1), XMFLOAT2(), maskTile[1] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 1], (float)j), regionNormalMap[i + 1][j], regionTangentMap[i + 1][j], XMFLOAT2(right1, bottom1), XMFLOAT2(), maskTile[2] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 2], j + 1.0f), regionNormalMap[i + 1][j + 1], regionTangentMap[i + 1][j + 1], XMFLOAT2(right1, top1), XMFLOAT2(), maskTile[3] },

			};

			memcpy(pointer, vert4, sizeof(VertTerrain)* 4);
			pointer += 4;

		};

	// изменить динамический буфер
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)); // экспериментировать
	VertTerrain* v = reinterpret_cast<VertTerrain*>(mappedData.pData);
	memcpy(v, vertexBuffer, sizeof(VertTerrain)* Const::regionVertexAmount);
	Mediator::pDevContext->Unmap(pVB, 0);

	return true;

};

// создать буфер вершин для тестовой локации
bool DynamicReg::CreateVBTestLocation() {

	// вспомогательные переменные
	VertTerrain* pointer = vertexBuffer;
	const float squadInTile = 0.5f;
	const float halfSquadInTile = 0.25f;

	// создание карты высот и нормали
	CreateMaps();

	// содержимое буфера
	for (int i(0); i < Const::regionCellSize; i++)
		for (int j(0); j < Const::regionCellSize; j++) {

			// определение текстурных координат маски смешивания
			XMFLOAT2* maskTile = emptyMask;

			// определение полных координат тайлов
			UINT xPosition = regionAdress.x * Const::regionCellSize + i;
			UINT yPosition = regionAdress.y * Const::regionCellSize + j;

			// определение текстурных координат тайлов
			float left1, right1, bottom1, top1;

			// если нужно показывать обычные тайлы
			left1 = top1 = 0.03f;
			right1 = bottom1 = 0.97f;
			if (xPosition % Const::regionCellSize == 0) left1 = 0.0f;
			if ((xPosition + 1) % Const::regionCellSize == 0) right1 = 1.0f;
			if ((yPosition + 1) % Const::regionCellSize == 0) top1 = 0.0f;
			if (yPosition % Const::regionCellSize == 0) bottom1 = 1.0f;

			// для каждого квадрата
			VertTerrain vert4[4] = {

				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 1], (float)j), regionNormalMap[i][j], regionTangentMap[i][j], XMFLOAT2(left1, bottom1), XMFLOAT2(), maskTile[0] },
				{ XMFLOAT3((float)i, regionHeightMap[i + 1][j + 2], j + 1.0f), regionNormalMap[i][j + 1], regionTangentMap[i][j + 1], XMFLOAT2(left1, top1), XMFLOAT2(), maskTile[1] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 1], (float)j), regionNormalMap[i + 1][j], regionTangentMap[i + 1][j], XMFLOAT2(right1, bottom1), XMFLOAT2(), maskTile[2] },
				{ XMFLOAT3(i + 1.0f, regionHeightMap[i + 2][j + 2], j + 1.0f), regionNormalMap[i + 1][j + 1], regionTangentMap[i + 1][j + 1], XMFLOAT2(right1, top1), XMFLOAT2(), maskTile[3] },

			};

			memcpy(pointer, vert4, sizeof(VertTerrain)* 4);
			pointer += 4;

		};

	// изменить динамический буфер
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)); // экспериментировать
	VertTerrain* v = reinterpret_cast<VertTerrain*>(mappedData.pData);
	memcpy(v, vertexBuffer, sizeof(VertTerrain)* Const::regionVertexAmount);
	Mediator::pDevContext->Unmap(pVB, 0);

	return true;

}

// создать карту нормалей по карте высот
void DynamicReg::CreateMaps() {

	// создание карты высот
	for (int i(0); i < Const::regionCellSize + 3; i++) {
		for (int j(0); j < Const::regionCellSize + 3; j++) {
			NumXY<float> worldPosition(regionAdress.x * (float)Const::regionCellSize + i - 1.0f, regionAdress.y * (float)Const::regionCellSize + j - 1.0f);
			regionHeightMap[i][j] = Mediator::superFunct->HeightFunction(worldPosition);
		}
	}

	// создание карты нормалей
	for (int i(0); i < Const::regionCellSize + 1; i++) {
		for (int j(0); j < Const::regionCellSize + 1; j++) {
			float grad1 = regionHeightMap[i + 2][j + 1] - regionHeightMap[i][j + 1];
			float grad2 = regionHeightMap[i + 1][j + 2] - regionHeightMap[i + 1][j];
			XMVECTOR vector1 = { 1.0f, grad1, 0.0f };
			XMVECTOR vector2 = { 0.0f, grad2, 1.0f };
			XMVECTOR vectorNormal = -XMVector3Normalize(XMVector3Cross(vector1, vector2));
			XMFLOAT3 vectorNormalF;
			XMStoreFloat3(&vectorNormalF, vectorNormal);
			regionNormalMap[i][j] = vectorNormalF;
			regionTangentMap[i][j] = XMFLOAT3(1.0f, regionHeightMap[i + 2][j + 1] - regionHeightMap[i + 1][j + 1], 0.0f);

		}
	}

}