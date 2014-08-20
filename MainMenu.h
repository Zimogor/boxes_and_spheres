#ifndef MAINMENU_H
#define MAINMENU_H

#include "Menu.h"
#include "Font.h"

// перечисление кнопок основного меню (должно совпадать с последовательность создания задних фонов и созданием кнопок)
enum MAIN_MENU_STATE { MMS_MAIN = 0 , MMS_LOADSCREEN = 1};

// перечисление результата нажатия кнопки (должно совпадать с последовательность создания кнопок)
enum MAIN_MENU_RESULT { MMR_START = 0, MMR_SAVE = 1, MMR_LOAD = 2, MMR_QUIT = 3, MMR_NOTHING = 255};

// класс основного меню
class MainMenu : public Menu {

public:

	// основное меню
	MainMenu(bool* result); // конструктор
	~MainMenu(); // деструктор

	// важные функции
	bool Update(MAIN_MENU_RESULT* menuResult); // обновление
	bool Draw(); // рисование
	bool DrawLoadScreen(); // нарисовать экран загрузки
	bool Reset(); // сброс

	// функции
	bool ChangeButtonCaption(wchar_t* newCaption, UINT buttonNumber); // сменить заголовок кнопки

private:

	// переменные
	Font* font = NULL; // шрифт
	ButtonsDesc* buttonsDesc = NULL; // описатель кнопок

};

#endif // MAINMENU_H