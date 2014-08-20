#pragma once

#include "Utilities.h"
#include "Nums.h"
#include "MathHelper.h"
#include "Console.h"
#include "Errors.h"
#include "Constants.h"
#include "Terrain.h"

#define NEAR_FAR_Z NumXY<float>(0.3f, 1000.0f) // плоскости дл€ стандартной проекции
#define NEAR_FAR_ORTHOZ NumXY<float>(0.01f, 100.0f) // плокости дл€ ортографической проекции

// класс фрустума
class Frustum {

public:

	// функции
	void ConstructFrustum(float screenDepth, CXMMATRIX projectionMatrix, CXMMATRIX viewMatrix); // построить фрустум
	bool CheckPoint(float x, float y, float z); // проверка точки
	bool CheckCube(float xCenter, float yCenter, float zCenter, float size); // проверка куба
	bool CheckSphere(myXMFLOAT3 center, float radius); // проверка сферы
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize); // проверка пр€моугольника

};

// класс камеры
class Camera {

public:

	// камера
	Camera(bool* result); // конструктор

	// важные функции
	bool Reset(); // сброс камеры
	bool Update(float dT, myXMFLOAT3 heroPosition); // обновление камеры

	// функции
	myXMFLOAT3 GetCamFocus() { return camFocus; }; // вернуть координаты фокуса камеры
	float GetCamAzimuth() { return azimuth; }; // вернуть азимут камеры

	// извлечение параметров
	myXMFLOAT3 GetPosition() { return camPosition; }; // координаты камеры
	Frustum* GetFrustum() { return &frustum; }; // фрустум камеры
	XMMATRIX GetViewMatrix(); // матрица камеры
	XMMATRIX GetProjMatrix(); // матрица проекции
	float GetRadius() { return radius; }; // радиус камеры


private:

	// функции
	bool RebuildFrustum(); // перестроить фрустум

	// переменные
	float azimuth; // азимут камеры
	float pitch = Const::releaseCamera ? 1.0f : 0.5f; // тангаж камеры
	float radius = Const::releaseCamera ? Const::maxRadius : 45.0f; // радиус камеры
	Frustum frustum; // фрустум

	// важные переменные
	myXMFLOAT3 camPosition; // координаты камеры
	myXMFLOAT3 camFocus = Const::spawnPoint; // координаты фокуса

	// константы
	const NumXY<float> NearFarZ = NEAR_FAR_Z; // дальна€ и ближн€€ плоскость в обычной проекции
	const NumXY<float> NearFarOZ = NEAR_FAR_ORTHOZ; // дальн€€ и ближн€€ плоскость в ортографической проекции
	const float rotationSpeed = 0.005f; // скорость вращени€ камеры
	const float minRadius = Const::minRadius; // минимальное приближение

};
