#include "VectorAnimModel.h"
#include "Mediator.h"

// ����������� ���������� ������ �����������
AnimModel* PerModel::parent = NULL; // ��������

// �����������
PerModel::PerModel(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity) {

	// ������������� ����������
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

	// ������� boundingbox
	boundBox = BoundingBox(NumXY<float>(position.x, position.z), PERS_RADIUS);

};

// �������� ����������� ���������� ��� ���������
void PerModel::UpdateParent(AnimModel* parent) {

	PerModel::parent = parent;

}

// ������������� � ����� ����� (���������� ���������� ��� ��������)
bool PerModel::GoToXY(NumXY<float> newPosition, bool absolute) {

	persState = PS_WALK; // ���������
	destination = absolute ? newPosition : NumXY<float>(position.x + newPosition.x, position.z + newPosition.y); // ����� ����������
	NumXY<float> transF = absolute ? NumXY<float>(destination.x - position.x, destination.y - position.z) : newPosition; // ��������
	SetRotAngle(atan2f(-transF.x, -transF.y)); // ���� ��������

	return true;

}

// ��������� ����������� (true - �����)
bool PerModel::TakeDamage(float damage) {

	// ���� �������
	if (persState == PS_DIE || persState == PS_CORPSE || persState == PS_BURY) return false;

	// �������� ����
	Mediator::helper->SetFloatText(position, (int)damage);
	hitPoints -= damage;

	// ���� �����
	persState = PS_STRIKE;
	waitTime = SetCurrentClip(PC_STRIKE);
	damageWaitPoint = waitTime * 0.5f;
	SetClipBlendSpeed(FAST_CLIP_BLEND_SPEED);
	SetAngleSpeed(ATACK_ANGLE_SPEED);
	NumXY<float> transF = NumXY<float>(Mediator::hero->GetPosition().x - position.x, Mediator::hero->GetPosition().z - position.z); // ��������
	SetRotAngle(atan2f(-transF.x, -transF.y));

	return hitPoints <= 0;

}

// ������ ����
bool PerModel::Die() {

	persState = PS_DIE;
	waitTime = SetCurrentClip(PC_DIE);
	boundBox.active = false;

	return true;

}

// ���������� ����������
bool PerModel::Update(float dT) {

	// ����������� ���������
	BR(Behave(dT));

	// ������� �� ������������
	if (persState == PS_CORPSE || persState == PS_BURY) {
		return true;
	}

	// ���������� ��������
	BR(SingleAnimatedModel::Update(dT));

	return true;

}

