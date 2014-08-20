#include "Terrain.h"
#include "Mediator.h"
#include "Hero.h"

// ������ ������ ������ ������� � ��������
wchar_t totalNameDDS[256]; // �������
wchar_t totalNameBin[256]; // ��������

// ����������� ����������
Terrain::Terrain(int location, bool* result) {

	showPoint = NumXY<float>(Const::spawnPoint.x, Const::spawnPoint.z);
	wchar_t* locationName = NULL;

	// �������� ��������
	switch (location) {
	case 0: locationName = L"resources//locations//location0.dds"; break;
	case 1: locationName = L"resources//locations//location1.dds"; break;
	}
	if (Const::testFileName) {
		chopableFrom = chopableTo = pickableFrom = pickableTo = -1;
		BRR(GenerateTestLocation());
	}
	else {
		switch (location) {
		case 0:
			chopableFrom = CHOPABLE_FROM_L1;
			chopableTo = CHOPABLE_TO_L1;
			pickableFrom = PICKABLE_FROM_L1;
			pickableTo = PICKABLE_TO_L1;
			BRR(GenerateLocation0());
			break;
		case 1:
			chopableFrom = CHOPABLE_FROM_L2;
			chopableTo = CHOPABLE_TO_L2;
			pickableFrom = PICKABLE_FROM_L2;
			pickableTo = PICKABLE_TO_L2;
			BRR(GenerateLocation1());
			break;
		}
	}

	// ������������� �������
	if (Const::showTiles) {
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//locations//simpleTiles.png", NULL, NULL, &pSRtexture, NULL));
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//locations//simpleNormals.dds", NULL, NULL, &pSRnormalMap, NULL));
	}
	else {
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, locationName, NULL, NULL, &pSRtexture, NULL));
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//locations//location0normals.dds", NULL, NULL, &pSRnormalMap, NULL));
	}
	Mediator::pDevContext->PSSetShaderResources(NORMALMAP_BUFFER, 1, &pSRnormalMap);

	// �������� ������ ��������
	BRR(CreateIndexBuffer());

	// �������� ������� ������ ������ �����������
	BRR(CreateInstanceBuffer());

	// ���������� ������ ������
	vertexBuffer = new VertTerrain[Const::regionVertexAmount];

	// �������� ����� ����� ��� ��������
	regionHeightMap = new float*[Const::regionCellSize + 3];
	for (int i(0); i < Const::regionCellSize + 3; i++) {
		regionHeightMap[i] = new float[Const::regionCellSize + 3];
	}

	// �������� ����� �������� � ��������� ��� ��������
	regionNormalMap = new XMFLOAT3*[Const::regionCellSize + 1];
	regionTangentMap = new XMFLOAT3*[Const::regionCellSize + 1];
	for (int i(0); i < Const::regionCellSize + 1; i++) {
		regionNormalMap[i] = new XMFLOAT3[Const::regionCellSize + 1];
		regionTangentMap[i] = new XMFLOAT3[Const::regionCellSize + 1];
	}

	// ���������� ����������� ��� ������� �������
	vectInstanceAmount = new UINT[TerrainVectorModels.size()];
	ZeroMemory(vectInstanceAmount, sizeof(UINT)* TerrainVectorModels.size());

	// ������������� ����������� ���������� ��������
	DynamicReg::FillStatic(location, regionHeightMap, regionNormalMap, regionTangentMap, vertexBuffer, vectInstanceAmount, TerrainVectorModels);

	// ������� ������� ������������ �������
	dynamicReg = new DynamicReg[dynamicVBAmount];

	// ��������� �������� ����� ��������
	int leftBorder = (int)floor((showPoint.x - Const::showRadius) / Const::regionCellSize);
	int rightBorder = (int)floor((showPoint.x + Const::showRadius) / Const::regionCellSize);
	int bottomBorder = (int)floor((showPoint.y - Const::showRadius) / Const::regionCellSize);
	int topBorder = (int)floor((showPoint.y + Const::showRadius) / Const::regionCellSize);
	for (int i(leftBorder); i <= rightBorder; i++)
		for (int j(bottomBorder); j <= topBorder; j++) {
			myXMFLOAT3 regCenter = myXMFLOAT3((i + 0.5f) * Const::regionCellSize, 0.0f, (j + 0.5f) * Const::regionCellSize);
			regCenter.y = Mediator::superFunct->HeightFunction(regCenter.x, regCenter.z);
			if (MathHelper::Distance(showPoint, NumXY<float>(regCenter.x, regCenter.z)) <= Const::showRadius) {

				// ����� ���������� ������������� �������
				UINT freeReg;
				BRR(FindFreeRegion(&freeReg));
				// ��������� ������
				BRR(dynamicReg[freeReg].Fill(NumXY<int>(i, j), regCenter));

			}
		}

	// �������� ���� ���������� ��������
	Const::regionFieldSize = UINT(Const::showRadius * 2 / Const::regionCellSize + 3);
	regionField = new DynamicReg**[Const::regionFieldSize];
	for (int i(0); i < Const::regionFieldSize; i++) {
		regionField[i] = new DynamicReg*[Const::regionFieldSize];
	}
	BRR(RegenerateRegionField());

	// ��������� ������ �����������
	instStarts = new UINT[TerrainVectorModels.size()];
	instAmounts = new UINT[TerrainVectorModels.size()];

	// ������� ����������
	Mediator::regionField = regionField;
	Mediator::cornerPoint = &cornerPoint;
	Mediator::terrain = this;
	*result = true;

}

