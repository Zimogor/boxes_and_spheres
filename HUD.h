#pragma once

#include "Utilities.h"
#include "Sprite.h"

// класс красной жидкости
class RedLiquid : public Sprite {

public:

	// красня жидкость
	RedLiquid(bool* result); // конструктор

	// функции
	bool SetLevel(float amount); // установить количество воды в процентах

private:

	// создание буфера вершин
	bool CreateVertexBuffer(); // создать буфер вершин

};

// класс HUD
class HUD : public Sprite {

public:

	// класс HUD
	HUD(bool* result); // конструктор
	~HUD(); // деструктор

	// важные функции
	bool Reset(); // сброс
	bool Draw(); // рисование

	// функции
	bool SetRedLevel(float amount); // установить новый уровень красной воды в процентах

private:

	// переменные
	RedLiquid* redLiquid = NULL; // красная жидкость для жизней

};