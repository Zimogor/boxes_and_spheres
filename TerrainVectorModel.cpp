#include "TerrainVectorModel.h"
#include "Mediator.h"

// ����������� ������� �������
TerrainVectorModel::TerrainVectorModel(wchar_t* textureFilePath, wchar_t* binFilePath, bool* result) : Model(textureFilePath, binFilePath, myXMFLOAT3(), result) {

	BRR(*result);

	*result = true;

};

// ������ ����� ������� (�������� ���������� �����)
bool TerrainVectorModel::Spawn(myXMFLOAT3* position) {

	float random0 = Mediator::superFunct->FullRandom0(position->x, position->z);
	float random1 = Mediator::superFunct->FullRandom1(position->x, position->z);
	float random2 = Mediator::superFunct->FullRandom2(position->x, position->z);
	float random3 = Mediator::superFunct->FullRandom3(position->x, position->z);
	float random4 = Mediator::superFunct->FullRandom4(position->x, position->z);
	XMFLOAT2 scale = { random0 * Const::widthScatter + 1.0f, random1 * Const::heightScatter + 1.0f };
	position->x += random3 * Const::positionScatter;
	position->z += random4 * Const::positionScatter;
	elements.push_back(PerModelInstance(*position, random2 * XM_PI, scale));

	return true;

};

// �������� ����� ��������
bool TerrainVectorModel::AddDescription(NumXY<int> regionAdress, UINT elementsAmount) {

	elementsDesc.push_back(ElementDesc(regionAdress, elementsAmount));

	return true;

}

// ������� ��� �������� � ������� (������������ ����������)
bool TerrainVectorModel::EraseByRegion(NumXY<int> regionAdress) {

	// �������� ���������
	UINT startToDelete = 0; // ������ ��������� ���������
	UINT toDelete = 0; // ���������� ��������� ���������

	// ����� � ��������� ��������� ����������
	for (UINT i(0); i < elementsDesc.size(); i++) {

		if (regionAdress == elementsDesc[i].regionAdress) {
		
			toDelete = elementsDesc[i].elementsAmount;
			auto iter = elementsDesc.begin() + i;
			elementsDesc.erase(iter);
			break;

		}

		startToDelete += elementsDesc[i].elementsAmount;

	}
	// �������� ������ ���������
	auto iter = elements.begin() + startToDelete;
	for (UINT i(0); i < toDelete; i++) {
		iter = elements.erase(iter);
	}

	return true;

}

// �������� ������� � ������
bool TerrainVectorModel::AddElementToVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance modelParams) {

	bool bInsert = false; // ��� � ������� ��� ����
	UINT toInsert = 0; // ����� �������� ��� ������

	// ������ ���������
	for (auto iter = elementsDesc.begin(); iter != elementsDesc.end(); iter++) {

		// ����� ������� �������
		if (iter->regionAdress == regionAdress) {

			// ���������� ��������
			toInsert += iter->elementsAmount;
			bInsert = true;
			iter->elementsAmount++;
			break;
		}

		toInsert += iter->elementsAmount;
	}

	if (bInsert) {
		// � ������� ��� ���� ���
		elements.insert(elements.begin() + toInsert, modelParams);
	}
	else {
		// � ������� ��� ����
		elementsDesc.push_back(ElementDesc(regionAdress, 1));
		elements.push_back(modelParams);
	}

	return true;

}

// ������� ������� �� ������, true - ���� ��������
bool TerrainVectorModel::RemoveElementFromVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance* modelParams) {

	UINT toDelete = 0; // ����� �������� ��� ��������
	bool bDelete = false; // ���� �� ����� �������

	// ������ ���������
	for (auto iter = elementsDesc.begin(); iter != elementsDesc.end(); iter++) {

		// ����� ������� �������
		if (iter->regionAdress == regionAdress) {

			// ����� ������ ������
			for (UINT i = 0; i < iter->elementsAmount; i++) {
				if (cellAdress == MathHelper::GetContentAdress(elements[toDelete + i].position)) {

					// ������� ������
					iter->elementsAmount -= 1;
					if (iter->elementsAmount == 0) elementsDesc.erase(iter);
					bDelete = true;
					toDelete += i;
					break;

				}
			}

			break;

		}

		toDelete += iter->elementsAmount;
	}

	// ������ �������
	if (bDelete) { // ��������� ���������� ������
		auto delInstance = elements.begin() + toDelete;
		if (modelParams) *modelParams = PerModelInstance(delInstance->position, delInstance->rotation, delInstance->scale);
		elements.erase(elements.begin() + toDelete);
	}
	else return false;

	return true;

}

// ��������� ������� �������
bool TerrainVectorModel::Draw(bool toShadow, ID3D11Buffer* pInst, UINT start, UINT amount) {

	// ���������� ���������
	UINT vertexStrides[2] = { sizeof(VertPos3Nor3Tex2), sizeof (PerModelInstance) };
	UINT vertexOffsets[2] = { 0, 0 };
	ID3D11Buffer* vertInstBuffers[2] = { pVertexBuffer, pInst };
	Mediator::pDevContext->IASetVertexBuffers(0, 2, vertInstBuffers, vertexStrides, vertexOffsets);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (!toShadow) Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);

	// ���������
	Mediator::pDevContext->DrawIndexedInstanced(indicesAmount, amount, 0, 0, start);

	return true;

}