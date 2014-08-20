#include "MainMenu.h"
#include "Mediator.h"

// ����������� ��������� ����
MainMenu::MainMenu(bool* result) : Menu(console, result) {

	BRR(*result);
	menuState = MMS_MAIN;

	// �������� ������
	BRRC2(font = new Font(XMFLOAT3(1.0f, 1.0f, 0.0f), FT_SLAVIC, Const::mainMenuButtonsFontDepth, result));

	// �������� ������ (������ ��������� � enum MAIN_MENU_RESULT)
	BRRC2(Button* butStart = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, -64.0f - 128.f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f, 64.0f), L"����� ����", result));
	buttons.push_back(butStart);
	BRRC2(Button* butSave = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, -64.0f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f + 154.0f, 64.0f), L"��������� ����", result));
	buttons.push_back(butSave);
	BRRC2(Button* butLoad = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, 64.0f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f + 162.0f, 64.0f), L"��������� ����", result));
	buttons.push_back(butLoad);
	BRRC2(Button* butEnd = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, 64.0f + 128.f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 266.0f, 64.0f), L"�����", result));
	buttons.push_back(butEnd);
	butSave->enable = false;
	butLoad->enable = false;

	// �������� ������� ����
	BRRC2(Sprite* mainBackground = new Sprite(L"resources//menu//main_menu.png", XMFLOAT3(0.0f, 0.0f, Const::mainMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, result));
	backgrounds.push_back(mainBackground);

	// �������� ���� ��������
	BRRC2(Sprite* loadScreen = new Sprite(L"resources//menu//loadscreen.dds", XMFLOAT3(0.0f, 0.0f, Const::mainMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, result));
	backgrounds.push_back(loadScreen);

	// �������� ���������
	buttonsDesc = new ButtonsDesc[buttons.size()];

	// �������� ���������� ������ � ������� ����������
	for (UINT i(0); i < buttons.size(); i++) {
		if (buttons[i]->caption) {
			BRR(font->TextOutXY(buttons[i]->caption, XMFLOAT2(buttons[i]->GetSpriteRelPosition().x - (UINT)buttons[i]->GetDimentions().x / 2, buttons[i]->GetSpriteRelPosition().y - buttons[i]->GetDimentions().y), SA_CENTER, false));
		}
		buttonsDesc[i].caption = buttons[i]->caption ? true : false;
		buttonsDesc[i].enable = buttons[i]->enable;
	}

}

// ���������� ��������� ����
bool MainMenu::Update(MAIN_MENU_RESULT* menuResult) {

	for (UINT i(0); i < buttons.size(); i++) {
		if (buttons[i]->UnderCursor() && Mediator::mouseChange->downButtons[0]) {
			*menuResult = (MAIN_MENU_RESULT)i;
			break;
		}
	}

	return true;

}

// ������� ��������� ������
bool MainMenu::ChangeButtonCaption(wchar_t* newCaption, UINT buttonNumber) {

	// ��������� ������ ��������
	if (!buttons[buttonNumber]->caption || !newCaption) {
		Mediator::errors->Push(L"can't change null pointer");
		return false;
	}

	// ����� ������� �����������
	UINT sentenceNumber = 0;
	for (UINT i(0); i < buttonNumber; i++) {
		if (buttonsDesc[i].caption) sentenceNumber++;
	}

	// ������ �����������
	BR(buttons[buttonNumber]->ChangeCaption(newCaption));
	BR(font->ReplaceSentence(sentenceNumber, newCaption));
	
	return true;

}

// ���������� ����� ��������
bool MainMenu::DrawLoadScreen() {

	menuState = MMS_LOADSCREEN;	
	for (auto iter = buttons.begin(); iter != buttons.end(); iter++)
		(*iter)->visible = false;
	BR(Menu::Draw());
	menuState = MMS_MAIN;
	for (auto iter = buttons.begin(); iter != buttons.end(); iter++)
		(*iter)->visible = true;

	return true;

}

// ���������
bool MainMenu::Draw() {

	// ��������� ��������
	BR(Menu::Draw());

	// ��������� �������
	BR(font->Draw(buttonsDesc));

	return true;

}

// �����
bool MainMenu::Reset() {

	// ����� ��������
	Menu* parent = this;
	BR(parent->Reset());

	// ����� �������
	BR(font->Reset());

	return true;

}

// ����������
MainMenu::~MainMenu() {
	
	ReleaseNULL(font);
	ReleaseNULLS(buttonsDesc);

};