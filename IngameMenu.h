#ifndef INGAMEMENU_H
#define INGAMEMENU_H

#include "Menu.h"
#include "Map.h"

#define ICONS_INVX_AMOUNT 11 // ����� ���������
#define ICONS_INVY_AMOUNT 4 // ������ ���������
#define ICONS_CRAFTX_AMOUNT 3 // ����� ������
#define ICONS_CRAFTY_AMOUNT 1 // ������ ������
#define VERT_AMOUNT_PER_INSTANCE 4 // ���������� ������ �� ���� ��������� ������
#define MAX_ITEM_ASSETS_AMOUNT 1 // ���������� ������ ��� ������

// ��������� ����������-������
struct PerIconInstance {

	XMFLOAT3 position;
	XMFLOAT2 texturePosition;

};

// ��������� �������
struct Recipe {

	Recipe() {};
	Recipe(INVENTORY_ITEM item0, UINT amount0, INVENTORY_ITEM item1, UINT amount1, INVENTORY_ITEM item2, UINT amount2, INVENTORY_ITEM resultItem, UINT resultAmount) {
		ingredient[0].invItem = item0;
		ingredient[0].amount = amount0;
		ingredient[1].invItem = item1;
		ingredient[1].amount = amount1;
		ingredient[2].invItem = item2;
		ingredient[2].amount = amount2;
		result.invItem = resultItem;
		result.amount = resultAmount;
	};
	InventoryContent ingredient[MAX_ITEM_CRAFT_AMOUNT - 1];
	InventoryContent result;

};

// ��������� ��� �������� ��������� ������ ����������
struct MenuTypeDescriptions {

	NumXY<UINT> resolutions; // �������������� ����������
	UINT iconDistance; // ��������� ����� ��������
	UINT iconDistCraft; // ��������� ����� �������� � ������
	NumXY<UINT> invDisp; // �������� �� ������ ������ ��� ���������
	NumXY<int> craftDisp; // �������� ������ �� ������ ������ ��� ��������
	NumXY<int> mainCraftIcon; // �������� ������� ������ ������
	NumXY<int> weaponIcon; // ������ � �������
	UINT iconSize; // ������ ������

};

// ������� ���� � ����������� �� ���������� ������ (������ ��������� � ���������� resolutions)
enum MENU_SIZE { MS_MEDIUM = 0, MS_SMALL = 1 };

// ������������ ������ �������������� ���� (������ ��������� � ������������������ �������� ������ ����� � ��������� ������)
enum INGAME_MENU_BUTTON { MB_INVENTORY = 0, MB_MAP = 1, MB_SKILLTREE = 2, MB_CRAFT = 3 };

// ����� �������������� ����
class IngameMenu : public Menu {

public:

	// ������������� �����
	IngameMenu(bool* result); // �����������
	~IngameMenu(); // ����������

	// ������ �������
	bool Update(); // ����������
	bool MapUpdate() { return map->Update(); }; // ���������� �����
	bool Draw(); // ���������
	bool Reset(); // �����

	// �������
	bool Open(); // �������� ����
	bool Close(); // �������� ����

private:

	// ����������
	std::vector<MenuTypeDescriptions> menuDescs; // �������������� ���� � ������� ���������� ��������
	MENU_SIZE menuSize; // ������ ����
	MENU_SIZE prevMenuSize; // ���������� ������ ����
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	ID3D11Buffer* pInstanceBuffer = NULL; // ����� ����������� ������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������ ������
	Font* font = NULL; // ����� ������
	Recipe* recipes = NULL; // ������ ��������
	UINT recipesAmount = 0; // ���������� ��������
	Map* map = NULL; // �����

	// �������
	MENU_SIZE DefineMenuSize(); // ���������� ���������� ������ ����
	bool ChangeMenuSize(MENU_SIZE menuSize); // �������� ������ ����
	bool CreateSmallMenu(); // ������� ���� ��� ������ ����������
	bool CreateMediumMenu(); // ������� ���� ��� ������ ����������
	bool ReCreateInstanceBuffer(); // ������� ����� ����������� (������ ��� ������ � ��������� �������������)
	bool ReCreateIconVB(); // ������� ����� ������ ������ (������ ��� ������ � ��������� �������������)
	bool RecreateFont(); // ����������� ����� ������
	XMFLOAT2 GetTextureFromItem(INVENTORY_ITEM item); // ������� ���������� �������� �� ������
	bool RebuildInventory(); // ����������� ��������� (������ ��� ����� �������� ����)
	XMFLOAT3 GetIconPosition(UINT iconNumber); // ���������� ������ �� � ������
	int GetIconNumber(NumXY<int> position); // ����� ������ �� ����������� (-1, ���� ��������)
	bool CalculateIcons(); // ������ � ��������
	bool makeCraft = false; // ������� ����� �������
	bool CreateRecipes(); // ������� �������
	bool CraftNewItem(PerIconInstance* icons); // ������� ����� �������

};

#endif // INGAMEMENU_H