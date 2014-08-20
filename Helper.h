#pragma once

#include "SimpleVectorModel.h"
#include "Font.h"

#define FLOAT_TEXT_VELOCITY 1.0f
#define FLOAT_TEXT_LIFETIME 2.0f


// �������� ��������� ������
struct PerFloatText {

	PerFloatText(XMFLOAT3 position) : lifeTime(FLOAT_TEXT_LIFETIME), position(position) {}; // �������������
	XMFLOAT3 position; // ����������
	float lifeTime; // ����� �����

};

// ����� ���������
class Helper {

public:

	// ����� ���������
	Helper(bool *result); // �����������
	~Helper(); // ����������

	// ������ �������
	bool Update(float dT); // ����������
	bool Draw(); // ���������

	// ��������� �����
	template <typename TYPE>
	bool SetFloatText(myXMFLOAT3 position, TYPE text, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3());
	bool SetFloatText(myXMFLOAT3 position, const wchar_t* text, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3());

	// �������
	bool SetBox(myXMFLOAT3 position, myXMFLOAT3 scale = myXMFLOAT3(1.0f), float rotation = 0.0f); // ������� �����
	bool SetFlyBox(myXMFLOAT3 position, myXMFLOAT3 scale = myXMFLOAT3(1.0f), float rotation = 0.0f); // �������� ����� ������

	// ��������� ��������� �����
	ID3D11Buffer* pVertexBoxBuffer = NULL; // ����� ������ ��� ���������� �����
	ID3D11Buffer* pIndexBoxBuffer = NULL; // ����� �������� ��� ���������� �����
	UINT boxIndicesAmount = 0; // ���������� �������� �����

	// ��������� ��������� ��������
	ID3D11Buffer* pVertexCylinderBuffer = NULL; // ����� ������ ���������� ��������
	ID3D11Buffer* pIndexCylinderBuffer = NULL; // ����� �������� ���������� ��������
	UINT cylinderIndicesAmount = 0; // ���������� �������� ��������
	
private:

	// ����������
	SimpleVectorModel* box = NULL; // ������
	Font* font = NULL; // �����
	std::vector<PerFloatText> sentenceParams; // ��������� ���������� ������

	// �������
	bool CreateCylinder(); // ������� �������

};

// ������ ���������� ������
template<typename TYPE>
bool Helper::SetFloatText(myXMFLOAT3 position, TYPE text, bool specialColorB, XMFLOAT3 specialColorF) {
	return SetFloatText(position, std::to_wstring(text).c_str(), specialColorB, specialColorF);
}