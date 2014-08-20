#pragma once

#include "Utilities.h"
#include "Structures.h"

// bounding box
class BoundingBox {

public:

	// конструкторы
	BoundingBox() : position(NumXY<float>()), radius(0), active(true) {};
	BoundingBox(NumXY<float> position, float radius) : position(position), radius(radius), active(true) {};

	// функции
	bool Draw(myXMFLOAT3 position); // рисование
	void SetPosition(NumXY<float> position) { this->position = position; }; // сместить boundingbox

	// переменные
	NumXY<float> position; // координаты центра
	float radius; // радиус окружности
	bool active; // boundingbox включен

};