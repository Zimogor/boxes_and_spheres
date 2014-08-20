#include "Map.h"
#include "Mediator.h"

int Patch::indicesAmount = 0; // количество индексов

// конструктор
Patch::Patch(NumXY<int> bottomLeftCorner, bool* result) {

	// переменные
	int vertNumber = 0;
	verticesData = new VertMap[patchVertSize * patchVertSize];

	// создать данные для патча
	for (int i(bottomLeftCorner.x); i < patchVertSize + bottomLeftCorner.x; i++)
		for (int j(bottomLeftCorner.y); j < patchVertSize + bottomLeftCorner.y; j++) {

			verticesData[vertNumber].color = XMFLOAT3(0.0f, 0.0f, 0.0f);
			verticesData[vertNumber].position = XMFLOAT3((float)i + 0.5f, (float)j + 0.5f, Const::mapDepth);
			vertNumber++;
		}

	// создание буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(VertMap)* patchVertSize * patchVertSize;
	bufferVertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = (void*)verticesData;
	HRR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

}

// обновить содержимое патча
bool Patch::Recreate() {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	VertMap* VB = reinterpret_cast<VertMap*>(mappedData.pData);
	memcpy(VB, verticesData, sizeof(VertMap)* patchVertSize * patchVertSize);
	Mediator::pDevContext->Unmap(pVertexBuffer, 0);

	return true;

}

// рисование
bool Patch::Draw() {

	UINT vertexStride = sizeof(VertMap);
	UINT vertexOffset = 0;
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// деструктор
Patch::~Patch() {

	ReleaseCOM(pVertexBuffer);
	ReleaseNULLS(verticesData);

}

// конструктор
Map::Map(bool* result) {

	// проверка правильности данных
	BRR(Const::patchCellSize  % Const::regionCellSize == 0);

	// переменные
	int indicesAmount = Const::patchCellSize  * Const::patchCellSize * 6;
	UINT* indicesData = new UINT[indicesAmount];
	int vertNumber = 0;
	int indNumber = 0;

	// заполнисть статические переменные
	Patch::FillStatic(indicesAmount);

	// инициализация индексов
	for (int i(0); i < Const::patchCellSize; i++)
		for (int j(0); j < Const::patchCellSize; j++) {
			indicesData[indNumber + 0] = 0 + j + i * patchVertSize;
			indicesData[indNumber + 1] = 1 + j + i * patchVertSize;
			indicesData[indNumber + 2] = patchVertSize + 0 + j + i * patchVertSize;
			indicesData[indNumber + 3] = patchVertSize + 0 + j + i * patchVertSize;
			indicesData[indNumber + 4] = 1 + j + i * patchVertSize;
			indicesData[indNumber + 5] = patchVertSize + 1 + j + i * patchVertSize;
			indNumber += 6;
		}

	// создание буфера индексов
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT)* indicesAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indicesData;
	HRR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	// удаление данных
	ReleaseNULLS(indicesData);

	// создание поля указателей патчей
	patchField = new Patch**[Const::patchFieldSize];
	for (UINT i(0); i < Const::patchFieldSize; i++) {
		patchField[i] = new Patch*[Const::patchFieldSize];
		for (UINT j(0); j < Const::patchFieldSize; j++) {
			patchField[i][j] = NULL;
		}
	}

	// ??? пробный патч
	patchField[0][0] = new Patch(NumXY<int>(), result);

	Mediator::map = this;

}