// ������� ����� ��������
wchar_t* Terrain::MakeDDSAdress(wchar_t* name) {

	const wchar_t prefix[] = L"resources\\textures\\";
	const wchar_t postfix[] = L".dds";
	wcscpy_s(totalNameDDS, prefix);
	wcscat_s(totalNameDDS, name);
	wcscat_s(totalNameDDS, postfix);
	return totalNameDDS;

}

// ������� ����� ������
wchar_t* Terrain::MakeBinAdress(wchar_t* name) {

	const wchar_t prefix[] = L"resources\\static_models\\";
	const wchar_t postfix[] = L".bin";
	wcscpy_s(totalNameBin, prefix);
	wcscat_s(totalNameBin, name);
	wcscat_s(totalNameBin, postfix);
	return totalNameBin;

}

// ������� ������� �� ������� ���������, true - ���� �������
bool Terrain::RemoveFromVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance* modelParams) {
	return TerrainVectorModels[item]->RemoveElementFromVector(regAdress, cellAdress, modelParams);
}

// �������� ������� � ������ ���������
//bool Terrain::AddToVector(CELL_ITEM item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance modelParams) {
bool Terrain::AddToVector(int item, NumXY<int> regAdress, NumXY<int> cellAdress, PerModelInstance modelParams) {
	return TerrainVectorModels[item]->AddElementToVector(regAdress, cellAdress, modelParams);
}

// ������������� ���� ����������
bool Terrain::RegenerateRegionField() {

	// �������� ����
	for (int i(0); i < Const::regionFieldSize; i++)
		for (int j(0); j < Const::regionFieldSize; j++) {
			regionField[i][j] = NULL;
		}

	// ������������� ���� ���� � ����������� ��������
	NumXY<int> cornerPoint;
	cornerPoint.x = (int)floor((showPoint.x - Const::showRadius) / Const::regionCellSize);
	cornerPoint.y = (int)floor((showPoint.y - Const::showRadius) / Const::regionCellSize);

	// ����������� ����������
	for (UINT i(0); i < dynamicVBAmount; i++)
		if (dynamicReg[i].busy) {
			NumXY<int> curAdress = dynamicReg[i].regionAdress;
			int x = (int)floor(curAdress.x - cornerPoint.x);
			int y = (int)floor(curAdress.y - cornerPoint.y);
			regionField[x][y] = &dynamicReg[i];
		}

	return true;

}

// �������� ������ �� ���������� ���������� ������ (���������� ������������� ����� ������� � ����� ������)
CellFiller* Terrain::GetCellFiller(myXMFLOAT3 position, NumXY<int>* cellAdress, NumXY<int>* regAdress) {

	NumXY<int> regAdr = MathHelper::GetDynamicRegionAdress(position);
	NumXY<int> cellAdr = MathHelper::GetContentAdress(position);
	if (cellAdress) *cellAdress = cellAdr;
	if (regAdress) *regAdress = regAdr;

	return &regionField[regAdr.x][regAdr.y]->getContent()[cellAdr.x][cellAdr.y];

}

