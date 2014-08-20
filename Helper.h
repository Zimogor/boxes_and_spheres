#pragma once

#include "SimpleVectorModel.h"
#include "Font.h"

#define FLOAT_TEXT_VELOCITY 1.0f
#define FLOAT_TEXT_LIFETIME 2.0f


// описание плывущего текста
struct PerFloatText {

	PerFloatText(XMFLOAT3 position) : lifeTime(FLOAT_TEXT_LIFETIME), position(position) {}; // инициализация
	XMFLOAT3 position; // координаты
	float lifeTime; // время жизни

};

// класс помощника
class Helper {

public:

	// класс помощника
	Helper(bool *result); // конструктор
	~Helper(); // деструктор

	// важные функции
	bool Update(float dT); // обновление
	bool Draw(); // рисование

	// плавающий текст
	template <typename TYPE>
	bool SetFloatText(myXMFLOAT3 position, TYPE text, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3());
	bool SetFloatText(myXMFLOAT3 position, const wchar_t* text, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3());

	// функции
	bool SetBox(myXMFLOAT3 position, myXMFLOAT3 scale = myXMFLOAT3(1.0f), float rotation = 0.0f); // создать кубик
	bool SetFlyBox(myXMFLOAT3 position, myXMFLOAT3 scale = myXMFLOAT3(1.0f), float rotation = 0.0f); // рисовать кубик подёнок

	// параметры рисования бокса
	ID3D11Buffer* pVertexBoxBuffer = NULL; // буфер вершин для единичного бокса
	ID3D11Buffer* pIndexBoxBuffer = NULL; // буфер индексов для единичного бокса
	UINT boxIndicesAmount = 0; // количество индексов бокса

	// параметры рисования цилиндра
	ID3D11Buffer* pVertexCylinderBuffer = NULL; // буфер вершин единичного цилиндра
	ID3D11Buffer* pIndexCylinderBuffer = NULL; // буфер индексов единичного цилиндра
	UINT cylinderIndicesAmount = 0; // количество индексов цилиндра
	
private:

	// переменные
	SimpleVectorModel* box = NULL; // кубики
	Font* font = NULL; // шрифт
	std::vector<PerFloatText> sentenceParams; // параметры плавающего текста

	// фукнции
	bool CreateCylinder(); // создать цилиндр

};

// шаблон плавающего текста
template<typename TYPE>
bool Helper::SetFloatText(myXMFLOAT3 position, TYPE text, bool specialColorB, XMFLOAT3 specialColorF) {
	return SetFloatText(position, std::to_wstring(text).c_str(), specialColorB, specialColorF);
}