#ifndef PERMODEL_H
#define PERMODEL_H

#include "Utilities.h"
#include "Nums.h"
#include "Console.h"
#include "Terrain.h"
#include "AnimModel.h"

#define DEAD_LIE_TIME 5.0f // ����� ������� �����
#define BURY_SPEED 0.5f // �������� ������������ ����� ��� �����
#define BURY_TIME 5.0f // ����� ������������ �� ��������
#define PERS_RADIUS 0.4f // ������� boundingBox �����


// ����� ����������� ��������
class PerModel : public SingleAnimatedModel {

public:

	// ���������
	PerModel(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity); // �����������
	~PerModel(); // ����������

	// ������ �������
	bool Draw(); // ��������� ����������
	bool Update(float dT); // ���������� ����������

	// �������
	static void UpdateParent(AnimModel* parent); // �������� ����������� ���������� ��� ���������	
	bool TakeDamage(float damage); // ��������� ����������� (true - �����)
	bool Die(); // ������ �������

	// ����������
	bool toDelete = false; // ������ ���������� �������
	BoundingBox boundBox; // boundingbox �����

private:

	// ����������
	PERS_STATE persState = PS_STAND; // ��������� ����������

	// ������������ ����������
	NumXY<float> destination = NumXY<float>(0.0f, 0.0f); // ����� ����������
	float waitTime = 0.0f; // ����� �������� �� ����������� ��� ��� ����-������

	// ����������� ����������
	float velocity; // �������� ��������
	static AnimModel* parent; // ��������

	// �������
	bool GoToXY(NumXY<float> newPosition, bool absolute); // ������������� � ����� ����� (���������� ���������� ��� ��������)
	bool Behave(float dT); // ��������� ����������

	// ������������� �������
	float BeIdle(); // ������� �� ����� (���������� �����)
	bool RandomRun(); // �������� ������

};


#endif // PERMODEL_H