// ��������� ����������
bool PerModel::Behave(float dT) {

	switch (persState) {

	// ����
	case PS_STRIKE:

		waitTime -= dT;
		damageWaitPoint -= dT;
		if (waitTime < 0.0f) {

			// ���� ����������, ��������� � �������� �������
			waitTime = BeIdle();
			SetClipBlendSpeed(Const::clipBlendSpeed);
			SetAngleSpeed(ANGLE_SPEED);
		}
		else if (damageWaitPoint < 0.0f) {

			// ���� �� �����
			myXMFLOAT3 damagePoint = MathHelper::GetPositionFromAngle(GetDrawAngle(), position, ATTACK_DISTANCE);
			Mediator::SendMobEventDamage(damagePoint, 2.0f);
			damageWaitPoint = waitTime;
		}
		break;

	// ����� �� �����
	case PS_STAND:

		waitTime -= dT;
		if (waitTime < 0.0f) {

			// ������ ���� ������
			BR(RandomRun());
		}
		break;

	// ���
	case PS_WALK:

	{

		// ��������������� ����������
		myXMFLOAT3 prevPos = position; // ���������� �������
		bool terrainBlock = true; // ������ �������� �����������

		// ����������� ����� ����������
		XMVECTOR displacement = { destination.x - position.x, destination.y - position.z };
		XMVECTOR norDisplacement = XMVector2Normalize(displacement) * dT * velocity;
		if (XMVectorGetX(XMVector2Length(displacement)) <= XMVectorGetX(XMVector2Length(norDisplacement))) {
			waitTime = BeIdle();
			break;
		}
		position = myXMFLOAT3(XMVectorGetX(norDisplacement) + position.x, position.y, XMVectorGetY(norDisplacement) + position.z);
		position.y = Mediator::superFunct->HeightFunction(position.x, position.z);
		boundBox.SetPosition(NumXY<float>(position.x, position.z));

		// �������� ��������������� �����������
		NumXY<int> regAdr = MathHelper::GetDynamicRegionAdress(position);
		NumXY<int> contAdr = MathHelper::GetContentAdress(position);
		if (regAdr.x != -1)
			if (Mediator::regionField[regAdr.x][regAdr.y])
				if (Mediator::regionField[regAdr.x][regAdr.y]->busy)
					terrainBlock = Mediator::regionField[regAdr.x][regAdr.y]->getContent()[contAdr.x][contAdr.y].blocked ? true : false;

		// ������������ � ������������
		if (terrainBlock || Mediator::artInt->CheckObject(boundBox, true)) {

			// ������� �� �� �����
			position = prevPos;
			boundBox.SetPosition(NumXY<float>(position.x, position.z));
			waitTime = BeIdle();
		}

		break;
	}

	// �������
	case PS_DIE:

		waitTime -= dT;
		if (waitTime < 0.0f) {
			persState = PS_CORPSE; // ����������� � ����
			waitTime = DEAD_LIE_TIME;
		}
		break;

	// ����� ������
	case PS_CORPSE:

		waitTime -= dT;
		if (waitTime < 0.0f) {
			persState = PS_BURY;
			waitTime = BURY_TIME;
		}
		break;

	// ������������� ��� �����
	case PS_BURY:

		waitTime -= dT;
		if (waitTime < 0.0f) toDelete = true; // ������� ������
		position.y -= dT * BURY_SPEED;
		break;

	} // switch case

	return true;

}

// ������ �� �����
float PerModel::BeIdle() {

	persState = PS_STAND;
	SetCurrentClip(PC_STAND);
	return MathHelper::RandF(1.0f, 2.0f);

}
// �������� ������
bool PerModel::RandomRun() {

	NumXY<float> displ = MathHelper::GetDisplacementFromDirection(MathHelper::RandF(-XM_PI, XM_PI), MathHelper::RandF(2.0f, 5.0f));
	BR(GoToXY(displ, false));
	SetCurrentClip(PC_WALK);
	return true;

}

// ��������� ����������
bool PerModel::Draw() {

	// ���������� ������ ������
	if (recreateFinalMatrices) {
		if (restCurClip) parent->CreateFinalMatrices(curClip, curFrame[curClip], finalMatrices);
		else parent->CreateFinalMatrices(curFrame, blendFactors, finalMatrices);
		recreateFinalMatrices = false;
	}

	// ���������� ���������
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	XMMATRIX R = XMLoadFloat4x4(&rotationMatrix);

	// �������� ��������� ������
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(Mediator::cbufAnimModel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufAnimModel* cbuf = reinterpret_cast<CbufAnimModel*>(mappedData.pData);
	XMStoreFloat4x4(&cbuf->gWorld, XMMatrixTranspose(R * W_JP)); // ������� �������
	for (UINT i(0); i < parent->bonesAmount; i++) { // ������� ������
		XMMATRIX finalMatrix = XMLoadFloat4x4(&finalMatrices[i]);
		XMStoreFloat4x4(&cbuf->gBones[i], XMMatrixTranspose(finalMatrix));
	}
	Mediator::pDevContext->Unmap(Mediator::cbufAnimModel, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_ANIMMODEL, 1, &Mediator::cbufAnimModel);

	// ���������
	Mediator::pDevContext->DrawIndexed(parent->indicesAmount, 0, 0);

	return true;

}

// ����������
PerModel::~PerModel() {


}