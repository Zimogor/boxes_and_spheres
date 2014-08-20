#include "MathHelper.h"
#include "Mediator.h"

// ���������� ���������
float MathHelper::Round(float value) {

	return ((int)(value * 10000) * 0.0001f);

}

// ���������� ����� �������
float MathHelper::Distance(NumXY<float> point1, NumXY<float> point2) {
	return sqrt(Pow2(point1.x - point2.x) + Pow2(point1.y - point2.y));
}
float MathHelper::Distance(myXMFLOAT3 point1, myXMFLOAT3 point2) {
	return sqrt(Pow2(point1.x - point2.x) + Pow2(point1.z - point2.z));
}

// ������������� ����� � �������� ��������
float MathHelper::RandF(float fromValue, float toValue) {

	return (float)rand() / RAND_MAX * (toValue - fromValue) + fromValue;
}

// ��������� �������� ��������� �� �����������
NumXY<float> MathHelper::GetDisplacementFromDirection(float angle, float distance) {

	return NumXY<float>(distance * cos(angle), distance * sin(angle));
}
// ��������� ���������� �� �����������
myXMFLOAT3 MathHelper::GetPositionFromAngle(float angle, XMFLOAT3 position, float radius) {

	return myXMFLOAT3(position.x - sin(angle) * radius, position.y, position.z - cos(angle) * radius);
}

// ���������� ������� �� ������� ���������
NumXY<int> MathHelper::GetDynamicRegionAdress(NumXY<float> position) { // ������������ �����

	int x = (int)floor(position.x / Const::regionCellSize - Mediator::cornerPoint->x);
	int y = (int)floor(position.y / Const::regionCellSize - Mediator::cornerPoint->y);
	if (x < 0 || y < 0 || x >= Const::regionFieldSize || y >= Const::regionFieldSize) return NumXY<int>(-1, -1); // ����� �� ������� �������
	return NumXY<int>(x, y);

}
NumXY<int> MathHelper::GetDynamicRegionAdress(myXMFLOAT3 position) { // ������������ �����

	int x = (int)floor(position.x / Const::regionCellSize - Mediator::cornerPoint->x);
	int z = (int)floor(position.z / Const::regionCellSize - Mediator::cornerPoint->y);
	if (x < 0 || z < 0 || x >= Const::regionFieldSize || z >= Const::regionFieldSize) return NumXY<int>(-1, -1); // ����� �� ������� �������
	return NumXY<int>(x, z);

}
NumXY<int> MathHelper::GetAbsRegionAdress(myXMFLOAT3 position) { // ���������� �����

	return NumXY<int>((int)floor(position.x / Const::regionCellSize), (int)floor(position.z / Const::regionCellSize));

}

// ���������� ���������� �� ������� ���������
NumXY<int> MathHelper::GetContentAdress(NumXY<float> position) {

	// cornerPoint �� �����
	int x, y;
	x = position.x >= 0 ? (int)position.x % Const::regionCellSize : (int)position.x % Const::regionCellSize + Const::regionCellSize - 1;
	y = position.y >= 0 ? (int)position.y % Const::regionCellSize : (int)position.y % Const::regionCellSize + Const::regionCellSize - 1;
	return NumXY<int>(x, y);

}
NumXY<int> MathHelper::GetContentAdress(XMFLOAT3 position) {

	// cornerPoint �� �����
	int x, z;
	x = position.x >= 0 ? (int)position.x % Const::regionCellSize : (int)position.x % Const::regionCellSize + Const::regionCellSize - 1;
	z = position.z >= 0 ? (int)position.z % Const::regionCellSize : (int)position.z % Const::regionCellSize + Const::regionCellSize - 1;
	return NumXY<int>(x, z);

}

// �������� �� ����������� bounding box
bool MathHelper::OverlapBox(BoundingBox bb1, BoundingBox bb2) {

	if (!bb1.active || !bb2.active) return false;
	float distance = Distance(bb1.position, bb2.position);
	return distance < bb1.radius + bb2.radius;;

}

// ����� ����� �� ���� ������ �� ��������� ������� ??? ��������� � ���������
NumXY<int> MathHelper::GetPatchAdress(NumXY<int> regionAdress) {

	NumXY<int> result;
	int regionsInPatch = Const::patchCellSize / Const::regionCellSize;

	result.x = (int)floor((float)regionAdress.x / regionsInPatch) + Const::patchFieldSize / 2;
	result.y = (int)floor((float)regionAdress.y / regionsInPatch) + Const::patchFieldSize / 2;

	return result;

}