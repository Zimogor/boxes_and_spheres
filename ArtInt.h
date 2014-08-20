#ifndef ARTINT_H
#define ARTINT_H

#include "Utilities.h"
#include "Nums.h"
#include "Console.h"
#include "BoundingBox.h"
#include "SubModels.h"

// ����� �������������� ����������
class ArtInt {

public:

	// ������������� ���������
	ArtInt(bool* result); // �����������
	~ArtInt(); // ����������

	// �������� �������
	bool Update(float dT, NumXY<float> showPoint); // ����������
	bool Draw(bool toShadowMap); // ���������

	// �������
	bool TakeDamage(myXMFLOAT3 position, float damage); // ������� ���� � �����
	bool CheckObject(BoundingBox boundBox, bool include); // ���������, �� ������ �� ����� ������ ������ (include - ��� ����� � ������)

private:

	// ������������ ����������
	float persSpawnTime = 1.0f; // ������������� �������� ������

	// �������
	bool TrySpawn(NumXY<float> showPoint, UINT persType); // ���������� ������ �����, true - ���� �����

	// ������� ������
	VectorBoars* boars = NULL;
	VectorAnimModel* devils = NULL;

};

#endif // ARTINT_H