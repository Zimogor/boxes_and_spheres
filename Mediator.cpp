#include "Mediator.h"

#define DEPTH_PRECISION 0.1f

// ��������� �� �������� ����
Console* Mediator::console = NULL; // �������
Terrain* Mediator::terrain = NULL; // ����������
Hero* Mediator::hero = NULL; // �����
Errors* Mediator::errors = NULL; // ���������� ������
SuperFunct* Mediator::superFunct = NULL; // ������������
Timer* Mediator::timer = NULL; // ������
ArtInt* Mediator::artInt = NULL; // ��
Helper* Mediator::helper = NULL; // ��������
Shader* Mediator::shader = NULL; // ������
Camera* Mediator::camera = NULL; // ������
HUD* Mediator::hud = NULL; // ���������
Map* Mediator::map = NULL; // �����

// �������������� ��������� � ����������
DynamicReg*** Mediator::regionField = NULL; // ���� ���������� ������������ ��������
InventoryContent* Mediator::inventory = NULL; // ��������� �����
MouseChange* Mediator::mouseChange = NULL; // ������ ����
KeyboardChange* Mediator::keyboardChange = NULL; // ������ ����������
BoundingBox* Mediator::heroBoundBox = NULL; // boundingbox �����
NumXY<int>* Mediator::cornerPoint = NULL; // ������������� ���� ���� � ����������� �������

// ��������� �� �������� ����������
D3D_FEATURE_LEVEL Mediator::featureLevel = D3D_FEATURE_LEVEL_9_1; // ������� DirectX
ID3D11Device* Mediator::pDev = NULL; // ��������� �� pDev
ID3D11DeviceContext* Mediator::pDevContext = NULL; // ��������� �� pDevContext
NumXY<UINT>* Mediator::winDimentions = NULL; // ����������� ����

// ��������� ������
ID3D11InputLayout* Mediator::pInputLayoutPos3Tex2 = NULL; // ��� �������� � ���������
ID3D11InputLayout* Mediator::pInputLayoutSpriteInstance; // ��� ������ ����
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3 = NULL; // ��� ������� ��� ��������
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3Tex2 = NULL; // ��� ������� c ���������
ID3D11InputLayout* Mediator::pInputLayoutTerrain = NULL; // ��� ���������
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3Tex2W1Bone2 = NULL; // ��� ������������ �������
ID3D11InputLayout* Mediator::pInputLayoutInstance = NULL; // ��� ������� ������� � ���������
ID3D11InputLayout* Mediator::pInputLayoutPosCol = NULL; // ��� �����

// ��������� �������
ID3D11VertexShader* Mediator::pVSPos = NULL; // ������ ��� �������� ��� ��������
ID3D11VertexShader* Mediator::pVSPosTex = NULL; // ������ ��� �������� � ���������
ID3D11VertexShader* Mediator::pVSSpriteInstance = NULL; // ��� ������ ����
ID3D11VertexShader* Mediator::pVSPosNor = NULL; // ��� ������� ��� ��������
ID3D11VertexShader* Mediator::pVSPosNorTex = NULL; // ��� ������� � ����������
ID3D11VertexShader* Mediator::pVSTerrain = NULL; // ��� �������� ����������
ID3D11VertexShader* Mediator::pVSPosNorTexWBone = NULL; // ��� ������� � �������
ID3D11VertexShader* Mediator::pVSShadow = NULL; // ���� ��� ������� � �������
ID3D11VertexShader* Mediator::pVSInstance = NULL; // ��� �������� �������
ID3D11VertexShader* Mediator::pVSShadowInstance = NULL; // ��� ����� �������� �������
ID3D11VertexShader* Mediator::pVSPosCol = NULL; // ��� �����

