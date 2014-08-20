#pragma once

#include "Model.h"

// ��� ������� ����������
struct PerSimpleVectorModel {

	PerSimpleVectorModel(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) : position(position), scale(scale), rotation(rotation) {};
	bool Draw(UINT indicesAmount); // ���������
	myXMFLOAT3 position; // ����������
	myXMFLOAT3 scale; // �������
	float rotation; // �������

};

// ���������� ������ �������
class SimpleVectorModel : public Model {

public:

	// ���������� ������ �������
	SimpleVectorModel(bool* result); // �����������

	// ������ �������
	bool Draw(); // ���������

	// �������
	bool CreateInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation); // ������� ���������
	bool ShowFlyInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation); // ������� ���������-������

private:

	// ����������
	std::vector<PerSimpleVectorModel> instances; // ���������� �������
	std::vector<PerSimpleVectorModel> flyInstances; // ����������-������

};