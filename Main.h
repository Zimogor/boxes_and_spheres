#ifndef MAIN_H
#define MAIN_H

#include "Mediator.h"
#include "Application.h"
#include "Timer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Sprite.h"
#include "Console.h"
#include "Terrain.h"
#include "Hero.h"
#include "ArtInt.h"
#include "MainMenu.h"
#include "IngameMenu.h"
#include "Helper.h"
#include "HUD.h"

class ZimApplication : public Application {

public:

	// приложение
	ZimApplication(HINSTANCE hInstance, bool* result) : Application(hInstance, result) {}; // конструктор
	~ZimApplication(); // деструктор

	// виртуальные функции
	virtual bool Draw(); // рисование приложения
	virtual bool ImDrawContent(); // мгновенное рисование
	virtual bool Update(float dT); // обновление приложения
	virtual bool Reset(); // сброс приложения
	virtual bool DrawShadow(); // рисование в карту теней

	// объекты приложения
	SuperFunct* superFunct = NULL; // класс суперфункций
	Shader* shader = NULL; // шейдер
	Console* console = NULL; // консоль
	Camera* camera = NULL; // камера
	MainMenu* mainMenu = NULL; // основное меню
	IngameMenu* ingameMenu = NULL; // внутриигровое меню
	Terrain* terrain = NULL; // территория
	Hero* hero = NULL; // герой
	ArtInt* artInt = NULL; // искусственный интеллект
	Helper* helper = NULL; // помощник
	HUD* hud = NULL; // интерфейс

	// функции
	bool LoadResources(int location); // загрузить ресурсы игры

	// переменные
	GAME_STATE gameState = GS_MAIN_MENU; // состояние игры
	bool resLoaded = false; // игровые ресурсы загружены

};


#endif // MAIN_H