#include "TerrainVectorModel.h"
#include "Mediator.h"

// конструктор вектора моделей
TerrainVectorModel::TerrainVectorModel(wchar_t* textureFilePath, wchar_t* binFilePath, bool* result) : Model(textureFilePath, binFilePath, myXMFLOAT3(), result) {

	BRR(*result);

	*result = true;

};

// родить новый элемент (измен€ет координаты ввода)
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

// добавить кусок описани€
bool TerrainVectorModel::AddDescription(NumXY<int> regionAdress, UINT elementsAmount) {

	elementsDesc.push_back(ElementDesc(regionAdress, elementsAmount));

	return true;

}

// удалить все элементы в регионе (используетс€ дескриптор)
bool TerrainVectorModel::EraseByRegion(NumXY<int> regionAdress) {

	// обновить описатель
	UINT startToDelete = 0; // начало удал€емых элементов
	UINT toDelete = 0; // количество удал€емых элементов

	// поиск в описателе элементов информации
	for (UINT i(0); i < elementsDesc.size(); i++) {

		if (regionAdress == elementsDesc[i].regionAdress) {
		
			toDelete = elementsDesc[i].elementsAmount;
			auto iter = elementsDesc.begin() + i;
			elementsDesc.erase(iter);
			break;

		}

		startToDelete += elementsDesc[i].elementsAmount;

	}
	// обновить вектор элементов
	auto iter = elements.begin() + startToDelete;
	for (UINT i(0); i < toDelete; i++) {
		iter = elements.erase(iter);
	}

	return true;

}

// добавить элемент в €чейку
bool TerrainVectorModel::AddElementToVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance modelParams) {

	bool bInsert = false; // пни в регионе уже есть
	UINT toInsert = 0; // адрес элемента дл€ ставки

	// правка описател€
	for (auto iter = elementsDesc.begin(); iter != elementsDesc.end(); iter++) {

		// поиск нужного региона
		if (iter->regionAdress == regionAdress) {

			// добавление элемента
			toInsert += iter->elementsAmount;
			bInsert = true;
			iter->elementsAmount++;
			break;
		}

		toInsert += iter->elementsAmount;
	}

	if (bInsert) {
		// в регионе уже есть пни
		elements.insert(elements.begin() + toInsert, modelParams);
	}
	else {
		// в регионе нет пней
		elementsDesc.push_back(ElementDesc(regionAdress, 1));
		elements.push_back(modelParams);
	}

	return true;

}

// удалить элемент из €чейки, true - если удалилс€
bool TerrainVectorModel::RemoveElementFromVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance* modelParams) {

	UINT toDelete = 0; // адрес элемента дл€ удалени€
	bool bDelete = false; // есть ли такой элемент

	// правка описател€
	for (auto iter = elementsDesc.begin(); iter != elementsDesc.end(); iter++) {

		// поиск нужного региона
		if (iter->regionAdress == regionAdress) {

			// поиск нужной €чейки
			for (UINT i = 0; i < iter->elementsAmount; i++) {
				if (cellAdress == MathHelper::GetContentAdress(elements[toDelete + i].position)) {

					// предмет найден
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

	// правка вектора
	if (bDelete) { // причесать повторени€ адреса
		auto delInstance = elements.begin() + toDelete;
		if (modelParams) *modelParams = PerModelInstance(delInstance->position, delInstance->rotation, delInstance->scale);
		elements.erase(elements.begin() + toDelete);
	}
	else return false;

	return true;

}

// рисование вектора моделей
bool TerrainVectorModel::Draw(bool toShadow, ID3D11Buffer* pInst, UINT start, UINT amount) {

	// установить параметры
	UINT vertexStrides[2] = { sizeof(VertPos3Nor3Tex2), sizeof (PerModelInstance) };
	UINT vertexOffsets[2] = { 0, 0 };
	ID3D11Buffer* vertInstBuffers[2] = { pVertexBuffer, pInst };
	Mediator::pDevContext->IASetVertexBuffers(0, 2, vertInstBuffers, vertexStrides, vertexOffsets);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (!toShadow) Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);

	// рисование
	Mediator::pDevContext->DrawIndexedInstanced(indicesAmount, amount, 0, 0, start);

	return true;

}