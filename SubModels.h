#pragma once

#include "PerModel.h"
#include "VectorAnimModel.h"

// ����� �������
class Boar : public PerModel {

public:

	// ����� �������
	Boar(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity) : PerModel(position, clipsAmount, bonesAmount, framesAmount, velocity) {}; // �����������

};

// ����� ������� �������
class VectorBoars : public VectorAnimModel {

public:

	// ����� ������� �������
	VectorBoars(wchar_t* binFilePath, wchar_t* textureFilePath, float velocity, bool* result) : VectorAnimModel(binFilePath, textureFilePath, velocity, result) {}; // �����������

	// �������
	virtual PerModel* AddNewModel(myXMFLOAT3 position, UINT bonesAmount) { return (new Boar(position, clipsAmount, bonesAmount, framesAmount, velocity)); } // ������� ������ ������
	virtual void DestroyModel(PerModel* perModel) { delete (Boar*)perModel; } // ������� ������ ������
	virtual void GiveSomething(); // ������ ����� �������� ���������

};
