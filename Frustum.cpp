#include "Camera.h"
#include "Mediator.h"

XMVECTOR planes[4]; // �������� ��������� ���������

// �������� �������
void Frustum::ConstructFrustum(float screenDepth, CXMMATRIX projectionMatrix, CXMMATRIX viewMatrix) {

	XMMATRIX projMatrix = projectionMatrix;

	// ���������� ����������� �������� �� Z � ��������.
	float zMinimum = -projMatrix._43 / projMatrix._33;
	float r = screenDepth / (screenDepth - zMinimum);
	projMatrix._33 = r;
	projMatrix._43 = -r * zMinimum;

	// �������� ������� �������� �� ������� � ���������� ������������ ������.
	XMMATRIX matrix = XMMatrixMultiply(viewMatrix, projMatrix);

	float a, b, c, d;

	// ���������� ����� (left) ���������.
	a = matrix._14 + matrix._11;
	b = matrix._24 + matrix._21;
	c = matrix._34 + matrix._31;
	d = matrix._44 + matrix._41;
	planes[0] = XMVectorSet(a, b, c, d);
	planes[0] = XMPlaneNormalize(planes[0]);

	// ���������� ������ (right) ���������.
	a = matrix._14 - matrix._11;
	b = matrix._24 - matrix._21;
	c = matrix._34 - matrix._31;
	d = matrix._44 - matrix._41;
	planes[1] = XMVectorSet(a, b, c, d);
	planes[1] = XMPlaneNormalize(planes[1]);

	// ���������� ������� (top) ���������.
	a = matrix._14 - matrix._12;
	b = matrix._24 - matrix._22;
	c = matrix._34 - matrix._32;
	d = matrix._44 - matrix._42;
	planes[2] = XMVectorSet(a, b, c, d);
	planes[2] = XMPlaneNormalize(planes[2]);

	// ���������� ������ (bottom) ���������.
	a = matrix._14 + matrix._12;
	b = matrix._24 + matrix._22;
	c = matrix._34 + matrix._32;
	d = matrix._44 + matrix._42;
	planes[3] = XMVectorSet(a, b, c, d);
	planes[3] = XMPlaneNormalize(planes[3]);

}

// �������� �����
bool Frustum::CheckPoint(float x, float y, float z) {

	for (int i = 0; i < 4; i++) {
		float ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(x, y, z, 1.0f)));
		if (ret < 0.0f) return false;
	}

	return true;
}


// �������� ����
bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float size) {

	for (int i = 0; i < 4; i++) {

		float ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - size), (yCenter - size), (zCenter - size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + size), (yCenter - size), (zCenter - size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - size), (yCenter + size), (zCenter - size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + size), (yCenter + size), (zCenter - size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - size), (yCenter - size), (zCenter + size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + size), (yCenter - size), (zCenter + size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - size), (yCenter + size), (zCenter + size), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + size), (yCenter + size), (zCenter + size), 1.0f)));
		if (ret >= 0.0f) continue;

		return false;
	}

	return true;
}

// �������� �����
bool Frustum::CheckSphere(myXMFLOAT3 center, float radius) {

	for (int i = 0; i < 4; i++) {
		float ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet(center.x, center.y, center.z, 1.0f)));
		if (ret < -radius) 	return false;
	}

	return true;
}

// �������� ��������������
bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize) {

	for (int i = 0; i < 4; i++) {

		float ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		ret = XMVectorGetX(XMPlaneDotCoord(planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)));
		if (ret >= 0.0f) continue;
		return false;
	}

	return true;
}