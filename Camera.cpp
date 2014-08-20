#include "Camera.h"
#include "Mediator.h"

XMMATRIX projMatrix; // ������� ��������
XMMATRIX orthoMatrix; // ������� ��������������� ��������
XMMATRIX viewMatrix; // ������� ������

// ����������� ������
Camera::Camera(bool* result) {

	azimuth = -1.2f;

	// ������� ������� ��������
	float ratio = (float)Mediator::winDimentions->x / Mediator::winDimentions->y;
	projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, ratio, NearFarZ.x, NearFarZ.y);

	// ������� ������� ��������������� ��������
	orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, NearFarOZ.x, NearFarOZ.y);

	// ������ � ����� �������
	BRR(Mediator::shader->SetResetBuffer(orthoMatrix));

	// ���������� ��������� ������
	camPosition.x = radius * sin(azimuth) * cos(pitch);
	camPosition.z = radius * cos(azimuth) * cos(pitch);
	camPosition.y = radius * sin(pitch);
	camPosition += camFocus;

	Mediator::camera = this;
	*result = true;

}

// ����������� �������
bool Camera::RebuildFrustum() {

	frustum.ConstructFrustum(NearFarZ.y, projMatrix, viewMatrix);
	return true;
}

// ����� ������
bool Camera::Reset() {

	// ����������� ������� �������
	float ratio = (float)Mediator::winDimentions->x / Mediator::winDimentions->y;
	projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, ratio, NearFarZ.x, NearFarZ.y);

	// ����������� ������� ��������������� ��������
	orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, NearFarOZ.x, NearFarOZ.y);

	// ������ � ����� �������
	BR(Mediator::shader->SetResetBuffer(orthoMatrix));

	// ����������� �������
	BR(RebuildFrustum());

	// �������� � ������
	XMVECTOR focus_JP = { camFocus.x - camPosition.x, camFocus.y - camPosition.y, camFocus.z - camPosition.z };
	XMMATRIX V_JP = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), focus_JP, camUp);
	BR(Mediator::shader->SetCamBuffer(V_JP * projMatrix, XMFLOAT4(camPosition.x, camPosition.y, camPosition.z, 1.0f), XMFLOAT4(camFocus.x, camFocus.y, camFocus.z, 1.0f)));

	return true;

}

// ���������� ������
bool Camera::Update(float dT, myXMFLOAT3 heroPosition) {

	camFocus = heroPosition;

	// ���������� ������� ������
	if (Mediator::mouseChange->wheelShift) {
		radius -= Const::scrollSpeed * Mediator::mouseChange->wheelShift * (radius + 5);
		radius = MathHelper::Clutch(radius, minRadius, Const::maxRadius);
	}
	// ���������� �������
	if (Mediator::mouseChange->cursorShift.x && Mediator::mouseChange->holdButtons[1]) {
		azimuth += Mediator::mouseChange->cursorShift.x * rotationSpeed;
		if (azimuth < -XM_2PI) azimuth += XM_2PI;
		if (azimuth > XM_2PI) azimuth -= XM_2PI;
	}
	// ���������� �������
	if (!Const::releaseCamera && Mediator::mouseChange->cursorShift.y && Mediator::mouseChange->holdButtons[1]) {
		pitch += Mediator::mouseChange->cursorShift.y * rotationSpeed;
		pitch = MathHelper::Clutch(pitch, Const::pitchLimit.x, Const::pitchLimit.y);
	}

	// ���������� ��������� ������
	camPosition.x = radius * sin(azimuth) * cos(pitch);
	camPosition.z = radius * cos(azimuth) * cos(pitch);
	camPosition.y = radius * sin(pitch);
	camPosition += camFocus;

	// �������� ������ � ������
	XMVECTOR focus_JP = { camFocus.x - camPosition.x, camFocus.y - camPosition.y, camFocus.z - camPosition.z };
	XMMATRIX V_JP = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), focus_JP, camUp);
	BR(Mediator::shader->SetCamBuffer(V_JP * projMatrix, XMFLOAT4(camPosition.x, camPosition.y, camPosition.z, 1.0f), XMFLOAT4(camFocus.x, camFocus.y, camFocus.z, 1.0f)));

	// ���������� ���������� ������ ������
	XMVECTOR vCamPos = XMVectorSet(camPosition.x, camPosition.y, camPosition.z, 1.0f);
	XMVECTOR vCamFocus = XMVectorSet(camFocus.x, camFocus.y, camFocus.z, 1.0f);
	viewMatrix = XMMatrixLookAtLH(vCamPos, vCamFocus, camUp);

	// ���������� ��������
	BR(RebuildFrustum());

	return true;

}

// ���������� ������� ��������
XMMATRIX Camera::GetProjMatrix() {
	return projMatrix;
}

// ���������� ������� ������
XMMATRIX Camera::GetViewMatrix() {
	return viewMatrix;
}