#include "VectorAnimModel.h"
#include "Mediator.h"

// статические переменные класса экземпл€ров
AnimModel* PerModel::parent = NULL; // родитель

// конструктор
PerModel::PerModel(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity) {

	// инициализаци€ переменных
	InitFinalMatrices(bonesAmount);
	this->position = position;
	this->position.y = Mediator::superFunct->HeightFunction(position.x, position.z);
	accumulation.resize(clipsAmount);
	std::fill(accumulation.begin(), accumulation.end(), 0.0f);
	blendFactors.resize(clipsAmount);
	std::fill(blendFactors.begin(), blendFactors.end(), 0.0f);
	blendFactors[0] = 1.0f;
	curFrame.resize(clipsAmount);
	std::fill(curFrame.begin(), curFrame.end(), 0);
	this->clipsAmount = clipsAmount;
	this->framesAmount = framesAmount;
	this->velocity = velocity;

	// создать boundingbox
	boundBox = BoundingBox(NumXY<float>(position.x, position.z), PERS_RADIUS);

};

// обновить статические переменные дл€ рисовани€
void PerModel::UpdateParent(AnimModel* parent) {

	PerModel::parent = parent;

}

// переместитьс€ в новую точку (абсолютные координаты или смещение)
bool PerModel::GoToXY(NumXY<float> newPosition, bool absolute) {

	persState = PS_WALK; // состо€ние
	destination = absolute ? newPosition : NumXY<float>(position.x + newPosition.x, position.z + newPosition.y); // пункт назначени€
	NumXY<float> transF = absolute ? NumXY<float>(destination.x - position.x, destination.y - position.z) : newPosition; // смещение
	SetRotAngle(atan2f(-transF.x, -transF.y)); // угол поворота

	return true;

}

// получение повреждени€ (true - помер)
bool PerModel::TakeDamage(float damage) {

	// бить мЄртвого
	if (persState == PS_DIE || persState == PS_CORPSE || persState == PS_BURY) return false;

	// получить удар
	Mediator::helper->SetFloatText(position, (int)damage);
	hitPoints -= damage;

	// дать сдачи
	persState = PS_STRIKE;
	waitTime = SetCurrentClip(PC_STRIKE);
	damageWaitPoint = waitTime * 0.5f;
	SetClipBlendSpeed(FAST_CLIP_BLEND_SPEED);
	SetAngleSpeed(ATACK_ANGLE_SPEED);
	NumXY<float> transF = NumXY<float>(Mediator::hero->GetPosition().x - position.x, Mediator::hero->GetPosition().z - position.z); // смещение
	SetRotAngle(atan2f(-transF.x, -transF.y));

	return hitPoints <= 0;

}

// объект умер
bool PerModel::Die() {

	persState = PS_DIE;
	waitTime = SetCurrentClip(PC_DIE);
	boundBox.active = false;

	return true;

}

// обновление экземпл€ра
bool PerModel::Update(float dT) {

	// определение поведени€
	BR(Behave(dT));

	// мЄртвого не обрабатывать
	if (persState == PS_CORPSE || persState == PS_BURY) {
		return true;
	}

	// обновление родител€
	BR(SingleAnimatedModel::Update(dT));

	return true;

}

