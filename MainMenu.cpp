#include "MainMenu.h"
#include "Mediator.h"

// конструктор основного меню
MainMenu::MainMenu(bool* result) : Menu(console, result) {

	BRR(*result);
	menuState = MMS_MAIN;

	// создание шрифта
	BRRC2(font = new Font(XMFLOAT3(1.0f, 1.0f, 0.0f), FT_SLAVIC, Const::mainMenuButtonsFontDepth, result));

	// создание кнопок (должно совпадать с enum MAIN_MENU_RESULT)
	BRRC2(Button* butStart = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, -64.0f - 128.f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f, 64.0f), L"НОВАЯ ИГРА", result));
	buttons.push_back(butStart);
	BRRC2(Button* butSave = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, -64.0f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f + 154.0f, 64.0f), L"СОХРАНИТЬ ИГРУ", result));
	buttons.push_back(butSave);
	BRRC2(Button* butLoad = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, 64.0f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 87.0f + 162.0f, 64.0f), L"ЗАГРУЗИТЬ ИГРУ", result));
	buttons.push_back(butLoad);
	BRRC2(Button* butEnd = new Button(L"resources//menu//b_background.png", XMFLOAT3(0.0f, 64.0f + 128.f, Const::mainMenuButtonsDepth), NumXY<float>(512.0f - 266.0f, 64.0f), L"ВЫХОД", result));
	buttons.push_back(butEnd);
	butSave->enable = false;
	butLoad->enable = false;

	// создание заднего фона
	BRRC2(Sprite* mainBackground = new Sprite(L"resources//menu//main_menu.png", XMFLOAT3(0.0f, 0.0f, Const::mainMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, result));
	backgrounds.push_back(mainBackground);

	// создание фона загрузки
	BRRC2(Sprite* loadScreen = new Sprite(L"resources//menu//loadscreen.dds", XMFLOAT3(0.0f, 0.0f, Const::mainMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, result));
	backgrounds.push_back(loadScreen);

	// создание описателя
	buttonsDesc = new ButtonsDesc[buttons.size()];

	// создание заголовков кнопок и массива описателей
	for (UINT i(0); i < buttons.size(); i++) {
		if (buttons[i]->caption) {
			BRR(font->TextOutXY(buttons[i]->caption, XMFLOAT2(buttons[i]->GetSpriteRelPosition().x - (UINT)buttons[i]->GetDimentions().x / 2, buttons[i]->GetSpriteRelPosition().y - buttons[i]->GetDimentions().y), SA_CENTER, false));
		}
		buttonsDesc[i].caption = buttons[i]->caption ? true : false;
		buttonsDesc[i].enable = buttons[i]->enable;
	}

}

// обновление основного меню
bool MainMenu::Update(MAIN_MENU_RESULT* menuResult) {

	for (UINT i(0); i < buttons.size(); i++) {
		if (buttons[i]->UnderCursor() && Mediator::mouseChange->downButtons[0]) {
			*menuResult = (MAIN_MENU_RESULT)i;
			break;
		}
	}

	return true;

}

// сменить заголовок кнопки
bool MainMenu::ChangeButtonCaption(wchar_t* newCaption, UINT buttonNumber) {

	// поленился писать алгоритм
	if (!buttons[buttonNumber]->caption || !newCaption) {
		Mediator::errors->Push(L"can't change null pointer");
		return false;
	}

	// поиск нужного предложения
	UINT sentenceNumber = 0;
	for (UINT i(0); i < buttonNumber; i++) {
		if (buttonsDesc[i].caption) sentenceNumber++;
	}

	// замена предложения
	BR(buttons[buttonNumber]->ChangeCaption(newCaption));
	BR(font->ReplaceSentence(sentenceNumber, newCaption));
	
	return true;

}

// нарисовать экран загрузки
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

// рисование
bool MainMenu::Draw() {

	// рисование родителя
	BR(Menu::Draw());

	// рисование шрифтов
	BR(font->Draw(buttonsDesc));

	return true;

}

// сброс
bool MainMenu::Reset() {

	// сброс родителя
	Menu* parent = this;
	BR(parent->Reset());

	// сброс шрифтов
	BR(font->Reset());

	return true;

}

// деструктор
MainMenu::~MainMenu() {
	
	ReleaseNULL(font);
	ReleaseNULLS(buttonsDesc);

};