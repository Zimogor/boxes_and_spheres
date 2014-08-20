#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"
#include "Font.h"

// ������������ ������ ��������� ���� (������ ��������� � ������������������ �������� ������ ����� � ��������� ������)
enum MAIN_MENU_STATE { MMS_MAIN = 0 , MMS_LOADSCREEN = 1};

// ������������ ���������� ������� ������ (������ ��������� � ������������������ �������� ������)
enum MAIN_MENU_RESULT { MMR_START = 0, MMR_SAVE = 1, MMR_LOAD = 2, MMR_QUIT = 3, MMR_NOTHING = 255};

// ����� ��������� ����
class MainMenu : public Menu {

public:

	// �������� ����
	MainMenu(bool* result); // �����������
	~MainMenu(); // ����������

	// ������ �������
	bool Update(MAIN_MENU_RESULT* menuResult); // ����������
	bool Draw(); // ���������
	bool DrawLoadScreen(); // ���������� ����� ��������
	bool Reset(); // �����

	// �������
	bool ChangeButtonCaption(wchar_t* newCaption, UINT buttonNumber); // ������� ��������� ������

private:

	// ����������
	Font* font = NULL; // �����
	ButtonsDesc* buttonsDesc = NULL; // ��������� ������

};

#endif // MAINMENU_H