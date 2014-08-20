#ifndef MATHHELPER_H
#define MATHHELPER_H

#include "Nums.h"
#include "Constants.h"
#include "BoundingBox.h"

namespace MathHelper {

	// ������ �������� ����� �����������
	template <typename TYPE>
	TYPE Clutch(TYPE value, TYPE min, TYPE max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	// ���������� ���������
	float Round(float value);

	// �������� ��������
	template <typename TYPE>
	void Swap(TYPE* value1, TYPE* value2) {
		TYPE swapValue = *value1;
		*value1 = *value2;
		*value2 = swapValue;
	}

	// ���������� ����� �������
	float Distance(NumXY<float> point1, NumXY<float> point2);
	float Distance(myXMFLOAT3 point1, myXMFLOAT3 point2);

	// ������������� ����� � �������� ��������
	float RandF(float fromValue, float toValue);

	// ��������� �������� ��������� �� �����������
	NumXY<float> GetDisplacementFromDirection(float angle, float distance);
	// ��������� ���������� �� �����������
	myXMFLOAT3 GetPositionFromAngle(float angle, XMFLOAT3 position, float radius);

	// ���������� � �������
	template <typename TYPE>
	TYPE Pow2(TYPE value) { return value * value; };

	// ����� ����� ����������
	template <typename TYPE>
	bool InBetween(TYPE value, TYPE min, TYPE max) { return (value >= min && value <= max); };

	// �������� �� ����������� bounding box
	bool OverlapBox(BoundingBox bb1, BoundingBox bb2);

	// ------------------------
	// �������������� ���������
	// ------------------------

	// ���������� ������� �� ������� ���������
	NumXY<int> GetDynamicRegionAdress(NumXY<float> position); // ������������ �����
	NumXY<int> GetDynamicRegionAdress(myXMFLOAT3 position); // ������������ �����
	NumXY<int> GetAbsRegionAdress(myXMFLOAT3 position); // ���������� �����

	// ���������� ���������� �� ������� ���������
	NumXY<int> GetContentAdress(NumXY<float> position);
	NumXY<int> GetContentAdress(XMFLOAT3 position);

	// ����� ����� �� ���� ������ �� ��������� �������
	NumXY<int> GetPatchAdress(NumXY<int> regionAdress); // ???

}

#endif // MATHHELPER_H