// �������� ������ ��������
bool Terrain::CreateIndexBuffer() {

	UINT* indices = new UINT[Const::regionIndexAmount];
	UINT* pointer = indices;
	UINT counter = 0;

	for (int i(0); i < Const::regionCellSize; i++)
		for (int j(0); j < Const::regionCellSize; j ++) {

			// ��� ������� ��������
			UINT index6[6] = { counter, counter + 1, counter + 2, counter + 2, counter + 1, counter + 3 };
			memcpy(pointer, index6, sizeof(UINT)* 6);
			counter += 4;
			pointer += 6;

		}

	// ��������� ������ ��������
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT)* Const::regionIndexAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ����� ��������
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indices;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	delete[] indices;

	return true;

}

// ������� ������ ����� ����� �����������
bool Terrain::CreateInstanceBuffer() {

	// ������� ����� �����
	PerModelInstance* instances = new PerModelInstance[INSTANCE_BUFFER_MODEL_SIZE];
	ZeroMemory(instances, sizeof(PerModelInstance)* INSTANCE_BUFFER_MODEL_SIZE);

	// ��������� ������ ��������
	D3D11_BUFFER_DESC bufferInstDesc = { 0 };
	bufferInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInstDesc.ByteWidth = sizeof(PerModelInstance)* INSTANCE_BUFFER_MODEL_SIZE;
	bufferInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferInstDesc.MiscFlags = 0;
	bufferInstDesc.StructureByteStride = 0;
	bufferInstDesc.Usage = D3D11_USAGE_DYNAMIC;

	// �������� ����� ��������
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = instances;
	HR(Mediator::pDev->CreateBuffer(&bufferInstDesc, &subIndData, &pInstanceBuffer));

	ReleaseNULLS(instances);

	return true;

}

// ����� ��������� ������
bool Terrain::FindFreeRegion(UINT* freeReg) {

	for (UINT i(0); i < dynamicVBAmount; i++) {
		if (dynamicReg[i].busy == false) {
			*freeReg = i;
			return true;
		}
	}
	return false;

}

// ���������� ����������
bool Terrain::Update(XMFLOAT3 camFocusPosition) {

	static float prevCamPositionX = camFocusPosition.x;
	static float prevCamPositionZ = camFocusPosition.z;

	// ���������� �������� ���� ��������
	if (abs(camFocusPosition.x - prevCamPositionX) > Const::updateDistance || abs(camFocusPosition.z - prevCamPositionZ) > Const::updateDistance) {

		prevCamPositionX = camFocusPosition.x;
		prevCamPositionZ = camFocusPosition.z;
		BR(UpdateCurrentRegions(NumXY<float>(camFocusPosition.x, camFocusPosition.z)));

	}

	// ������������� ���� ���� � ����������� �������
	cornerPoint.x = (int)floor((showPoint.x - Const::showRadius) / Const::regionCellSize);
	cornerPoint.y = (int)floor((showPoint.y - Const::showRadius) / Const::regionCellSize);

	return true;

}

// �������� �������� �������
bool Terrain::UpdateCurrentRegions(NumXY<float> newShowPoint) {

	// �������� ������ ��������
	for (UINT i(0); i < dynamicVBAmount; i++) {
		if (dynamicReg[i].busy) {
			if (MathHelper::Distance(newShowPoint, NumXY<float>(dynamicReg[i].regionCenter.x, dynamicReg[i].regionCenter.z)) > Const::showRadius)	{

				// ������� ������ �������
				for (auto iter = TerrainVectorModels.begin(); iter != TerrainVectorModels.end(); iter++) {
					(*iter)->EraseByRegion(dynamicReg[i].regionAdress);
				}

				// ��������� ������������ �����
				dynamicReg[i].busy = false;

			}
		}
	}

	// �������� ����� ��������
	int leftBorder = (int)floor((newShowPoint.x - Const::showRadius) / Const::regionCellSize);
	int rightBorder = (int)floor((newShowPoint.x + Const::showRadius) / Const::regionCellSize);
	int bottomBorder = (int)floor((newShowPoint.y - Const::showRadius) / Const::regionCellSize);
	int topBorder = (int)floor((newShowPoint.y + Const::showRadius) / Const::regionCellSize);

	for (int i(leftBorder); i <= rightBorder; i++)
		for (int j(bottomBorder); j <= topBorder; j++) {
			myXMFLOAT3 regCenter = myXMFLOAT3((i + 0.5f) * Const::regionCellSize, 0.0f, (j + 0.5f) * Const::regionCellSize);
			regCenter.y = Mediator::superFunct->HeightFunction(regCenter.x, regCenter.z);
			if (MathHelper::Distance(showPoint, NumXY<float>(regCenter.x, regCenter.z)) > Const::showRadius) {
				if (MathHelper::Distance(newShowPoint, NumXY<float>(regCenter.x, regCenter.z)) < Const::showRadius) {

					// ����� ���������� ������������� �������
					UINT freeReg;
					BR(FindFreeRegion(&freeReg));
					// ��������� ������
					BR(dynamicReg[freeReg].Fill(NumXY<int>(i, j), regCenter));

				}
			}
		}

	showPoint = newShowPoint;

	// ����������� ���� ��������
	BR(RegenerateRegionField());

	return true;

}

