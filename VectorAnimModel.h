#pragma once

#include "AnimModel.h"
#include "PerModel.h"
#include "Terrain.h"

#define ANIM_MODEL_RADIUS 1.0f

// ����� ������� ������� � �������
class VectorAnimModel : public AnimModel {

public:

	// ������ ������� � �������
	VectorAnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, float velocity, bool* result); // �����������
	~VectorAnimModel(); // ����������

	// ������� �������
	bool Update(float dT, NumXY<float> showPoint); // �������� ������ ������� � �������
	bool Draw(bool toShadowMap); // �������� ������ ������� � �������
	bool Spawn(myXMFLOAT3 position); // ������� ���������
	bool Spawn(NumXY<float> position); // ������� ���������

	// �������
	UINT GetAmount() { return modelVector.size(); }; // ���������� �����������
	bool GetDamage(myXMFLOAT3 position, float damage); // �������� ���� � �����
	UINT CheckObject(BoundingBox boundBox); // �������� �� ����������� � ������� �������

protected:

	// ������ �������
	std::vector<PerModel*> modelVector;

	// ����������
	float velocity = 0.0f; // �������� ����������� �����������

	// �������
	virtual PerModel* AddNewModel(myXMFLOAT3 position, UINT bonesAmount) { return (new PerModel(position, clipsAmount, bonesAmount, framesAmount, velocity)); } // ������� ����� ������
	virtual void DestroyModel(PerModel* perModel) { delete perModel; } // ������� ������
	virtual void GiveSomething() {}; // ���� ����� ��������

};