// ���������� �������
ID3D11PixelShader* Mediator::pPSPos = NULL; // ��� �������� ��� ��������
ID3D11PixelShader* Mediator::pPSPosTex = NULL; // ��� �������� � ���������
ID3D11PixelShader* Mediator::pPSFont = NULL; // ��� �������
ID3D11PixelShader* Mediator::pPSSolid; // ��� �������� �������
ID3D11PixelShader* Mediator::pPSPosNor = NULL; // ��� ������� ��� ��������
ID3D11PixelShader* Mediator::pPSPosNorTex = NULL; // ��� ������� � ���������
ID3D11PixelShader* Mediator::pPSClipping = NULL; // ��� ������� � ��������� � ����������
ID3D11PixelShader* Mediator::pPSTerrain = NULL; // ��� �������� ����������
ID3D11PixelShader* Mediator::pPSTerrainNoShadow = NULL; // ��� �������� ���������� (��� ����)
ID3D11PixelShader* Mediator::pPSEmpty = NULL; // ������ �������
ID3D11PixelShader* Mediator::pPSPosCol = NULL; // ��� �����

// ��������� ����������� ������
ID3D11Buffer* Mediator::cbufAnimModel = NULL; // ��� ������������ �������
ID3D11Buffer* Mediator::cbufReset = NULL; // ��� ������
ID3D11Buffer* Mediator::cbufObject = NULL; // ��� ��������
ID3D11Buffer* Mediator::cbufFont = NULL; // ��� �������
ID3D11Buffer* Mediator::cbufFrame = NULL; // ��� ������� �����

// ������� ���������� �� ���������� ����� ��� ��������
XMFLOAT3 Mediator::GetTerrainUnderCursor() {

	// �������� ������
	XMMATRIX P = camera->GetProjMatrix();
	XMMATRIX V = camera->GetViewMatrix();

	// ������� ��� � ������������ ������
	float vx = (2.0f * mouseChange->cursorPosition.x / winDimentions->x - 1.0f) / P(0, 0);
	float vy = (-2.0f * mouseChange->cursorPosition.y / winDimentions->y + 1.0f) / P(1, 1);
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // ������ ����
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f); // ����������� ����

	// ������������� ��� � ������� ������������
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
	rayOrigin = XMVector3TransformCoord(rayOrigin, invView);
	rayDir = XMVector3Normalize(XMVector3TransformNormal(rayDir, invView));

	// �������� � ����� ������������ ����
	XMVECTOR rayPoint = rayOrigin + rayDir * camera->GetRadius();
	XMFLOAT3 rayPointF;
	XMStoreFloat3(&rayPointF, rayPoint);
	float depth = superFunct->HeightFunction(rayPointF.x, rayPointF.z) - rayPointF.y;
	while (abs(depth) >= DEPTH_PRECISION) {
		rayPoint -= rayDir * depth;
		XMStoreFloat3(&rayPointF, rayPoint);
		depth = superFunct->HeightFunction(rayPointF.x, rayPointF.z) - rayPointF.y;
	}

	return rayPointF;

}

// ----------------------------------
// ������� �����������
// ----------------------------------

// ����� ���������� ������ � ���������
bool Mediator::SendLifeChange(float amount) {
	return hud->SetRedLevel(amount);
}

// ���� ����� � �����
bool Mediator::SendHeroEventDamage(myXMFLOAT3 position, float damageAmount) {
	return artInt->TakeDamage(position, damageAmount);
}

// ���� ���� � �����
bool Mediator::SendMobEventDamage(myXMFLOAT3 position, float damageAmount) {
	return hero->TakeDamage(position, damageAmount); // ��������� ���� �����
}
bool Mediator::SendMobEventDamage(float damageAmount) {
	return hero->TakeDamage(damageAmount); // ���� �����
}

// ������ � �����
bool Mediator::SendEventWorking(WIELD_EVENTS events, myXMFLOAT3 position) {
	return terrain->TakePick(events, position);
}

// ���������� (true - ���� ��������)
bool Mediator::SendPickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult) {
	return terrain->TakePickup(position, cellItemResult);
}

// �������� �������������� ����
bool Mediator::SendEventMenuClose() {
	hero->ChangeTool();
	return true;
}

// �������� ������ �������
bool Mediator::SendEventNewRegion(DynamicReg* dynamicReg) {
	return map->NewRegion(dynamicReg);
}