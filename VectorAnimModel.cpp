#include "VectorAnimModel.h"
#include "Mediator.h"

// конструктор
VectorAnimModel::VectorAnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, float velocity, bool* result) : AnimModel(binFilePath, textureFilePath, result) {

	BRR(*result);

	// инициализаци€ переменных
	this->velocity = velocity;

	*result = true;

}

// создать экземпл€р
bool VectorAnimModel::Spawn(myXMFLOAT3 position) {

	modelVector.push_back(AddNewModel(position, bonesAmount));

	return true;

}
bool VectorAnimModel::Spawn(NumXY<float> position) {

	myXMFLOAT3 positionF3 = myXMFLOAT3(position.x, Mediator::superFunct->HeightFunction(position), position.y);
	modelVector.push_back(AddNewModel(positionF3, bonesAmount));

	return true;

}

// получить урон
bool VectorAnimModel::GetDamage(myXMFLOAT3 position, float damage) {

	if (modelVector.size()) {

		for (auto iter = modelVector.begin(); iter != modelVector.end(); iter++)
			if (MathHelper::Distance((*iter)->position, position) <= 1.0f) {

				// удар по объекту
				if ((*iter)->TakeDamage(damage)) {
					// объект убит
					BR((*iter)->Die());
					GiveSomething(); // дать предметов герою
				}
			}
	}

	return true;

}

// проверка на пересечение с персами вектора
UINT VectorAnimModel::CheckObject(BoundingBox boundBox) {

	UINT counter = 0;

	// столкновение каждого с каждым
	for (auto iter = modelVector.begin(); iter != modelVector.end(); iter++) {
		if (MathHelper::OverlapBox(boundBox, (*iter)->boundBox)) {
			counter++;
		}
	}

	return counter;

	return 0;

}

// обновление вектора моделей с кост€ми 
bool VectorAnimModel::Update(float dT, NumXY<float> showPoint) {

	if (modelVector.size()) {

		// обновить вектор моделей
		for (auto iter = modelVector.begin(); iter != modelVector.end(); ) {

			// удалить модель
			if ((*iter)->toDelete) {
				delete *iter;
				iter = modelVector.erase(iter);
				continue;
			}

			// обновить модель
			NumXY<int> curReg = MathHelper::GetDynamicRegionAdress((*iter)->position);
			if (curReg.x != -1)
				if (Mediator::regionField[curReg.x][curReg.y]) {
					// обновить экземпл€р
					BR((*iter)->Update(dT));
					iter++;
					continue;
				}

			//сгинул
			delete *iter;
			iter = modelVector.erase(iter);

		} // дл€ каждой модели
	}

	return true;

}

// рисование вектора моделей с кост€ми
bool VectorAnimModel::Draw(bool toShadowMap) {

	if (modelVector.size()) {

		// обновить полустатические члены дл€ рисовани€
		PerModel::UpdateParent(this);

		// установить параметры
		UINT vertexStride = sizeof(VertPos3Nor2Tex2W1Bone2);
		UINT vertexOffset = 0;
		Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
		Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2W1Bone2);
		if (toShadowMap) {
			Mediator::pDevContext->VSSetShader(Mediator::pVSShadow, NULL, 0);
			Mediator::pDevContext->PSSetShader(Mediator::pPSEmpty, NULL, 0);
		}
		else {
			Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
			Mediator::pDevContext->VSSetShader(Mediator::pVSPosNorTexWBone, NULL, 0);
			Mediator::pDevContext->PSSetShader(Mediator::pPSPosNorTex, NULL, 0);
		}

		// дл€ каждого экземпл€ра
		for (auto iter = modelVector.begin(); iter != modelVector.end(); iter ++) {
			if (!Mediator::camera->GetFrustum()->CheckSphere((*iter)->position, ANIM_MODEL_RADIUS)) continue;
			BR((*iter)->Draw()); // рисование
		}

		if (toShadowMap) return true;

		// рисовать bounding boxes
		if (Const::showBoundingBox) {

			vertexStride = sizeof(VertPos3Nor3);
			Mediator::pDevContext->IASetVertexBuffers(0, 1, &Mediator::helper->pVertexCylinderBuffer, &vertexStride, &vertexOffset);
			Mediator::pDevContext->IASetIndexBuffer(Mediator::helper->pIndexCylinderBuffer, DXGI_FORMAT_R32_UINT, 0);
			Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3);
			Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
			Mediator::pDevContext->PSSetShader(Mediator::pPSSolid, NULL, 0);

			for (auto iter = modelVector.begin(); iter != modelVector.end(); iter++)
				if ((*iter)->boundBox.active) {
					if (!Mediator::camera->GetFrustum()->CheckSphere((*iter)->position, ANIM_MODEL_RADIUS)) continue;
					BR((*iter)->boundBox.Draw((*iter)->position));
				}

		}

	}

	return true;

}

// деструктор
VectorAnimModel::~VectorAnimModel() {

	for (UINT i(0); i < modelVector.size(); i++) {
		DestroyModel(modelVector[i]);
	}

}