#pragma once

#include "Utilities.h"
#include "Structures.h"
#include "Terrain.h"

const int patchVertSize = Const::patchCellSize + 1; // ������ ������� ����� � ��������

// ���� �����
class Patch {

public:

	// ����� �����
	Patch(NumXY<int> bottomLeftCorner, bool* result); // �����������
	~Patch(); // ����������

	// ������ �������
	bool Draw(); // ���������

	// �������
	bool Recreate(); // �������� ���������� �����
	static void FillStatic(int indicesAmount) { Patch::indicesAmount = indicesAmount; }; // ��������� ����������� ����������

	// ����������
	VertMap* verticesData = NULL; // ������ ������ � ������ �����

private:

	// ����������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������ ��� �����

	// ����������� ���������
	static int indicesAmount; // ���������� ��������

};

// ����� �����
class Map {

public:

	// �����
	Map(bool* result); // �����������
	~Map(); // ����������

	// ������ �������
	bool Draw(); // ���������
	bool Update(); // ����������

	// �������
	bool NewRegion(DynamicReg* dynamicRegion); // �������� ����� ������

private:

	// ����������
	ID3D11Buffer* pIndexBuffer = NULL; // ����� �������� ��� �����
	std::vector<DynamicReg*> newRegions; // ����� ������� ��� ���������
	Patch*** patchField = NULL; // ���� ���������� �����

};