#pragma once

#include "PerModel.h"
#include "VectorAnimModel.h"

// класс кабанов
class Boar : public PerModel {

public:

	// класс кабанов
	Boar(myXMFLOAT3 position, UINT clipsAmount, UINT bonesAmount, UINT* framesAmount, float velocity) : PerModel(position, clipsAmount, bonesAmount, framesAmount, velocity) {}; // конструктор

};

// класс вектора кабанов
class VectorBoars : public VectorAnimModel {

public:

	// класс вектора кабанов
	VectorBoars(wchar_t* binFilePath, wchar_t* textureFilePath, float velocity, bool* result) : VectorAnimModel(binFilePath, textureFilePath, velocity, result) {}; // конструктор

	// функции
	virtual PerModel* AddNewModel(myXMFLOAT3 position, UINT bonesAmount) { return (new Boar(position, clipsAmount, bonesAmount, framesAmount, velocity)); } // создать нового кабана
	virtual void DestroyModel(PerModel* perModel) { delete (Boar*)perModel; } // удалить одного кабана
	virtual void GiveSomething(); // выдать герою предметы инвентаря

};
