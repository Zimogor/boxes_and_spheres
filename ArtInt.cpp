#include "ArtInt.h"
#include "Mediator.h"

// конструктор
ArtInt::ArtInt(bool* result) {

	// инициализация векторов персов
	BRRC2(devils = new VectorAnimModel(L"resources\\animated_models\\devil.bin", L"resources\\animated_models\\devil.dds", 2.0f, result)); // черти
	BRRC2(boars = new VectorBoars(L"resources\\animated_models\\boar.bin", L"resources\\animated_models\\boar.dds", 2.0f, result)); // кабаны

	Mediator::artInt = this;

}

// попытаться родить перса
bool ArtInt::TrySpawn(NumXY<float> showPoint, UINT persType) {

	// ограничение количества
	if (boars->GetAmount() + devils->GetAmount() > 10) return false;

	// инициализация переменных
	float angle = MathHelper::RandF(-XM_PI, XM_PI);
	float radius = Const::showRadius;
	NumXY<float> spawnPoint;
	NumXY<int> regAdr;

	do {

		// определить точку появления
		spawnPoint = showPoint + MathHelper::GetDisplacementFromDirection(angle, radius);
		regAdr = MathHelper::GetDynamicRegionAdress(spawnPoint);
		if (regAdr.x == -1) break;
		if (Mediator::regionField[regAdr.x][regAdr.y]) break;
		radius -= 1.0f;

	} while (true);

	// проверка на столкновение с окружающей средой
	NumXY<int> contAdr = MathHelper::GetContentAdress(spawnPoint);
	if (Mediator::regionField[regAdr.x][regAdr.y]->getContent()[contAdr.x][contAdr.y].blocked) return false;

	// провека на столкновение с другим персом
	BoundingBox potBox(NumXY<float>(spawnPoint.x, spawnPoint.y), PERS_RADIUS);
	if (CheckObject(potBox, false)) return false;

	// родить перса
	if (persType) { BR(boars->Spawn(spawnPoint)); }
	else { BR(devils->Spawn(spawnPoint)); }

	return true;

}

// нанести уров в точку
bool ArtInt::TakeDamage(myXMFLOAT3 position, float damage) {

	BR(boars->GetDamage(position, damage));
	BR(devils->GetDamage(position, damage));

	return true;

}

// проверить, на занята ли точка другим персом (include - сам оъект в списке)
bool ArtInt::CheckObject(BoundingBox boundCyl, bool include) {

	UINT counter = 0;

	// столкновение с героем
	if (MathHelper::OverlapBox(boundCyl, *Mediator::heroBoundBox)) counter += 1;

	// столкновение с персами
	counter += devils->CheckObject(boundCyl);
	if (counter == 1 && !include) return true;
	if (counter > 1) return true;
	counter += boars->CheckObject(boundCyl);
	if (counter == 1 && !include) return true;
	if (counter > 1) return true;

	return false;

}

// обновление ИИ
bool ArtInt::Update(float dT, NumXY<float> showPoint) {

	// рождение персов
	static float persTime = persSpawnTime;
	persTime -= dT;
	if (persTime < 0.0f) {
		persTime += persSpawnTime;
		TrySpawn(showPoint, 0);
		TrySpawn(showPoint, 1);
	}

	// обновление векторов персов
	BR(boars->Update(dT, showPoint)); // пауки
	BR(devils->Update(dT, showPoint)); // коровы

	return true;

}

// рисование ИИ
bool ArtInt::Draw(bool toShadowMap) {

	// установка параметров
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// рисование персов
	BR(boars->Draw(toShadowMap)); // пауки
	BR(devils->Draw(toShadowMap)); // коровы

	return true;

}

// деструктор
ArtInt::~ArtInt() {

	ReleaseNULL(boars);
	ReleaseNULL(devils);

}
