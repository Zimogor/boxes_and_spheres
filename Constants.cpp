#include "Constants.h"

// ����������
bool Const::winMaximized = true; // ���� �� ���� �����
UINT Const::animationSpeed = 24; // �������� �������� (������ � �������)
float Const::clipBlendSpeed = 3.0f; // �������� �������� ��������
bool Const::writeToDebugFile = false; // ������ � ���� �������
bool Const::breakDT = true; // ��������� ������������� ��������
UINT Const::sleepTime = 0; // ������������� �������� ������������������
bool Const::showConsole = true; // ���������� �������
#if defined (DEBUG) || defined (_DEBUG)
bool Const::loadConstants = true; // ��������� ��������� �� �����
#else 
bool Const::loadConstants = true; // ��������� ��������� �� �����
#endif

// �����
bool Const::detalization = true; // ������� �����������
bool Const::backCulling = true; // ����� backCulling
bool Const::wireFrame = false; // ����� ������ �����
bool Const::alpha = true; // �����-�����
UINT Const::vSync = 0; // ������������ �������������
bool Const::multisampling = true; // �������������������
bool Const::anisotrophicFilter = false; // ������������ ����������
bool Const::showBoundingBox = false; // ���������� boundingbox

// ����������
int Const::regionCellSize = 4; // ������ ������� ���������� (� ���������)
float Const::updateDistance = 1.0f; // ��������� �� ������� ����������� ���� ��������
UINT Const::regionVertexAmount = Const::regionCellSize * Const::regionCellSize * 4; // ���������� ������ � �������
UINT Const::regionIndexAmount = Const::regionCellSize * Const::regionCellSize * 6; // ���������� �������� � �������
float Const::regionHalfCellSize = Const::regionCellSize * 0.5f; // �������� ������� ������� � �������
int Const::regionFieldSize = UINT(Const::showRadius * 2 / Const::regionCellSize + 1); // ������ ���� ��������
UINT Const::tilesInSquadForest = 2; // ���������� ������ � ����� �������� ����
UINT Const::tilesInSquadMeadow = 2; // ���������� ������ � ����� �������� ����
UINT Const::tilesInSquadSwamp = 2; // ���������� ������ � ����� �������� ������
float Const::widthScatter = 0.1f; // ������� �� ������
float Const::heightScatter = 0.25f; // ������� �� ������
float Const::positionScatter = 0.3f; // ������� �� �����������
bool Const::showTiles = false; // ���������� ������
wchar_t* Const::testFileName = NULL; // ��� ������������ �������
int Const::location = 0; // ����� ��������� �������
UINT Const::patchFieldSize = 128; // ������ ���� ������ ??? ��� �����������
int Const::patchCellSize = 256; // ������ ������� � ������� ??? ��� ������������ ??? ���� 256

// ���� � ����
bool Const::shadowsOn = true; // �������� ����
NumXY<UINT> Const::shadowMapSize = NumXY<UINT>(1800, 1600); // ���������� ����� �����
myXMFLOAT3 Const::lightPosition = spawnPoint + myXMFLOAT3(6.0f, 3.0f, 12.0f); // ���������� ��������� �����
XMFLOAT4 Const::viewRect = { -5.f, 5.0f, 5.0f, -5.0f }; // ������ �������� �����
NumXY<float> Const::nearFarLightZ(10.0f, 20.0f); // ��������� ����� (x - near, y - far)

// ������
bool Const::releaseCamera = true; // ������ ��� ������ � �������������
float Const::scrollSpeed = 0.0006f; // �������� ����������
float Const::showRadius = 23.0f; // ������ ����������� ��������
float Const::maxRadius = Const::releaseCamera ? 11.0f : 100.0f; // ������������ ���������
float Const::minRadius = Const::releaseCamera ? 4.0f : 0.3f; // ����������� �����������
NumXY<float> Const::pitchLimit = NumXY<float>( -1.0f, 1.0f ); // ����������� �������

// �����
myXMFLOAT3 Const::spawnPoint = myXMFLOAT3(5.0f, 0.0f, 20.0f); // ����� ���������� �����
float Const::heroSpeed = 5.0f; // �������� �����
bool Const::fullInventory = false; // ������ ���������
float Const::distanceToPick = 0.6f; // ���������� ���������� � ����������

// ������� ��������
float Const::mainMenuBackgroundDepth = 15.0f; // ������� ���� ��������� ����
float Const::mainMenuButtonsDepth = 9.0f; // ������� ������ ��������� ����
float Const::mainMenuButtonsFontDepth = 0.3f; // ������� ������ ������ ��������� ����
float Const::ingameMenuBackgroundDepth = 10.0f; // ������� ���� �������� ����
float Const::ingameMenuButtonsDepth = 9.0f; // ������� ������ �������� ����
float Const::ingameMenuIconsDepth = 0.45f; // ������� ������ �������� ����
float Const::ingameMenuIconsFontDepth = 0.4f; // ������� ������ ������ �������� ����
float Const::ingameMenuIconsUpDepth = 0.1f; // ����������� ������� ��������� ������
float Const::consoleDepth = 0.2f; // ������� �������
float Const::helpFontDepth = 1.0f; // ������� ���������������� ������
float Const::mapDepth = 9.0f; // ������� �����

// ---------------------------------�������----------------------------------

// ����� � �����������
std::ifstream Const::config; // ��������������� ����

// ��������� ��������� �� �����
bool Const::LoadFromFile() {

	// �������� �����
	config.open("resources//config", std::ios_base::binary);
	if (!config.is_open()) {
		MyMessage(L"constant file doesn't exist");
		return false;
	}

	// ��������� ��������� �����
	config.read((char*)&alpha, 1); // �����-�����
	config.read((char*)&backCulling, 1); // backCulling
	config.read((char*)&wireFrame, 1); // ����� ����������� �����
	config.read((char*)&detalization, 1); // �����������
	config.read((char*)&vSync, 4); // ������������ �������������
	config.read((char*)&multisampling, 1); // �������������������
	config.read((char*)&anisotrophicFilter, 1); // ������������ ����������
	config.read((char*)&showBoundingBox, 1); // ���������� bounding box

	// ��������� ��������� ����������
	config.read((char*)&winMaximized, 1); // �� ���� �����
	config.read((char*)&animationSpeed, 4); // �������� ��������
	config.read((char*)&writeToDebugFile, 1); // ������ � ���� �������
	config.read((char*)&breakDT, 1); // ������� �������� ��� ��������
	config.read((char*)&sleepTime, 4); // ������������ �������
	config.read((char*)&showConsole, 1); // ���������� �������
	config.read((char*)&clipBlendSpeed, 4); // �������� �������� ��������

	// ��������� ��������� ����������
	config.read((char*)&regionCellSize, 4); // ������ ������� � �������
	config.read((char*)&updateDistance, 4); // ���������� ���������� ��������
	config.read((char*)&tilesInSquadForest, 4); // ���������� ������ �� �������� ����
	config.read((char*)&tilesInSquadMeadow, 4); // ���������� ������ �� �������� ������
	config.read((char*)&tilesInSquadSwamp, 4); // ���������� ������ �� �������� ������
	config.read((char*)&widthScatter, 4); // ������� �������� �� ������
	config.read((char*)&heightScatter, 4); // ������� �������� �� ������
	config.read((char*)&positionScatter, 4); // ������� �������� �� �����������
	config.read((char*)&showTiles, 1); // ���������� ������	
	// ����� ������������ �������
	bool testObject; // ����� ������������
	int wordLength; // ���������� ��������� � ��������
	config.read((char*)&testObject, 1);
	config.read((char*)&wordLength, 4);
	testFileName = new wchar_t[wordLength + 1];
	config.read((char*)testFileName, wordLength * sizeof(wchar_t) + sizeof(wchar_t));
	if (!testObject) ReleaseNULLS(testFileName); // ������������ �� �����
	config.read((char*)&location, 4); // ����� ��������� �������

	// ��������� ��������� ���������
	regionVertexAmount = regionCellSize * regionCellSize * 4; // ���������� ������ � �������
	regionIndexAmount = regionCellSize * regionCellSize * 6; // ���������� �������� � �������
	regionHalfCellSize = regionCellSize * 0.5f; // �������� ������� ������� � �������
	regionFieldSize = UINT(showRadius * 2 / regionCellSize + 1); // ������ ���� ��������

	// ��������� ��������� ��������� � �����
	float halfShadowBoxSide;
	config.read((char*)&shadowsOn, 1);
	config.read((char*)&shadowMapSize.x, 4);
	config.read((char*)&shadowMapSize.y, 4);
	config.read((char*)&lightPosition.x, 4);
	config.read((char*)&lightPosition.y, 4);
	config.read((char*)&lightPosition.z, 4);
	config.read((char*)&halfShadowBoxSide, 4);
	config.read((char*)&nearFarLightZ.x, 4);
	config.read((char*)&nearFarLightZ.y, 4);
	// ��������� ��������� ���������
	viewRect.y = viewRect.z = -halfShadowBoxSide;
	viewRect.w = viewRect.x = halfShadowBoxSide;

	// ��������� ��������� ��� ������
	float maxRelaseRadius, maxFreeRadius, minReleaseRadius, minFreeRadius;
	float maxPitch, minPitch;
	config.read((char*)&releaseCamera, 1); // ������������� �������� ������
	config.read((char*)&scrollSpeed, 4); // �������� ����������
	config.read((char*)&showRadius, 4); // ������ ����������� ��������
	config.read((char*)&maxRelaseRadius, 4); // ������������ ������ � �������� ������
	config.read((char*)&minReleaseRadius, 4); // ����������� ������ � �������� ������
	config.read((char*)&maxFreeRadius, 4); // ������������ ������ � ��������� ������
	config.read((char*)&minFreeRadius, 4); // ����������� ������ � �������� ������
	config.read((char*)&maxPitch, 4); // ������������ ���� �������
	config.read((char*)&minPitch, 4); // ����������� ���� �������
	// ��������� ��������� ���������
	maxRadius = releaseCamera ? maxRelaseRadius : maxFreeRadius; // ������������ ���������
	minRadius = releaseCamera ? minReleaseRadius : minFreeRadius; // ����������� �����������
	pitchLimit = NumXY<float>(minPitch, maxPitch); // ����������� �������
	scrollSpeed /= 1000.0f; // �������� ����������

	// ��������� ��������� �����
	float heroPosX, heroPosY;
	config.read((char*)&heroPosX, 4); // X-���������� �����
	config.read((char*)&heroPosY, 4); // Y-���������� �����
	config.read((char*)&heroSpeed, 4); // �������� ����������� �����
	config.read((char*)&fullInventory, 1); // ������ ���������
	config.read((char*)&distanceToPick, 4); // ��������� ���������� � ����������
	// ��������� ��������� ���������
	spawnPoint = myXMFLOAT3(heroPosX, 0.0f, heroPosY); // ����� ���������� �����

	// ��������� ������� ��������
	config.read((char*)&mainMenuBackgroundDepth, 4); // ������� ����
	config.read((char*)&mainMenuButtonsDepth, 4); // ������ �������� ����
	config.read((char*)&mainMenuButtonsFontDepth, 4); // ����� ������ �������� ����
	config.read((char*)&ingameMenuBackgroundDepth, 4); // ������������� ����
	config.read((char*)&ingameMenuButtonsDepth, 4); // ������ �������������� ����
	config.read((char*)&ingameMenuIconsDepth, 4); // ������ �������������� ����
	config.read((char*)&ingameMenuIconsFontDepth, 4); // ����� ������ �������������� ����
	config.read((char*)&ingameMenuIconsUpDepth, 4); // ����������� ������� ���������� ������
	config.read((char*)&consoleDepth, 4); // �������
	config.read((char*)&helpFontDepth, 4); // ��������������� �����
	config.read((char*)&mapDepth, 4); // �����

	// ������� �����
	config.close();

	maxRadius = releaseCamera ? 11.0f : 100.0f; // ������������ ���������

	return true;

}

// ��������� ���������
void Const::UnloadConstants() {

	ReleaseNULLS(testFileName);

}