// ��������� ����������
bool Terrain::Draw(bool toShadow) {

	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutInstance);

	if (toShadow) {

		// ����������� ����� ���� ��������
		BR(FillInstanceBuffer(MODEL_RADIUS + 3.0f)); // ���������� ��� �����

		// ���������� ��������� �������
		Mediator::pDevContext->VSSetShader(Mediator::pVSShadowInstance, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSEmpty, NULL, 0);

		// ��������� �������� �������
		for (UINT i(0); i < TerrainVectorModels.size(); i++) {
			if (instAmounts[i]) TerrainVectorModels[i]->Draw(true, pInstanceBuffer, instStarts[i], instAmounts[i]);
		}

	}
	else {

		// ����������� ����� ���� ��������
		BR(FillInstanceBuffer(MODEL_RADIUS));

		// ���������� ��������� �������
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutInstance);
		Mediator::pDevContext->VSSetShader(Mediator::pVSInstance, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSClipping, NULL, 0);

		// ��������� �������� �������
		for (UINT i(0); i < TerrainVectorModels.size(); i++) {
			if (instAmounts[i]) TerrainVectorModels[i]->Draw(false, pInstanceBuffer, instStarts[i], instAmounts[i]);
		}

		// ���������� ��������� ��������
		Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutTerrain);
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
		Mediator::pDevContext->VSSetShader(Mediator::pVSTerrain, NULL, 0);
		if (Const::shadowsOn) Mediator::pDevContext->PSSetShader(Mediator::pPSTerrain, NULL, 0);
		else (Mediator::pDevContext->PSSetShader(Mediator::pPSTerrainNoShadow, NULL, 0));

		// ��������� ��������
		for (UINT i(0); i < dynamicVBAmount; i++) {
			if (dynamicReg[i].busy) {
				if (!Mediator::camera->GetFrustum()->CheckSphere(dynamicReg[i].regionCenter, 0.8f * Const::regionCellSize)) continue;
				BR(dynamicReg[i].Draw());
			}
		}

	}

	return true;

}

// ����������� ����� ���� �������� � ������ frustumCulling
bool Terrain::FillInstanceBuffer(float frustumRadius) {

	UINT instNumber = 0;
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	PerModelInstance* model = reinterpret_cast<PerModelInstance*>(mappedData.pData);

	// ���������� ������ ������
	for (UINT i(0); i < TerrainVectorModels.size(); i++) {
		UINT amount = 0;
		instStarts[i] = instNumber;
		for (UINT j(0); j < TerrainVectorModels[i]->GetElementsAmount(); j++) {
			if (Mediator::camera->GetFrustum()->CheckSphere(TerrainVectorModels[i]->GetPosition(j), frustumRadius)) {
				memcpy(model, TerrainVectorModels[i]->GetPointer(j), sizeof(PerModelInstance));
				model++;
				instNumber++;
				amount++;
			}
		}
		instAmounts[i] = amount;
	}

	Mediator::pDevContext->Unmap(pInstanceBuffer, 0);

	return true;

}

