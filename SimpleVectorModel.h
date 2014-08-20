#pragma once

#include "Model.h"

// дл€ каждого экземпл€ра
struct PerSimpleVectorModel {

	PerSimpleVectorModel(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) : position(position), scale(scale), rotation(rotation) {};
	bool Draw(UINT indicesAmount); // рисование
	myXMFLOAT3 position; // координаты
	myXMFLOAT3 scale; // масштаб
	float rotation; // поворот

};

// простейший вектор моделей
class SimpleVectorModel : public Model {

public:

	// простейший вектор моделей
	SimpleVectorModel(bool* result); // конструктор

	// важные функции
	bool Draw(); // рисование

	// функции
	bool CreateInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation); // создать экземпл€р
	bool ShowFlyInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation); // создать экземпл€р-подЄнок

private:

	// переменные
	std::vector<PerSimpleVectorModel> instances; // экземпл€ры вектора
	std::vector<PerSimpleVectorModel> flyInstances; // экземпл€ры-подЄнки

};