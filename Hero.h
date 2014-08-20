#ifndef HERO_H
#define HERO_H

#include "AnimModel.h"
#include "Model.h"

#define HERO_RADIUS 0.3f // ������ �����

// ����� ����������� (������)
class Tool;

// ����� �����
class Hero : public AnimModel {

public:

	// �����
	Hero(bool* result); // �����������
	~Hero(); // ����������

	// ������ �������
	bool Update(float dT, float camAzimuth); // ���������� �����
	bool Draw(bool toShadowMap); // ��������� �����

	// �������
	void ThrustIntoInventory(InventoryContent invContent); // ������ ������� � ���������
	bool TakeDamage(myXMFLOAT3 damagePoint, float damage); // ���������� �������� ����
	bool TakeDamage(float damage); // �������� ���������� ����
	void ChangeTool(); // ������� ������

private:

	// ����������
	HERO_STATE heroState; // ��������� �����
	float freezeTime = 0.0f; // ����� �������������� ��� ����� ���� ��������
	float waitPoint0 = 0.0f; // ��������������� ��������� ��������
	float waitPoint1 = 0.0f; // ��������������� ��������� ��������
	InventoryContent inventory[MAX_ITEM_AMOUNT]; // ���������
	BoundingBox boundBox; // bounding box
	std::vector<Tool*> tools; // ��������� ��� ������
	int curToolIndex = -1; // ����� �������� ������ � ����������

	// �������
	INVENTORY_ITEM CellItemIntoInvenotryItem(CELL_ITEM cellItem); // ������������ ������� ������ �������� ���������
	bool CalculatePicking(HERO_STATE* prevState); // ��������� ����������, true - ���� ��������
	bool CalculateAttacking(HERO_STATE *prevState); // ��������� ��������, true - ���� ��������
	bool Die(); // ����� �����

};

// ������ (��, ��� ����� ������� � ����)
class Tool : public Model {

public:

	// ����� �����������
	Tool(wchar_t* textureFilePath, wchar_t* binFilePath, float damage, HERO_CLIPS standClip, HERO_CLIPS attackClip, bool* result) : Model(textureFilePath, binFilePath, myXMFLOAT3(0.0f, 3.0f, 0.0f), result), damage(damage), standClip(standClip), attackClip(attackClip) {}; // �����������

	// ������������ �������
	bool(Tool::*Action)(myXMFLOAT3 actionPoint) = NULL;
	bool ToolAction(myXMFLOAT3 actionPoint) { return (this->*Action)(actionPoint); };

	// ��������� ��������
	bool PickAction(myXMFLOAT3 actionPoint); // ���� ������
	bool AxeAction(myXMFLOAT3 actionPoint); // ���� �������

	// ����������
	float damage = 0.0f; // ����������� �������
	HERO_CLIPS standClip = HC_BATSTAND;
	HERO_CLIPS attackClip = HC_ATTACK0;

};

#endif // HERO_H