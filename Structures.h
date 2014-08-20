#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "Nums.h"
#include "Utilities.h"

#define CELL_ITEM int

// ��������� ������� �������
struct CbufAnimModel { // ��� ������������ �������

	XMFLOAT4X4 gWorld; // ������� �������
	XMFLOAT4X4 gBones[32]; // ������ ������ ������
};
struct CbufReset { // ��� ������

	XMFLOAT4X4 gOrthoMatrix; // ������� ��������������� �������������
};
struct CbufObject { // ��� ��������

	XMFLOAT4X4 gWorld; // ������� �������
};
struct CbufFrame1 { // ������������ �������������

	XMFLOAT4 lightVector; // ����������� ����� (����������)
};
struct CbufFont { // ��� �������

	XMFLOAT4 fontColor; // ���� ������
};
struct CbufFrame2 { // ��� ������� �����

	XMFLOAT4 camPos; // ���������� ������
	XMFLOAT4 camFocus; // ����� ������
	XMFLOAT4X4 gViewProjMatrix; // ������� ������
};
struct CbufFrame3 { // ��� ����� �����

	XMFLOAT4X4 gViewLightMatrix; // ������� ��������� ����� (������������ ������)
	XMFLOAT4X4 gViewLightMatrixAbs; // ������� ��������� ����� (����������)
	XMFLOAT4X4 gProjLightMatrix; // ������� �������� �����
};
struct CbufOnce { // ������ ���������

	XMFLOAT4 shadowMapDimentons; // ����������� ����� �����
};

// ��������� ������
struct VertPos3 { // ��� �������� ��� ��������

	XMFLOAT3 position;
};
struct VertPos3Tex2 { // ��� �������� � ���������

	XMFLOAT3 poisition;
	XMFLOAT2 texture;
};
struct VertPos3Nor3 { // ��� ������� ��� ��������

	XMFLOAT3 position;
	XMFLOAT3 normal;
};
struct VertPos3Nor3Tex2 { // ��� ������� � ���������

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
};
struct VertTerrain { // ��� ����������

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent; // ��� normal mapping
	XMFLOAT2 texture1;
	XMFLOAT2 texture2;
	XMFLOAT2 maskTex; // �������� �����
};
struct VertPos3Nor2Tex2W1Bone2 { // ��� ������������ �������

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	float weight;
	UINT bones[2];
};
struct VertInstance { // ��� ������� ������� � ���������

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	XMFLOAT3 instancePos;
};
struct VertMap { // ��� �����

	XMFLOAT3 position;
	XMFLOAT3 color;
};

// ��������� �������� �����
struct MouseChange {

	short wheelShift; // �������� ������
	NumXY<int> cursorShift; // �������� �������
	bool downButtons[2]; // ������ ���� ������
	bool upButtons[2]; // ������ ������
	bool holdButtons[2]; // ������ ������
	NumXY<int> cursorPosition; // ��������� �������
	MouseChange() {
		Clear();
	}
	bool Clear(){
		ZeroMemory(&wheelShift, sizeof(short));
		cursorShift.x = 0; cursorShift.y = 0;
		ZeroMemory(downButtons, sizeof(downButtons));
		ZeroMemory(upButtons, sizeof(upButtons));
		return true;
	}

};

// ��������� ������ � �����������
struct KeyboardChange {

	bool keys[4]; // ������� ���������
	bool keysShort[8]; // ������� �������
	KeyboardChange() {
		ZeroMemory(this, sizeof(KeyboardChange));
	}
	bool Clear() {
		ZeroMemory(keysShort, sizeof(keysShort));
		return true;
	}

};

// �������� ����������� ������ ( !!! �� ���������� � ��������� ��������� ������� ������� !!!)
enum CELL_ITEM_L1 { CI_BRUSH, CI_FERN, CI_MUSHROOM, CI_FIR, CI_BOLDFIR, CI_TREE, CI_BOULDER, CI_REED, CI_SUNFLOWER, CI_TUSSOCK, CI_DANDELION, CI_WHEAT, CI_CAMOMILE, CI_STUMP, CI_NONE }; // ��� ������� (������) �������
enum CELL_ITEM_L2 { CI_TT1, CI_TT2, CI_TT3 }; // ��� ������� ����� (������)

// ���� �������� ��� ������� (������) �������
#define PICKABLE_FROM_L1 CI_BRUSH
#define PICKABLE_TO_L1 CI_MUSHROOM
#define CHOPABLE_FROM_L1 CI_FIR
#define CHOPABLE_TO_L1 CI_TREE