// поведение экземпл€ра
bool PerModel::Behave(float dT) {

	switch (persState) {

	// удар
	case PS_STRIKE:

		waitTime -= dT;
		damageWaitPoint -= dT;
		if (waitTime < 0.0f) {

			// удар закончилс€, вернутьс€ в исходную позицию
			waitTime = BeIdle();
			SetClipBlendSpeed(Const::clipBlendSpeed);
			SetAngleSpeed(ANGLE_SPEED);
		}
		else if (damageWaitPoint < 0.0f) {

			// удар по герою
			myXMFLOAT3 damagePoint = MathHelper::GetPositionFromAngle(GetDrawAngle(), position, ATTACK_DISTANCE);
			Mediator::SendMobEventDamage(damagePoint, 2.0f);
			damageWaitPoint = waitTime;
		}
		break;

	// стоит на месте
	case PS_STAND:

		waitTime -= dT;
		if (waitTime < 0.0f) {

			// бежать куда попало
			BR(RandomRun());
		}
		break;

	// идЄт
	case PS_WALK:

	{

		// вспомогательные переменные
		myXMFLOAT3 prevPos = position; // предыдуща€ позици€
		bool terrainBlock = true; // проход перекрыт территорией

		// определение места назначени€
		XMVECTOR displacement = { destination.x - position.x, destination.y - position.z };
		XMVECTOR norDisplacement = XMVector2Normalize(displacement) * dT * velocity;
		if (XMVectorGetX(XMVector2Length(displacement)) <= XMVectorGetX(XMVector2Length(norDisplacement))) {
			waitTime = BeIdle();
			break;
		}
		position = myXMFLOAT3(XMVectorGetX(norDisplacement) + position.x, position.y, XMVectorGetY(norDisplacement) + position.z);
		position.y = Mediator::superFunct->HeightFunction(position.x, position.z);
		boundBox.SetPosition(NumXY<float>(position.x, position.z));

		// проверка территориальных преп€тствий
		NumXY<int> regAdr = MathHelper::GetDynamicRegionAdress(position);
		NumXY<int> contAdr = MathHelper::GetContentAdress(position);
		if (regAdr.x != -1)
			if (Mediator::regionField[regAdr.x][regAdr.y])
				if (Mediator::regionField[regAdr.x][regAdr.y]->busy)
					terrainBlock = Mediator::regionField[regAdr.x][regAdr.y]->getContent()[contAdr.x][contAdr.y].blocked ? true : false;

		// столкновение с преп€тствием
		if (terrainBlock || Mediator::artInt->CheckObject(boundBox, true)) {

			// вернуть всЄ на место
			position = prevPos;
			boundBox.SetPosition(NumXY<float>(position.x, position.z));
			waitTime = BeIdle();
		}

		break;
	}

	// умирает
	case PS_DIE:

		waitTime -= dT;
		if (waitTime < 0.0f) {
			persState = PS_CORPSE; // превратилс€ в труп
			waitTime = DEAD_LIE_TIME;
		}
		break;

	// лежит трупом
	case PS_CORPSE:

		waitTime -= dT;
		if (waitTime < 0.0f) {
			persState = PS_BURY;
			waitTime = BURY_TIME;
		}
		break;

	// проваливаетс€ под землю
	case PS_BURY:

		waitTime -= dT;
		if (waitTime < 0.0f) toDelete = true; // удалить объект
		position.y -= dT * BURY_SPEED;
		break;

	} // switch case

	return true;

}

// сто€ть на месте
float PerModel::BeIdle() {

	persState = PS_STAND;
	SetCurrentClip(PC_STAND);
	return MathHelper::RandF(1.0f, 2.0f);

}
// случайно бежать
bool PerModel::RandomRun() {

	NumXY<float> displ = MathHelper::GetDisplacementFromDirection(MathHelper::RandF(-XM_PI, XM_PI), MathHelper::RandF(2.0f, 5.0f));
	BR(GoToXY(displ, false));
	SetCurrentClip(PC_WALK);
	return true;

}

// рисование экземпл€ра
bool PerModel::Draw() {

	// вычисление матриц костей
	if (recreateFinalMatrices) {
		if (restCurClip) parent->CreateFinalMatrices(curClip, curFrame[curClip], finalMatrices);
		else parent->CreateFinalMatrices(curFrame, blendFactors, finalMatrices);
		recreateFinalMatrices = false;
	}

	// установить константы
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	XMMATRIX R = XMLoadFloat4x4(&rotationMatrix);

	// обновить шейдерные буферы
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(Mediator::cbufAnimModel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufAnimModel* cbuf = reinterpret_cast<CbufAnimModel*>(mappedData.pData);
	XMStoreFloat4x4(&cbuf->gWorld, XMMatrixTranspose(R * W_JP)); // мирова€ матрица
	for (UINT i(0); i < parent->bonesAmount; i++) { // матрицы костей
		XMMATRIX finalMatrix = XMLoadFloat4x4(&finalMatrices[i]);
		XMStoreFloat4x4(&cbuf->gBones[i], XMMatrixTranspose(finalMatrix));
	}
	Mediator::pDevContext->Unmap(Mediator::cbufAnimModel, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_ANIMMODEL, 1, &Mediator::cbufAnimModel);

	// рисование
	Mediator::pDevContext->DrawIndexed(parent->indicesAmount, 0, 0);

	return true;

}

// деструктор
PerModel::~PerModel() {


}