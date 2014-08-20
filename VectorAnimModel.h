#pragma once

#include "AnimModel.h"
#include "PerModel.h"
#include "Terrain.h"

#define ANIM_MODEL_RADIUS 1.0f

// класс вектора моделей с кост€ми
class VectorAnimModel : public AnimModel {

public:

	// вектор моделей с кост€ми
	VectorAnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, float velocity, bool* result); // конструктор
	~VectorAnimModel(); // деструктор

	// главные функции
	bool Update(float dT, NumXY<float> showPoint); // обновить вектор моделей с кост€ми
	bool Draw(bool toShadowMap); // рисовать вектор моделей с кост€ми
	bool Spawn(myXMFLOAT3 position); // создать экземпл€р
	bool Spawn(NumXY<float> position); // создать экземпл€р

	// функции
	UINT GetAmount() { return modelVector.size(); }; // количество экземпл€ров
	bool GetDamage(myXMFLOAT3 position, float damage); // получить урон в точке
	UINT CheckObject(BoundingBox boundBox); // проверка на пересечение с персами вектора

protected:

	// вектор моделей
	std::vector<PerModel*> modelVector;

	// переменные
	float velocity = 0.0f; // скорость перемещени€ экземпл€ров

	// функции
	virtual PerModel* AddNewModel(myXMFLOAT3 position, UINT bonesAmount) { return (new PerModel(position, clipsAmount, bonesAmount, framesAmount, velocity)); } // создать новую модель
	virtual void DestroyModel(PerModel* perModel) { delete perModel; } // удалить модель
	virtual void GiveSomething() {}; // дать герою предметы

};