// ���� �������� ��� ������� ����� (������)
#define PICKABLE_FROM_L2 -1
#define PICKABLE_TO_L2 -1
#define CHOPABLE_FROM_L2 -1
#define CHOPABLE_TO_L2 -1

// ���� ������
enum BIOMS { B_SWAMP = 0, B_FOREST = 1, B_MEADOW = 2, B_DREAMFOREST = 3, B_TESTED = 4 };

// ���������� ������
struct CellFiller {

	// ������������
	CellFiller() : item(CI_NONE), blocked(0), biomType(B_TESTED) {};
	CellFiller(CELL_ITEM item, unsigned char blocked) : item(item), blocked(blocked), biomType(B_TESTED) {};

	// ������ ������
	CELL_ITEM item; // �������
	unsigned char blocked; // ������������
	int additional; // ������ �����
	NumXY<float> itemCenter; // ����� �������� ��� �������� (������������� �� ����)
	BIOMS biomType; // ��� ����������

};

// ��������� ������
struct ButtonsDesc {
	bool caption;
	bool enable;
};

// �������� ����������� ���������
enum INVENTORY_ITEM { II_NOTHING = 0, II_STICK = 1, II_FERN = 3, II_MUSHROOM = 4, II_ROPE = 5, II_MEAT = 6, II_FANG = 7, II_ROCK = 8, II_LOG = 9,

	// ������ � ������
	II_CLUB = 128, II_AXE = 129, II_SWORD = 130, II_PICK = 131 // !!! ������� ������ ��������������� MIN_TOOL !!! � ������� ������ ��������������� ������� !!!

};
#define II_MINTOOL II_CLUB // ����������� ���� ��� ������ �����


// ��������� ����������� ���������
struct InventoryContent {

	// ������������
	InventoryContent(INVENTORY_ITEM invItem, UINT amount) : invItem(invItem), amount(amount) {};
	InventoryContent() {};

	// ����������
	INVENTORY_ITEM invItem; // ������� ���������
	UINT amount; // ���������� ���������

};

// ����� ������ (������ ��������� � ������� ����������� ��������)
enum PERS_CLIPS{ PC_STAND = 0, PC_WALK = 1, PC_DIE = 2, PC_STRIKE = 3 };

// ��������� ����� (������, ����, �������, ������ ������, ���������� ��� �����)
enum PERS_STATE { PS_STAND = 0, PS_WALK = 1, PS_DIE = 2, PS_CORPSE = 3, PS_BURY = 4, PS_STRIKE = 5};

// ����� ����� (������ ��������� � ������� �������)
enum HERO_CLIPS { HC_BATSTAND = 0, HC_BATATTACK0 = 1, HC_BATATTACK1 = 2, HC_BATATTACK2 = 3, HC_BATDIE = 4, HC_BATRUN = 5, HC_STAND = 6, HC_ATTACK0 = 7, HC_ATTACK1 = 8, HC_ATTACK2 = 9, HC_DIE = 10, HC_RUN = 11, HC_PICK = 12};

// ��������� �����
enum HERO_STATE { HS_STAND, HS_WALK, HS_PICK, HS_ATTACK, HS_DEAD };

// ���� ������������ ��������
enum SPRITE_ALIGN {SA_TOPLEFT, SA_BOTTOMLEFT, SA_TOPRIGHT, SA_BOTTOMRIGHT, SA_CENTER};

// ������� ����������
enum KEYBOARD_BUTTONS { KB_W, KB_S, KB_A, KB_D };
enum KEYBOARD_BUTSHORT { KS_TAB, KS_SPACE, KS_ENTER, KS_ESCAPE, KS_CTRL, KS_F1, KS_F2, KS_F3 };

// ��������� ����
enum GAME_STATE { GS_INGAME_MENU, GS_GAME, GS_MAIN_MENU };

// ���� �������
enum FONT_TYPE { FT_SLAVIC, FT_CONSOLE };

// �������� �������� �������
enum SHADER_REGISTORS { SR_ANIMMODEL = 0, SR_CAMERA = 1, SR_MODEL = 2, SR_LIGHTVECTOR = 3, SR_FONT = 4, SR_FRAME = 5, SR_LIGHT = 6, SR_ONCE = 7};

// ������� ����������� �� ����������
enum WIELD_EVENTS { E_PICKAXE, E_CHOP};

#endif // STRUCTURES_H
