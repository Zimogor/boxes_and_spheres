#ifndef MATHHELPER_H
#define MATHHELPER_H

#include "Nums.h"
#include "Constants.h"
#include "BoundingBox.h"

namespace MathHelper {

	// зажать значение между переменными
	template <typename TYPE>
	TYPE Clutch(TYPE value, TYPE min, TYPE max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	// округление плавающих
	float Round(float value);

	// поменять значения
	template <typename TYPE>
	void Swap(TYPE* value1, TYPE* value2) {
		TYPE swapValue = *value1;
		*value1 = *value2;
		*value2 = swapValue;
	}

	// расстояние между точками
	float Distance(NumXY<float> point1, NumXY<float> point2);
	float Distance(myXMFLOAT3 point1, myXMFLOAT3 point2);

	// сгенерировать число в заданных пределах
	float RandF(float fromValue, float toValue);

	// вычислить смещение координат по направлению
	NumXY<float> GetDisplacementFromDirection(float angle, float distance);
	// вычислить координаты по направлению
	myXMFLOAT3 GetPositionFromAngle(float angle, XMFLOAT3 position, float radius);

	// возведение в квадрат
	template <typename TYPE>
	TYPE Pow2(TYPE value) { return value * value; };

	// между двумя значениями
	template <typename TYPE>
	bool InBetween(TYPE value, TYPE min, TYPE max) { return (value >= min && value <= max); };

	// проверка на пересечение bounding box
	bool OverlapBox(BoundingBox bb1, BoundingBox bb2);

	// ------------------------
	// преобразования координат
	// ------------------------

	// координаты региона из простых координат
	NumXY<int> GetDynamicRegionAdress(NumXY<float> position); // динамический адрес
	NumXY<int> GetDynamicRegionAdress(myXMFLOAT3 position); // динамический адрес
	NumXY<int> GetAbsRegionAdress(myXMFLOAT3 position); // абсолютный адрес

	// координаты контейнера из простых координат
	NumXY<int> GetContentAdress(NumXY<float> position);
	NumXY<int> GetContentAdress(XMFLOAT3 position);

	// адрес патча из поля патчей из координат региона
	NumXY<int> GetPatchAdress(NumXY<int> regionAdress); // ???

}

#endif // MATHHELPER_H