#pragma once

#include "Utilities.h"
#include "Nums.h"
#include "MathHelper.h"
#include "Console.h"
#include "Errors.h"
#include "Constants.h"
#include "Terrain.h"

#define NEAR_FAR_Z NumXY<float>(0.3f, 1000.0f) // ��������� ��� ����������� ��������
#define NEAR_FAR_ORTHOZ NumXY<float>(0.01f, 100.0f) // �������� ��� ��������������� ��������

// ����� ��������
class Frustum {

public:

	// �������
	void ConstructFrustum(float screenDepth, CXMMATRIX projectionMatrix, CXMMATRIX viewMatrix); // ��������� �������
	bool CheckPoint(float x, float y, float z); // �������� �����
	bool CheckCube(float xCenter, float yCenter, float zCenter, float size); // �������� ����
	bool CheckSphere(myXMFLOAT3 center, float radius); // �������� �����
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize); // �������� ��������������

};

// ����� ������
class Camera {

public:

	// ������
	Camera(bool* result); // �����������

	// ������ �������
	bool Reset(); // ����� ������
	bool Update(float dT, myXMFLOAT3 heroPosition); // ���������� ������

	// �������
	myXMFLOAT3 GetCamFocus() { return camFocus; }; // ������� ���������� ������ ������
	float GetCamAzimuth() { return azimuth; }; // ������� ������ ������

	// ���������� ����������
	myXMFLOAT3 GetPosition() { return camPosition; }; // ���������� ������
	Frustum* GetFrustum() { return &frustum; }; // ������� ������
	XMMATRIX GetViewMatrix(); // ������� ������
	XMMATRIX GetProjMatrix(); // ������� ��������
	float GetRadius() { return radius; }; // ������ ������


private:

	// �������
	bool RebuildFrustum(); // ����������� �������

	// ����������
	float azimuth; // ������ ������
	float pitch = Const::releaseCamera ? 1.0f : 0.5f; // ������ ������
	float radius = Const::releaseCamera ? Const::maxRadius : 45.0f; // ������ ������
	Frustum frustum; // �������

	// ������ ����������
	myXMFLOAT3 camPosition; // ���������� ������
	myXMFLOAT3 camFocus = Const::spawnPoint; // ���������� ������

	// ���������
	const NumXY<float> NearFarZ = NEAR_FAR_Z; // ������� � ������� ��������� � ������� ��������
	const NumXY<float> NearFarOZ = NEAR_FAR_ORTHOZ; // ������� � ������� ��������� � ��������������� ��������
	const float rotationSpeed = 0.005f; // �������� �������� ������
	const float minRadius = Const::minRadius; // ����������� �����������

};
