#ifndef MAIN_H
#define MAIN_H

#include "Mediator.h"
#include "Application.h"
#include "Timer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Sprite.h"
#include "Console.h"
#include "Terrain.h"
#include "Hero.h"
#include "ArtInt.h"
#include "MainMenu.h"
#include "IngameMenu.h"
#include "Helper.h"
#include "HUD.h"

class ZimApplication : public Application {

public:

	// ����������
	ZimApplication(HINSTANCE hInstance, bool* result) : Application(hInstance, result) {}; // �����������
	~ZimApplication(); // ����������

	// ����������� �������
	virtual bool Draw(); // ��������� ����������
	virtual bool ImDrawContent(); // ���������� ���������
	virtual bool Update(float dT); // ���������� ����������
	virtual bool Reset(); // ����� ����������
	virtual bool DrawShadow(); // ��������� � ����� �����

	// ������� ����������
	SuperFunct* superFunct = NULL; // ����� ������������
	Shader* shader = NULL; // ������
	Console* console = NULL; // �������
	Camera* camera = NULL; // ������
	MainMenu* mainMenu = NULL; // �������� ����
	IngameMenu* ingameMenu = NULL; // ������������� ����
	Terrain* terrain = NULL; // ����������
	Hero* hero = NULL; // �����
	ArtInt* artInt = NULL; // ������������� ���������
	Helper* helper = NULL; // ��������
	HUD* hud = NULL; // ���������

	// �������
	bool LoadResources(int location); // ��������� ������� ����

	// ����������
	GAME_STATE gameState = GS_MAIN_MENU; // ��������� ����
	bool resLoaded = false; // ������� ������� ���������

};


#endif // MAIN_H