// ���� ������� � �����
bool Terrain::TakePick(WIELD_EVENTS events, myXMFLOAT3 damagePoint) {

	NumXY<int> cellAdr;
	CellFiller* cellFiller = GetCellFiller(damagePoint, &cellAdr);
	
	switch (events) {

	case E_PICKAXE: // ���� ������

		if (cellFiller->item != CI_BOULDER) break;
		Mediator::helper->SetFloatText(damagePoint, L"���!", true, XMFLOAT3(1.0f, 1.0f, 0.0f));

		// ������ ��������
		if ((cellFiller->additional -= 1) == 0) {

			NumXY<int> absRegAdr = MathHelper::GetAbsRegionAdress(damagePoint);
			Mediator::terrain->RemoveFromVector(cellFiller->item, absRegAdr, cellAdr);
			*cellFiller = CellFiller(CI_NONE, 0);

			// ������ � ���������
			Mediator::hero->ThrustIntoInventory(InventoryContent(II_ROCK, 1));
		}
		break;

	case E_CHOP: // ���� �������

		if (cellFiller->item < chopableFrom || cellFiller->item > chopableTo) break;
		Mediator::helper->SetFloatText(damagePoint, L"�����!", true, XMFLOAT3(1.0f, 1.0f, 0.0f));

		// ������ ���������
		if ((cellFiller->additional -= 1) == 0) {

			NumXY<int> absRegAdr = MathHelper::GetAbsRegionAdress(damagePoint);
			PerModelInstance modelParams;
			Mediator::terrain->RemoveFromVector(cellFiller->item, absRegAdr, cellAdr, &modelParams);
			Mediator::terrain->AddToVector(CI_STUMP, absRegAdr, cellAdr, modelParams);
			*cellFiller = CellFiller(CI_STUMP, 0);

			// ������ � ���������
			Mediator::hero->ThrustIntoInventory(InventoryContent(II_LOG, 1));
		}
		break;
	}

	return true;

}

// ���������� (true - ���� ��������)
bool Terrain::TakePickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult) {

	NumXY<int> cellAdr;
	float minDistance = FLT_MAX;
	CellFiller* minCellFiller = NULL;
	myXMFLOAT3 minPosition;
	NumXY<int> minCellAdr;

	// ����� �� ��������� �������
	for (int i(-1); i <= 1; i++)
		for (int j(-1); j <= 1; j++) {

			myXMFLOAT3 testedPosition = myXMFLOAT3(position.x + i, position.y, position.z + j);
			CellFiller* cellFiller = GetCellFiller(testedPosition, &cellAdr);
			if (cellFiller->item >= pickableFrom && cellFiller->item <= pickableTo) {

				float distance = MathHelper::Distance(cellFiller->itemCenter, NumXY<float>(position.x, position.z));
				if (minDistance >= distance) {
					minDistance = distance;
					minCellFiller = cellFiller;
					minPosition = testedPosition;
					minCellAdr = cellAdr;
				}
			}
		}

	// ����� ���-��
	if (minCellFiller && minDistance <= Const::distanceToPick) {

		NumXY<int> absRegAdr = MathHelper::GetAbsRegionAdress(minPosition);
		Mediator::terrain->RemoveFromVector(minCellFiller->item, absRegAdr, minCellAdr);
		*cellItemResult = minCellFiller->item;
		minCellFiller->item = CI_NONE;

		return true;

	}

	return false;
	
}

// ���������� ����������
Terrain::~Terrain() {

	// �������� ���� ���������� ��������
	if (regionField) {
		for (int i(0); i < Const::regionFieldSize; i++) {
			delete[] regionField[i];
		}
		ReleaseNULLS(regionField);
	}

	// �������� ����� �����
	if (regionHeightMap) {
		for (int i(0); i < Const::regionCellSize + 3; i++) {
			ReleaseNULLS(regionHeightMap[i]);
		}
		ReleaseNULLS(regionHeightMap);
	}

	// �������� ����� ��������
	if (regionNormalMap) {
		for (int i(0); i < Const::regionCellSize + 1; i++) {
			ReleaseNULLS(regionNormalMap[i]);
		}
		ReleaseNULLS(regionNormalMap);
	}

	// �������� ����� ���������
	if (regionTangentMap) {
		for (int i(0); i < Const::regionCellSize + 1; i++) {
			ReleaseNULLS(regionTangentMap[i]);
		}
		ReleaseNULLS(regionTangentMap);
	}

	// ���������� ������ ������
	ReleaseNULLS(vertexBuffer);

	// �������� ������������ �������
	ReleaseNULLS(dynamicReg);

	// �������� �������� �������
	for (UINT i(0); i < TerrainVectorModels.size(); i++) {
		ReleaseNULL(TerrainVectorModels[i]);
	}

	// ������ �������
	delete[] vectInstanceAmount;
	ReleaseNULL(instStarts);
	ReleaseNULL(instAmounts);
	ReleaseCOM(pInstanceBuffer);
	ReleaseCOM(pIndexBuffer);
	ReleaseCOM(pSRtexture);
	ReleaseCOM(pSRnormalMap);

}