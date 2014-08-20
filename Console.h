#ifndef CONSOLE_H
#define CONSOLE_H

#include "Sprite.h"
#include "Font.h"

// константы
#define CONSOLE_WIDTH 400.0f // ширина консоли
#define CONSOLE_HEIGHT 450.0f // высота консоли
#define FONT_DISPLACEMENT 2.0f // смещение шрифта от кра€ консоли
const XMFLOAT3 consolePosition = XMFLOAT3(-20.0f, 20.0f, Const::consoleDepth); // положение консоли
const XMFLOAT2 fontStartPosition = XMFLOAT2(consolePosition.x - CONSOLE_WIDTH + FONT_DISPLACEMENT, consolePosition.y + FONT_DISPLACEMENT); // координаты шрифта без учЄта матрицы смещени€

// класс консоли
class Console : public Sprite {

public:

	// консоль
	Console(bool* result); // конструктор
	~Console(); // деструктор

	// новое сообщение
	bool Push(const wchar_t* message); // простое
	template <typename TYPE>
	bool Push(wchar_t* message, TYPE value); // с аргументом
	bool Push(wchar_t* message, CELL_ITEM cellItem); // содержимое €чейки

	// полезные функции
	bool UpdateMainData(UINT FPS, float dT); // обновить основные данные
	bool Update(); // обновление консоли
	bool Draw(); // рисование консоли
	bool Reset(); // сброс консоли

private:

	// переменные
	Font* font = NULL; // первое предложение €вл€етс€ основным
	UINT curStringNumber = 1; // нулева€ строка нужна дл€ основого текста
	std::wofstream debugFile; // файл дл€ записи
	bool show = Const::showConsole; // отображать консоль
	float fontHeight; // высота шрифта
	UINT maxStringNumber; // максимальное количество строк в консоли

};

// сообщение с аргументом
template <typename TYPE>
bool Console::Push(wchar_t* message, TYPE value) {

	// собрать сообщение
	std::wostringstream out;
	out << message << ' ';
	out << value;

	// вывод строки
	BR(font->TextOutXY(out.str().c_str(), XMFLOAT2(fontStartPosition.x, fontStartPosition.y + curStringNumber * fontHeight), SA_TOPRIGHT, false));

	// движение массива строк
	if (curStringNumber < maxStringNumber) curStringNumber++;
	else {
		// сдвинуть вверх
		BR(font->MoveSentences(1, XMFLOAT2(0.0f, -fontHeight), true));
	}

	// запись в файл отладки
	if (Const::writeToDebugFile) {
		debugFile.write(out.str().c_str(), out.str().length());
		debugFile.write(L"\r\n", 2);
	}

	return true;

}

#endif // CONSOLE_H