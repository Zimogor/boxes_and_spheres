#include "Camera.h"
#include "Mediator.h"

XMMATRIX projMatrix; // матрица проекции
XMMATRIX orthoMatrix; // матрица отрографической проекции
XMMATRIX viewMatrix; // матрица камеры

// конструктор камеры
Camera::Camera(bool* result) {

	azimuth = -1.2f;

	// создать матрицу проекции
	float ratio = (float)Mediator::winDimentions->x / Mediator::winDimentions->y;
	projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, ratio, NearFarZ.x, NearFarZ.y);

	// создать матрицу ортографической проекции
	orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, NearFarOZ.x, NearFarOZ.y);

	// запись в буфер шейдера
	BRR(Mediator::shader->SetResetBuffer(orthoMatrix));

	// вычисление координат камеры
	camPosition.x = radius * sin(azimuth) * cos(pitch);
	camPosition.z = radius * cos(azimuth) * cos(pitch);
	camPosition.y = radius * sin(pitch);
	camPosition += camFocus;

	Mediator::camera = this;
	*result = true;

}

// перестроить фрустум
bool Camera::RebuildFrustum() {

	frustum.ConstructFrustum(NearFarZ.y, projMatrix, viewMatrix);
	return true;
}

// сброс камеры
bool Camera::Reset() {

	// пересоздать матрицу проеции
	float ratio = (float)Mediator::winDimentions->x / Mediator::winDimentions->y;
	projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, ratio, NearFarZ.x, NearFarZ.y);

	// пересоздать матрицу ортографической проекции
	orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, NearFarOZ.x, NearFarOZ.y);

	// запись в буфер шейдера
	BR(Mediator::shader->SetResetBuffer(orthoMatrix));

	// перестроить фрустум
	BR(RebuildFrustum());

	// передача в шейдер
	XMVECTOR focus_JP = { camFocus.x - camPosition.x, camFocus.y - camPosition.y, camFocus.z - camPosition.z };
	XMMATRIX V_JP = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), focus_JP, camUp);
	BR(Mediator::shader->SetCamBuffer(V_JP * projMatrix, XMFLOAT4(camPosition.x, camPosition.y, camPosition.z, 1.0f), XMFLOAT4(camFocus.x, camFocus.y, camFocus.z, 1.0f)));

	return true;

}

// обновление камеры
bool Camera::Update(float dT, myXMFLOAT3 heroPosition) {

	camFocus = heroPosition;

	// обновление радиуса камеры
	if (Mediator::mouseChange->wheelShift) {
		radius -= Const::scrollSpeed * Mediator::mouseChange->wheelShift * (radius + 5);
		radius = MathHelper::Clutch(radius, minRadius, Const::maxRadius);
	}
	// обновление азимута
	if (Mediator::mouseChange->cursorShift.x && Mediator::mouseChange->holdButtons[1]) {
		azimuth += Mediator::mouseChange->cursorShift.x * rotationSpeed;
		if (azimuth < -XM_2PI) azimuth += XM_2PI;
		if (azimuth > XM_2PI) azimuth -= XM_2PI;
	}
	// обновление тангажа
	if (!Const::releaseCamera && Mediator::mouseChange->cursorShift.y && Mediator::mouseChange->holdButtons[1]) {
		pitch += Mediator::mouseChange->cursorShift.y * rotationSpeed;
		pitch = MathHelper::Clutch(pitch, Const::pitchLimit.x, Const::pitchLimit.y);
	}

	// вычисление координат камеры
	camPosition.x = radius * sin(azimuth) * cos(pitch);
	camPosition.z = radius * cos(azimuth) * cos(pitch);
	camPosition.y = radius * sin(pitch);
	camPosition += camFocus;

	// передача данных в шейдер
	XMVECTOR focus_JP = { camFocus.x - camPosition.x, camFocus.y - camPosition.y, camFocus.z - camPosition.z };
	XMMATRIX V_JP = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), focus_JP, camUp);
	BR(Mediator::shader->SetCamBuffer(V_JP * projMatrix, XMFLOAT4(camPosition.x, camPosition.y, camPosition.z, 1.0f), XMFLOAT4(camFocus.x, camFocus.y, camFocus.z, 1.0f)));

	// вычисление абсолютных матриц камеры
	XMVECTOR vCamPos = XMVectorSet(camPosition.x, camPosition.y, camPosition.z, 1.0f);
	XMVECTOR vCamFocus = XMVectorSet(camFocus.x, camFocus.y, camFocus.z, 1.0f);
	viewMatrix = XMMatrixLookAtLH(vCamPos, vCamFocus, camUp);

	// построение фрустума
	BR(RebuildFrustum());

	return true;

}

// извлечение матрицы проекции
XMMATRIX Camera::GetProjMatrix() {
	return projMatrix;
}

// извлечение матрицы камеры
XMMATRIX Camera::GetViewMatrix() {
	return viewMatrix;
}