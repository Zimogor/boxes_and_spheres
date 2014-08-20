#ifndef ARTINT_H
#define ARTINT_H

#include "Utilities.h"
#include "Nums.h"
#include "Console.h"
#include "BoundingBox.h"
#include "SubModels.h"

// класс искусственного интеллекта
class ArtInt {

public:

	// искусственный интеллект
	ArtInt(bool* result); // конструктор
	~ArtInt(); // деструктор

	// полезные функции
	bool Update(float dT, NumXY<float> showPoint); // обновление
	bool Draw(bool toShadowMap); // рисование

	// функции
	bool TakeDamage(myXMFLOAT3 position, float damage); // нанести урон в точку
	bool CheckObject(BoundingBox boundBox, bool include); // проверить, на занята ли точка другим персом (include - сам оъект в списке)

private:

	// сомнительные переменные
	float persSpawnTime = 1.0f; // периодичность рождения пауков

	// функции
	bool TrySpawn(NumXY<float> showPoint, UINT persType); // попытаться родить перса, true - если родил

	// векторы персов
	VectorBoars* boars = NULL;
	VectorAnimModel* devils = NULL;

};

#endif // ARTINT_H