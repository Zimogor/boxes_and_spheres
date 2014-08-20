#ifndef MENU_H
#define MENU_H

#include "Sprite.h"
#include "Console.h"

// класс кнопки
class Button : public Sprite {

public: 

	// класс кнопки
	Button(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, const wchar_t* caption, bool* result); // конструктор
	~Button() { ReleaseNULLS(caption); }; // деструктор

	// важные функции
	bool Reset(); // сброс

	// функции
	bool UnderCursor(); // курсор над кнопкой
	NumXY<float> GetDimentions() { return dimentions; }; // вернуть размерность кнопки
	bool ChangeCaption(wchar_t* newCaption); // сменить заголовок

	// переменные
	wchar_t* caption = NULL; // заголовок
	bool enable = true; // доступность
	bool visible = true; // видимость и доступность
	bool transarent = false; // просто невидимость

private:

	// функции
	bool SetCaption(const wchar_t* caption); // установить заголовок

	// переменные
	XMFLOAT3 absPosition; // абсолютные координаты кнопки
	NumXY<float> dimentions; // размерность кнопки

};

// класс меню
class Menu : public Sprite {

public:

	// класс меню
	Menu(Console* console, bool* result); // конструктор
	~Menu(); // деструктор

	// важные функции
	bool Draw(); // рисование
	bool Reset(); // сброс

protected:

	// копии указателей
	Console* console = NULL;

	// переменные
	UINT menuState; // состояние меню
	std::vector<Button*> buttons; // кнопки меню
	std::vector<Sprite*> backgrounds; // задний фон для разных режимов

};

#endif // MENU_H