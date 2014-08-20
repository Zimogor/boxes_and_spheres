#ifndef ANIMMODEL_H
#define ANIMMODEL_H

#include "Utilities.h"
#include "Structures.h"
#include "Constants.h"
#include "MathHelper.h"

#define ANGLE_SPEED 5.0f // ������� �������� �� ���������
#define ATACK_ANGLE_SPEED 20.0f // �������� �������� �� ����� �����
#define FAST_CLIP_BLEND_SPEED 10.0f // ���������� (��� �����) �������� �������� ��������
#define ATTACK_DISTANCE 1.0f // ���������� �����
#define MODEL_LIFE 15.0f // ���������� ������ ������

// ��, ��� ������ �� ������ � �������
class SingleAnimatedModel {

public:

	// �����
	SingleAnimatedModel() { hitPoints = maxHitPoints; }; // �����������
	~SingleAnimatedModel() { delete[] finalMatrices; }; // ����������

	// ����������
	myXMFLOAT3 position; // ���������� �������

	// ���������� ����� ����
	float SetCurrentClip(UINT clipNumber); // ���������, ���������� ����� �����
	float SetCurrentClip(UINT clipNumber, UINT nextClipNumber); // ������� ���� ���� ������ �����, ���������� ����� �����

	// �������
	bool Update(float dT); // ����������

protected:

	// �������
	XMFLOAT4X4 GetRotationMatrix() { return rotationMatrix; }; // ������� ������� ��������
	float GetDrawAngle() { return drawAngle; }; // ������� ���� ��������
	void SetAngleSpeed(float angleSpeed) { this->angleSpeed = angleSpeed; }; // ���������� �������� ��������
	void SetClipBlendSpeed(float clipBlendSpeed) { this->clipBlendSpeed = clipBlendSpeed; }; // ���������� �������� �������� ��������
	void SetRotAngle(float rotAngle) { this->rotAngle = rotAngle; }; // ���������� ���� ��� ��������
	void InitFinalMatrices(UINT bonesAmount) { finalMatrices = new XMFLOAT4X4[bonesAmount]; }; // ������� ������ ��������� ������ ������

	// ����������
	std::vector<UINT> curFrame; // ������� ���� ��� ������ �� ��������
	std::vector<float> accumulation; // ���������� �������� ����� ��� ������ �� ��������
	std::vector<float> blendFactors; // ���� ��� ������� �� ������
	UINT* framesAmount = NULL; // ���������� ������ �������� ������� �����
	UINT clipsAmount = 0; // ���������� ������ ��������
	UINT nextClip = 0; // ��������� ����
	UINT curClip = 0; // ����� �������� �����
	float drawAngle = DRAW_ANGLE_DEFAULT; // ���� ��������� �������
	XMFLOAT4X4 rotationMatrix; // ������� ��������
	bool restCurClip = true; // ��������� ����� �������� ����� (��� �������� ��������)
	bool recreateFinalMatrices = true; // ���������� �������� ������� ������
	XMFLOAT4X4* finalMatrices = NULL; // ��� �������� ������
	float damageWaitPoint = 0.0f; // ����� ����� ������� � ��������� �����
	float maxHitPoints = MODEL_LIFE; // ������������ ���������� ������
	float hitPoints = 0.0f; // ���� ������

private:

	float rotAngle = 0.0f; // ���� ��������
	float clipBlendSpeed = Const::clipBlendSpeed; // �������� �������� ��������
	float angleSpeed = ANGLE_SPEED; // �������� �������� ��������
	bool waitNextClip = false; // ����������� ����, ��� ����� ����������
	float nextClipTime = 0.0f; // ���������� �� ����� ������������ �����

};

class PerModel;

// ��������� �����
struct Clip {

	~Clip() { ReleaseNULLS(bonesPositions); ReleaseNULLS(bonesQuaternions); }; // ����������
	XMFLOAT3* bonesPositions = NULL; // ������ ������� ������
	XMFLOAT4* bonesQuaternions = NULL; // ������ ������������

};

// ������ � �������
class AnimModel : public SingleAnimatedModel {

	friend PerModel;

public:

	// ������ � �������
	AnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, bool* result); // �����������
	~AnimModel(); // ����������

	// ������ �������
	bool Draw(bool toShadowMap); // �������� ������
	myXMFLOAT3 GetPosition() { return position; }; // ������� ���������� �������

protected:

	// ������� ��������� ������� ��� �������
	void CreateFinalMatricesOldStyle(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices); // ��� ���������� ������ �� �������� ��������� (��� �������)
	void CreateFinalMatrices(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices); // ��� ���������� ������
	void CreateFinalMatrices(UINT clipNumber1, UINT frameNumber1, UINT clipNumber2, UINT frameNumber2, float blendFactor, XMFLOAT4X4* finalMatrices); // ���������� ���� ������
	void CreateFinalMatrices(std::vector<UINT> frameNumbers, std::vector<float> blendFactors, XMFLOAT4X4* finalMatrices); // ���������� ����������� ������

	// ����������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������
	ID3D11Buffer* pIndexBuffer = NULL; // ����� ��������
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	UINT bonesAmount = 0; // ���������� ������
	UINT indicesAmount = 0; // ���������� ��������
	std::vector<Clip> clips; // ������ ������

private:

	// �������
	bool BuildOrder(); // ��������� ������ order ��� �������� ������
	bool LoadAmimModelFromFile(wchar_t* binFilePath); // ��������� ������ �� �����

	// ����������
	std::vector<int> hierarchy; // �������� ������ (�������� ��� ������ �����)
	std::vector<int> order; // ������� ��������� ������ ��� ���������� final
	std::vector<XMFLOAT4X4> offsetMatrices; // ������� offset

};

#endif // ANIMMODEL_H