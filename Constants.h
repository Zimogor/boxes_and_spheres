#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Utilities.h"
#include "Nums.h"
#include "Structures.h"

const XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // ������ �����

#define MAX_ITEM_INV_AMOUNT 44 // ���������� ������ ��� ���������
#define MAX_ITEM_CRAFT_AMOUNT 4 // ���������� ������ ��� ������ (������� �������� ������)
#define ITEM_CRAFT_SLOT 47 // ���� ��� ����������� ������
#define ITEM_WEAPON_SLOT 48 // ���� ��� ������
#define MAX_ITEM_AMOUNT 49 // ������ ���������� ���� ������
#define TEXTURE_BUFFER 0 // ��������� ��� �������� �������
#define SHADOW_BUFFER 1 // ��������� ��� ����� �����
#define NORMALMAP_BUFFER 2 // �������� ��� normalmapping
#define SAMPLER_BUFFER 0 // ��������� ��� �������� �������
#define SAMPLER_SHADOW 1 // ��������� ��� �������� �������

class Const {

public:

	// ����������
	static bool winMaximized; // ���� �� ���� �����
	static UINT animationSpeed; // �������� �������� (������ � �������)
	static float clipBlendSpeed; // �������� �������� ��������
	static bool writeToDebugFile; // ������ � ���� �������
	static bool loadConstants; // ��������� ��������� �� �����
	static bool breakDT; // ��������� ������������� ��������
	static UINT sleepTime; // ������������� �������� ������������������
	static bool showConsole; // ���������� �������

	// �����
	static bool backCulling; // ����� backCulling
	static bool detalization; // ������� �����������
	static bool wireFrame; // ����� ������ �����
	static bool alpha; // �����-�����
	static UINT vSync; // ������������ �������������
	static bool multisampling; // �������������������
	static bool anisotrophicFilter; // ������������ ����������
	static bool showBoundingBox; // ���������� boundingbox

	// ����������
	static int regionCellSize; // ������ ������� ����������
	static UINT regionVertexAmount; // ���������� ������ � �������
	static UINT regionIndexAmount; // ���������� �������� � �������
	static float updateDistance; // ��������� �� ������� ����������� ���� ��������
	static float regionHalfCellSize; // �������� ������� ������� � �������
	static int regionFieldSize; // ������ ���� ��������
	static UINT tilesInSquadForest; // ���������� ������ � ����� �������� ����
	static UINT tilesInSquadMeadow; // ���������� ������ � ����� �������� ����
	static UINT tilesInSquadSwamp; // ���������� ������ � ����� �������� ������
	static float widthScatter; // ������� �� ������
	static float heightScatter; // ������� �� ������
	static float positionScatter; // ������� �� �����������
	static bool showTiles; // ���������� ������
	static wchar_t* testFileName; // ��� ������������ �������
	static int location; // ����� ��������� �������
	static UINT patchFieldSize; // ������ ���� ������
	static int patchCellSize; // ������ ������� � �������

	// ���� � ����
	static bool shadowsOn; // �������� ����
	static NumXY<UINT> shadowMapSize; // ���������� ����� �����
	static myXMFLOAT3 lightPosition; // ���������� ��������� �����
	static XMFLOAT4 viewRect; // ������ �������� �����
	static NumXY<float> nearFarLightZ; // ��������� ����� (x - near, y - far)

	// ������
	static float scrollSpeed; // �������� ����������
	static float showRadius; // ������ ����������� ��������
	static float maxRadius; // ������������ ���������
	static float minRadius; // ����������� �����������
	static NumXY<float> pitchLimit; // ����������� �������
	static bool releaseCamera; // ������ ��� ������ � �������������

	// �����
	static myXMFLOAT3 spawnPoint; // ����� ���������� �����
	static float heroSpeed; // �������� �����
	static bool fullInventory; // ������ ���������
	static float distanceToPick; // ���������� ���������� � ����������

	// ������� ��������
	static float mainMenuBackgroundDepth; // ������� ���� ��������� ����
	static float mainMenuButtonsDepth; // ������� ������ ��������� ����
	static float mainMenuButtonsFontDepth; // ������� ������ ������ ��������� ����
	static float ingameMenuBackgroundDepth; // ������� ���� �������� ����
	static float ingameMenuButtonsDepth; // ������� ������ �������� ����
	static float ingameMenuIconsDepth; // ������� ������ �������� ����
	static float ingameMenuIconsFontDepth; // ������� ������ ������ �������� ����
	static float ingameMenuIconsUpDepth; // ����������� ������� ��������� ������
	static float consoleDepth; // ������� �������
	static float helpFontDepth; // ������� ���������������� ������
	static float mapDepth; // ������� �����

	// �������
	static bool LoadFromFile(); // ��������� ��������� �� �����
	static void UnloadConstants(); // ��������� ���������

private:

	// ����������
	static std::ifstream config; // ���������������� ����

};

#endif // CONSTANTS_H