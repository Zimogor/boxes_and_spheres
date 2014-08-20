#include "ArtInt.h"
#include "Mediator.h"

// �����������
ArtInt::ArtInt(bool* result) {

	// ������������� �������� ������
	BRRC2(devils = new VectorAnimModel(L"resources\\animated_models\\devil.bin", L"resources\\animated_models\\devil.dds", 2.0f, result)); // �����
	BRRC2(boars = new VectorBoars(L"resources\\animated_models\\boar.bin", L"resources\\animated_models\\boar.dds", 2.0f, result)); // ������

	Mediator::artInt = this;

}

// ���������� ������ �����
bool ArtInt::TrySpawn(NumXY<float> showPoint, UINT persType) {

	// ����������� ����������
	if (boars->GetAmount() + devils->GetAmount() > 10) return false;

	// ������������� ����������
	float angle = MathHelper::RandF(-XM_PI, XM_PI);
	float radius = Const::showRadius;
	NumXY<float> spawnPoint;
	NumXY<int> regAdr;

	do {

		// ���������� ����� ���������
		spawnPoint = showPoint + MathHelper::GetDisplacementFromDirection(angle, radius);
		regAdr = MathHelper::GetDynamicRegionAdress(spawnPoint);
		if (regAdr.x == -1) break;
		if (Mediator::regionField[regAdr.x][regAdr.y]) break;
		radius -= 1.0f;

	} while (true);

	// �������� �� ������������ � ���������� ������
	NumXY<int> contAdr = MathHelper::GetContentAdress(spawnPoint);
	if (Mediator::regionField[regAdr.x][regAdr.y]->getContent()[contAdr.x][contAdr.y].blocked) return false;

	// ������� �� ������������ � ������ ������
	BoundingBox potBox(NumXY<float>(spawnPoint.x, spawnPoint.y), PERS_RADIUS);
	if (CheckObject(potBox, false)) return false;

	// ������ �����
	if (persType) { BR(boars->Spawn(spawnPoint)); }
	else { BR(devils->Spawn(spawnPoint)); }

	return true;

}

// ������� ���� � �����
bool ArtInt::TakeDamage(myXMFLOAT3 position, float damage) {

	BR(boars->GetDamage(position, damage));
	BR(devils->GetDamage(position, damage));

	return true;

}

// ���������, �� ������ �� ����� ������ ������ (include - ��� ����� � ������)
bool ArtInt::CheckObject(BoundingBox boundCyl, bool include) {

	UINT counter = 0;

	// ������������ � ������
	if (MathHelper::OverlapBox(boundCyl, *Mediator::heroBoundBox)) counter += 1;

	// ������������ � �������
	counter += devils->CheckObject(boundCyl);
	if (counter == 1 && !include) return true;
	if (counter > 1) return true;
	counter += boars->CheckObject(boundCyl);
	if (counter == 1 && !include) return true;
	if (counter > 1) return true;

	return false;

}

// ���������� ��
bool ArtInt::Update(float dT, NumXY<float> showPoint) {

	// �������� ������
	static float persTime = persSpawnTime;
	persTime -= dT;
	if (persTime < 0.0f) {
		persTime += persSpawnTime;
		TrySpawn(showPoint, 0);
		TrySpawn(showPoint, 1);
	}

	// ���������� �������� ������
	BR(boars->Update(dT, showPoint)); // �����
	BR(devils->Update(dT, showPoint)); // ������

	return true;

}

// ��������� ��
bool ArtInt::Draw(bool toShadowMap) {

	// ��������� ����������
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ��������� ������
	BR(boars->Draw(toShadowMap)); // �����
	BR(devils->Draw(toShadowMap)); // ������

	return true;

}

// ����������
ArtInt::~ArtInt() {

	ReleaseNULL(boars);
	ReleaseNULL(devils);

}
