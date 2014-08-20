#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "Utilities.h"
#include "Structures.h"
#include "Errors.h"
#include "Camera.h"
#include "ArtInt.h"
#include "Helper.h"
#include "Shader.h"
#include "Hero.h"
#include "HUD.h"
#include "Map.h"

class Console;
class Terrain;
class Hero;
class Errors;
class SuperFunct;
class Timer;
class DynamicReg;
class HUD;
class Map;

// ����� ����������
class Mediator {

public:

	// ������� �����������
	static bool SendHeroEventDamage(myXMFLOAT3 position, float damageAmount); // ���� ����� � �����
	static bool SendMobEventDamage(myXMFLOAT3 position, float damageAmount); // ���� ���� � �����
	static bool SendMobEventDamage(float damageAmount); // ����������� �����
	static bool SendEventWorking(WIELD_EVENTS events, myXMFLOAT3 position); // ������ � �����
	static bool SendPickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult); // ���������� (true - ���� ��������)
	static bool SendLifeChange(float amount); // ����� ���������� ������ � ���������
	static bool SendEventMenuClose(); // �������� �������������� ����
	static bool SendEventNewRegion(DynamicReg* dynamicReg); // �������� ������ �������

	// �������
	static XMFLOAT3 GetTerrainUnderCursor(); // ������� ���������� �� ����������� ����� ��� ��������

	// ��������� �� �������� ����
	static Console* console; // �������
	static Terrain* terrain; // ����������
	static Hero* hero; // �����
	static Errors* errors; // ���������� �������
	static SuperFunct* superFunct; // ������������
	static Timer* timer; // ������
	static ArtInt* artInt; // ��
	static Helper* helper; // ��������
	static Shader* shader; // ������
	static Camera* camera; // ������
	static HUD* hud; // ���������
	static Map* map; // �����

	// �������������� ��������� � ����������
	static DynamicReg*** regionField; // ���� ���������� ������������ ��������
	static InventoryContent* inventory; // ��������� �����
	static MouseChange* mouseChange; // ������ ����
	static KeyboardChange* keyboardChange; // ������ ����������
	static BoundingBox* heroBoundBox; // boundbox �����
	static NumXY<int>* cornerPoint; // ������������� ���� ���� � ����������� �������

	// ��������� �� �������� ����������
	static D3D_FEATURE_LEVEL featureLevel; // ������� DirectX
	static ID3D11Device* pDev; // ��������� �� pDev
	static ID3D11DeviceContext* pDevContext; // ��������� �� pDevContext
	static NumXY<UINT>* winDimentions; // ����������� ����

	// ��������� ������
	static ID3D11InputLayout* pInputLayoutPos3Tex2; // ��� �������� � ���������
	static ID3D11InputLayout* pInputLayoutSpriteInstance; // ��� ������ ����
	static ID3D11InputLayout* pInputLayoutPos3Nor3; // ��� ������� ��� ��������
	static ID3D11InputLayout* pInputLayoutPos3Nor3Tex2; // ��� ������� c ���������
	static ID3D11InputLayout* pInputLayoutTerrain; // ��� ���������
	static ID3D11InputLayout* pInputLayoutPos3Nor3Tex2W1Bone2; // ��� ������������ �������
	static ID3D11InputLayout* pInputLayoutInstance; // ��� ������� ������� � ���������
	static ID3D11InputLayout* pInputLayoutPosCol; // ��� �����

	// ��������� �������
	static ID3D11VertexShader* pVSPos; // ��� �������� ��� ��������
	static ID3D11VertexShader* pVSPosTex; // ��� �������� � ���������
	static ID3D11VertexShader* pVSSpriteInstance; // ��� ������ ����
	static ID3D11VertexShader* pVSPosNor; // ��� ������� ��� ��������
	static ID3D11VertexShader* pVSPosNorTex; // ��� ������� � ����������
	static ID3D11VertexShader* pVSTerrain; // ��� �������� ����������
	static ID3D11VertexShader* pVSPosNorTexWBone; // ��� ������������ ������� � �������
	static ID3D11VertexShader* pVSShadow; // ���� ������������ ������� � �������
	static ID3D11VertexShader* pVSInstance; // ��� �������� �������
	static ID3D11VertexShader* pVSShadowInstance; // ��� ����� �������� �������
	static ID3D11VertexShader* pVSPosCol; // ��� �����

	// ���������� �������
	static ID3D11PixelShader* pPSPos; // ��� �������� ��� ��������
	static ID3D11PixelShader* pPSPosTex; // ��� �������� � ���������
	static ID3D11PixelShader* pPSFont; // ��� �������
	static ID3D11PixelShader* pPSSolid; // ��� �������� �������
	static ID3D11PixelShader* pPSPosNor; // ��� ������� ��� ��������
	static ID3D11PixelShader* pPSPosNorTex; // ��� ������� � ���������
	static ID3D11PixelShader* pPSClipping; // ��� ������� � ��������� � ����������
	static ID3D11PixelShader* pPSTerrain; // ��� �������� ����������
	static ID3D11PixelShader* pPSTerrainNoShadow; // ��� �������� ���������� (��� ����)
	static ID3D11PixelShader* pPSEmpty; // ������ ������� ������
	static ID3D11PixelShader* pPSPosCol; // lkz rfhns

	// ��������� ������
	static ID3D11Buffer* cbufAnimModel; // ��� ������������ �������
	static ID3D11Buffer* cbufReset; // ��� ������
	static ID3D11Buffer* cbufObject; // ��� ��������
	static ID3D11Buffer* cbufFont; // ��� �������
	static ID3D11Buffer* cbufFrame; // ��� ������� �����

};

#endif // MEIDATOR_H