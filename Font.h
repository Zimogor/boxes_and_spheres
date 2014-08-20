#ifndef FONT_H
#define FONT_H

#include "Utilities.h"
#include "Structures.h"
#include "MathHelper.h"

// константы
#define LETTER_VERTEX_SIZE 4 // количество вершин на букву
#define LETTER_INDEX_SIZE 6 // количество индексов на букву
#define UNICODE_ENG_DISPLACEMENT 32 // смещение символов в Unicode
#define UNICODE_RUS_DISPLACEMENT 941 // смещение русских символов в Unicode
#define MAX_STRING_LENGTH 128 // максимальная длина слова
#define UNDEFINED_SYMBOL 94 // неопределённый символ

// пакет констант для создания предложения
struct FontConstants {

	// конструкторы
	FontConstants() : height(0), symbolsAmount(0) {};
	FontConstants(float height, UINT symbolsAmount, XMFLOAT4 color) : height(height), symbolsAmount(symbolsAmount), color(color) {};

	float height; // высота строки
	UINT symbolsAmount; // количество символов
	XMFLOAT4 color; // цвет шрифта
	float zDepth; // глубина шрифта

};

// структура описателя
struct Descriptor {

	float left; // левый край
	float right; // правй край
	float top; // верхний край
	float bottom; // нижний край
	unsigned char width; // ширина
	unsigned char height; // высота
	char xOffset; // смещение по X
	char yOffset; // смещение по Y

};

// класс предложения
class Sentence {

public:

	// предложение
	Sentence(const wchar_t* message, XMFLOAT2 position, Descriptor* descriptor, FontConstants fontConstants, SPRITE_ALIGN fontAlign, bool center, bool specialColorB, XMFLOAT3 specialColorF3, bool* result); // конструктор
	~Sentence(); // деструктор

	// важные функции
	bool Draw(); // рисование
	bool Move(XMFLOAT2 changePosition); // сместить предложение
	bool Reset(); // сброс

	// функции
	SPRITE_ALIGN getAlignment() { return fontAlign; }; // вернуть выравнивание предложения
	XMFLOAT2 getPosition() { return position; }; // вернуть координаты предложения
	bool SetPosition(XMFLOAT3 newPosition); // заменить координаты
	bool SetPosition(XMFLOAT2 newPosition); // заменить координаты
	bool specialColorB = false; // использовать особый цвет

private:

	// функции
	bool CreateVertextBuffer(const wchar_t* message, UINT messageLength, Descriptor* descriptor, FontConstants fontConstants); // создать буфер вершин
	bool CreateIndexBuffer(UINT messageLength); // создать буфер индексов
	bool RebuildMatrixTranslation(); // обновить матрицу преобразования

	// переменные
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин
	ID3D11Buffer* pIndexBuffer = NULL; // буфер индексов
	UINT indicesAmount = 0; // количество индексов
	XMFLOAT4X4 translationF; // матрица смещения
	SPRITE_ALIGN fontAlign; // выравнивание предложения
	XMFLOAT2 position; // координаты предложения (верхний левый угол)
	NumXY<int> dimentions = NumXY<int>(); // размерность предложения
	bool center; // координаты заданы относительно центра
	XMFLOAT3 specialColorF; // особый цвет

};

// класс шрифта
class Font {

public:

	// шрифт
	Font(XMFLOAT3 color, FONT_TYPE fontType, float zDehtp, bool* result); // конструктор
	~Font(); // деструктор

	// важные функции
	bool TextOutXY(const wchar_t* text, XMFLOAT2 position, SPRITE_ALIGN fontAlign, bool center, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3()); // создание новой строки (по координатам верхнего левого угла экрана и предложения)
	bool Draw(); // рисование
	bool Draw(ButtonsDesc* buttonsDesc); // рисование с описателем кнопок
	bool Reset(); // сброс

	// функции извлечения
	float GetFontHeight() { return fontConstants.height; }; // получить высоту шрифта
	UINT GetSentenceAmount() { return sentences.size(); }; // количество предложений

	// вспомогительные функции
	bool MoveSentences(UINT start, XMFLOAT2 translation, bool delStart); // сдвинуть все предложения начиная со start
	bool ReplaceSentence(UINT number, const wchar_t* newText); // заменить предложение
	bool DeleteAll(); // удаляет все предложения
	bool Delete(UINT number); // удаляет предложение
	bool SetPosition(UINT sentenceNumber, XMFLOAT3 newPosition); // изменить координаты одного предложения
	bool SetPosition(UINT sentenceNumber, XMFLOAT2 newPosition); // изменить координаты одного предложения

private:

	// функции
	bool CreateConsoleDescriptor(); // создать консольный описатель
	bool CreateSlavicDescriptor(); // создать церковно-славянский описатель

	// переменные
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	Descriptor* descriptor = NULL; // описатель шрифта

	// предложения
	std::vector<Sentence*> sentences;
	FontConstants fontConstants;

};

#endif // FONT_H