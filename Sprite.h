#ifndef SPRITE_H
#define SPRITE_H

#include "Utilities.h"
#include "Structures.h"

class Sprite {

public:

	// спрайт
	Sprite(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, SPRITE_ALIGN spriteAlight, bool* result); // конструктор
	~Sprite(); // деструктор

	// важные функции
	bool Draw(); // рисование спрайта
	bool Reset(); // сброс спрайта

	// функции
	XMFLOAT3 GetSpriteRelPosition() { return position; }; // получить относительные координаты спрайта

private:

	// переменные

	// константы
	const SPRITE_ALIGN spriteAlign; // выравнивание спрайта

protected:

	// конструктор
	Sprite(XMFLOAT3 position, NumXY<float> dimentions, SPRITE_ALIGN spriteAlign) : position(position), dimentions(dimentions), spriteAlign(spriteAlign) {}; // для наследников

	// переменные
	bool textured = false; // спрайт с текстурой
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	XMFLOAT4X4 translationF; // матрица перемещения
	UINT vertexAmount = 4; // количество индексов

	// константы
	const XMFLOAT3 position; // координаты спрайта
	const NumXY<float> dimentions; // размерность спрайта

	// функции
	bool CreateVertexBuffer(); // создать буфер вершин
	bool UpdateMatrixTranslation(NumXY<UINT> winDimentions); // обновить матрицу перемещения

};

#endif // SPRITE_H