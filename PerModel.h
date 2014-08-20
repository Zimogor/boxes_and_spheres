#ifndef PERMODEL_H
#define PERMODEL_H

#include "Utilities.h"
#include "Nums.h"
#include "Console.h"
#include "Terrain.h"
#include "AnimModel.h"

#define DEAD_LIE_TIME 5.0f // врем€ лежани€ трупа
#define BURY_SPEED 0.5f // скорость проваливани€ трупа под землю
#define BURY_TIME 5.0f // врем€ проваливани€ до удалени€
#define PERS_RADIUS 0.4f // диаметр boundingBox перса


// класс экземпл€ров векторов
class PerModel : public SingleAnimatedModel {

public:

	// экземпл€р
	PerModel(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity); // конструктор
	~PerModel(); // деструктор

	// важные функции
	bool Draw(); // рисование экземпл€ра
	bool Update(float dT); // обновление экземпл€ра

	// функции
	static void UpdateParent(AnimModel* parent); // обновить статические переменные дл€ рисовани€	
	bool TakeDamage(float damage); // получение повреждени€ (true - помер)
	bool Die(); // объект умирает

	// переменные
	bool toDelete = false; // объект необходимо удалить
	BoundingBox boundBox; // boundingbox перса

private:

	// переменные
	PERS_STATE persState = PS_STAND; // состо€ние экземпл€ра

	// сомнительные переменные
	NumXY<float> destination = NumXY<float>(0.0f, 0.0f); // пункт назначени€
	float waitTime = 0.0f; // врем€ ожидани€ до перемещени€ или ещЄ чего-нибудь

	// статические переменные
	float velocity; // скорость движени€
	static AnimModel* parent; // родитель

	// функции
	bool GoToXY(NumXY<float> newPosition, bool absolute); // переместитьс€ в новую точку (абсолютные координаты или смещение)
	bool Behave(float dT); // поведение экземпл€ра

	// поведенческие функции
	float BeIdle(); // сто€сть на месте (возвращает врем€)
	bool RandomRun(); // случайно бежать

};


#endif // PERMODEL_H