#pragma once

#include "Utilities.h"
#include "Sprite.h"

// ����� ������� ��������
class RedLiquid : public Sprite {

public:

	// ������ ��������
	RedLiquid(bool* result); // �����������

	// �������
	bool SetLevel(float amount); // ���������� ���������� ���� � ���������

private:

	// �������� ������ ������
	bool CreateVertexBuffer(); // ������� ����� ������

};

// ����� HUD
class HUD : public Sprite {

public:

	// ����� HUD
	HUD(bool* result); // �����������
	~HUD(); // ����������

	// ������ �������
	bool Reset(); // �����
	bool Draw(); // ���������

	// �������
	bool SetRedLevel(float amount); // ���������� ����� ������� ������� ���� � ���������

private:

	// ����������
	RedLiquid* redLiquid = NULL; // ������� �������� ��� ������

};