// обновление
bool Map::Update() {

	if (newRegions.size()) {

		// вставить новую территорию в карту
		for (UINT regionNumber(0); regionNumber < newRegions.size(); regionNumber++) {

			// ???
			bool result;
			NumXY<int>patchAdress = MathHelper::GetPatchAdress(newRegions[regionNumber]->regionAdress);
			//Mediator::console->Push(L"region adress ", newRegions[regionNumber]->regionAdress);
			//Mediator::console->Push(L"patch adress ", patchAdress);
			if (!patchField[patchAdress.x][patchAdress.y]) {
				NumXY<int> regAdr = newRegions[regionNumber]->regionAdress;
				regAdr.x = ((float)regAdr.x / (patchAdress.x - Const::patchFieldSize));
				regAdr.y = ((float)regAdr.y / (patchAdress.y - Const::patchFieldSize));
				NumXY<int> patchCorner(regAdr.x * Const::regionCellSize, regAdr.y * Const::regionCellSize);
				patchField[patchAdress.x][patchAdress.y] = new Patch(patchCorner, &result);
				Mediator::console->Push(L"patch number ", patchAdress);
				Mediator::console->Push(L"patch corner ", patchCorner);
			}

			// заменить новые цвета
			for (int i(0); i < Const::regionCellSize; i++)
				for (int j(0); j < Const::regionCellSize; j++) {

					// различные параметры
					int xAbsAdress = newRegions[regionNumber]->regionAdress.x * Const::regionCellSize + i;
					int yAbsAdress = newRegions[regionNumber]->regionAdress.y * Const::regionCellSize + j;
					if (xAbsAdress > Const::patchCellSize || yAbsAdress > Const::patchCellSize) continue;
					if (xAbsAdress < 0 || yAbsAdress < 0) continue;
					int patchIndex = xAbsAdress * patchVertSize + yAbsAdress;
					CellFiller content = newRegions[regionNumber]->getContent()[i][j];
					VertMap* verticesData = patchField[0][0]->verticesData;

					// изменение значений вершин карты
					switch (content.biomType) {

					case B_FOREST:
						switch (content.item) {
						case CI_BOLDFIR: case CI_FIR: verticesData[patchIndex].color = XMFLOAT3(0.30f, 0.26f, 0.17f); break;
						default: verticesData[patchIndex].color = XMFLOAT3(0.40f, 0.34f, 0.22f); break;
						}
						break;
					case B_MEADOW:
						switch (content.item) {
						case CI_TREE: verticesData[patchIndex].color = XMFLOAT3(0.35f, 0.50f, 0.17f); break;
						default: verticesData[patchIndex].color = XMFLOAT3(0.45f, 0.63f, 0.25f); break;
						}
						break;
					case B_SWAMP:
						verticesData[patchIndex].color = XMFLOAT3(0.18f, 0.23f, 0.10f);
						break;
					default:
						verticesData[patchIndex].color = XMFLOAT3(1.0f, 1.0f, 1.0f);
						break;
					}
				} // для каждой ячейки
		} // для каждого нового региона

		BR(patchField[0][0]->Recreate()); // обновить патч

	}

	newRegions.clear();
	return true;

}

// рисование
bool Map::Draw() {

	UINT vertexStride = sizeof(VertMap);
	UINT vertexOffset = 0;

	// установить параметры
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPosCol);
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosCol, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSPosCol, NULL, 0);

	// ???
	int x = 0;
	int y = 0;
	int z = 0;
	x = (int)Mediator::winDimentions->x / 2 - (int)Mediator::hero->GetPosition().x;
	y = (int)Mediator::winDimentions->y / 2 - (int)Mediator::hero->GetPosition().z;

	XMMATRIX world = XMMatrixTranslation((float)x, (float)y, (float)z);
	Mediator::shader->SetObjectBuffer(world);

	// рисование
	BR(patchField[0][0]->Draw()); // ???

	return true;

}

// создался новый регион
bool Map::NewRegion(DynamicReg* dynamicReg) {

	newRegions.push_back(dynamicReg);
	return true;

}

// деструктор
Map::~Map() {

	ReleaseCOM(pIndexBuffer);

	for (UINT i(0); i < Const::patchFieldSize; i++) {
		for (UINT j(0); j < Const::patchFieldSize; j++) {
			ReleaseNULL(patchField[i][j]);
		}
		ReleaseNULLS(patchField[i]);
	}
	ReleaseNULL(patchField);

}