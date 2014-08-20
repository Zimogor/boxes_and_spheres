#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "Nums.h"
#include "Utilities.h"

#define CELL_ITEM int

// структуры буферов шейдера
struct CbufAnimModel { // для анимационных моделей

	XMFLOAT4X4 gWorld; // мировая матрица
	XMFLOAT4X4 gBones[32]; // массив матриц костей
};
struct CbufReset { // для камеры

	XMFLOAT4X4 gOrthoMatrix; // матрица ортонормального проецирования
};
struct CbufObject { // для объектов

	XMFLOAT4X4 gWorld; // мировая матрица
};
struct CbufFrame1 { // единственная инициализация

	XMFLOAT4 lightVector; // направление света (пиксельный)
};
struct CbufFont { // для шрифтов

	XMFLOAT4 fontColor; // цвет шрифта
};
struct CbufFrame2 { // для каждого кадра

	XMFLOAT4 camPos; // координаты камеры
	XMFLOAT4 camFocus; // фокус камеры
	XMFLOAT4X4 gViewProjMatrix; // матрицы камеры
};
struct CbufFrame3 { // для карты теней

	XMFLOAT4X4 gViewLightMatrix; // матрица источника света (относительно камеры)
	XMFLOAT4X4 gViewLightMatrixAbs; // матрица источника света (абсолютная)
	XMFLOAT4X4 gProjLightMatrix; // матрицы проекции света
};
struct CbufOnce { // вечные константы

	XMFLOAT4 shadowMapDimentons; // размерность карты теней
};

// структуры вершин
struct VertPos3 { // для спрайтов без текстуры

	XMFLOAT3 position;
};
struct VertPos3Tex2 { // для спрайтов с текстурой

	XMFLOAT3 poisition;
	XMFLOAT2 texture;
};
struct VertPos3Nor3 { // для моделей без текстуры

	XMFLOAT3 position;
	XMFLOAT3 normal;
};
struct VertPos3Nor3Tex2 { // для моделей с текстурой

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
};
struct VertTerrain { // для территории

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 tangent; // для normal mapping
	XMFLOAT2 texture1;
	XMFLOAT2 texture2;
	XMFLOAT2 maskTex; // текстура маски
};
struct VertPos3Nor2Tex2W1Bone2 { // для анимационных моделей

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	float weight;
	UINT bones[2];
};
struct VertInstance { // для вектора моделей с текстурой

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	XMFLOAT3 instancePos;
};
struct VertMap { // для карты

	XMFLOAT3 position;
	XMFLOAT3 color;
};

// структура движения мышью
struct MouseChange {

	short wheelShift; // вращение колеса
	NumXY<int> cursorShift; // смещение курсора
	bool downButtons[2]; // кнопки мыши нажаты
	bool upButtons[2]; // кнопки отжаты
	bool holdButtons[2]; // кнопки зажаты
	NumXY<int> cursorPosition; // коорднаты курсора
	MouseChange() {
		Clear();
	}
	bool Clear(){
		ZeroMemory(&wheelShift, sizeof(short));
		cursorShift.x = 0; cursorShift.y = 0;
		ZeroMemory(downButtons, sizeof(downButtons));
		ZeroMemory(upButtons, sizeof(upButtons));
		return true;
	}

};

// структура работы с клавиатурой
struct KeyboardChange {

	bool keys[4]; // клавиши удержания
	bool keysShort[8]; // клавиши нажатия
	KeyboardChange() {
		ZeroMemory(this, sizeof(KeyboardChange));
	}
	bool Clear() {
		ZeroMemory(keysShort, sizeof(keysShort));
		return true;
	}

};

// предметы содержимого ячейки ( !!! не накосячить с порядоком пострения вектора моделей !!!)
enum CELL_ITEM_L1 { CI_BRUSH, CI_FERN, CI_MUSHROOM, CI_FIR, CI_BOLDFIR, CI_TREE, CI_BOULDER, CI_REED, CI_SUNFLOWER, CI_TUSSOCK, CI_DANDELION, CI_WHEAT, CI_CAMOMILE, CI_STUMP, CI_NONE }; // для базовой (первой) локации
enum CELL_ITEM_L2 { CI_TT1, CI_TT2, CI_TT3 }; // для локации входа (второй)

// типы ресурсов для базовой (первой) локации
#define PICKABLE_FROM_L1 CI_BRUSH
#define PICKABLE_TO_L1 CI_MUSHROOM
#define CHOPABLE_FROM_L1 CI_FIR
#define CHOPABLE_TO_L1 CI_TREE

// типы ресурсов для локации входа (второй)
#define PICKABLE_FROM_L2 -1
#define PICKABLE_TO_L2 -1
#define CHOPABLE_FROM_L2 -1
#define CHOPABLE_TO_L2 -1

// виды биомов
enum BIOMS { B_SWAMP = 0, B_FOREST = 1, B_MEADOW = 2, B_DREAMFOREST = 3, B_TESTED = 4 };

// содержимое ячейки
struct CellFiller {

	// конструкторы
	CellFiller() : item(CI_NONE), blocked(0), biomType(B_TESTED) {};
	CellFiller(CELL_ITEM item, unsigned char blocked) : item(item), blocked(blocked), biomType(B_TESTED) {};

	// состав ячейки
	CELL_ITEM item; // предмет
	unsigned char blocked; // проходимость
	int additional; // прочая хрень
	NumXY<float> itemCenter; // центр лежащего там предмета (экпортировать не надо)
	BIOMS biomType; // тип территории

};

// описатель кнопок
struct ButtonsDesc {
	bool caption;
	bool enable;
};

// предметы содержимого инвенторя
enum INVENTORY_ITEM { II_NOTHING = 0, II_STICK = 1, II_FERN = 3, II_MUSHROOM = 4, II_ROPE = 5, II_MEAT = 6, II_FANG = 7, II_ROCK = 8, II_LOG = 9,

	// орудия и оружие
	II_CLUB = 128, II_AXE = 129, II_SWORD = 130, II_PICK = 131 // !!! минимум должен соответствовать MIN_TOOL !!! и порядок должен соответствовать вектору !!!

};
#define II_MINTOOL II_CLUB // минимальный слот для орудия героя


// структура содержимого инвентаря
struct InventoryContent {

	// конструкторы
	InventoryContent(INVENTORY_ITEM invItem, UINT amount) : invItem(invItem), amount(amount) {};
	InventoryContent() {};

	// содержимое
	INVENTORY_ITEM invItem; // предмет инвентаря
	UINT amount; // количество предметов

};

// клипы персов (должно совпадать с клипами загруженной анимации)
enum PERS_CLIPS{ PC_STAND = 0, PC_WALK = 1, PC_DIE = 2, PC_STRIKE = 3 };

// поведение перса (стоять, идти, умирать, лежать трупом, хорониться под землю)
enum PERS_STATE { PS_STAND = 0, PS_WALK = 1, PS_DIE = 2, PS_CORPSE = 3, PS_BURY = 4, PS_STRIKE = 5};

// клипы героя (должно совпадать с клипами импорта)
enum HERO_CLIPS { HC_BATSTAND = 0, HC_BATATTACK0 = 1, HC_BATATTACK1 = 2, HC_BATATTACK2 = 3, HC_BATDIE = 4, HC_BATRUN = 5, HC_STAND = 6, HC_ATTACK0 = 7, HC_ATTACK1 = 8, HC_ATTACK2 = 9, HC_DIE = 10, HC_RUN = 11, HC_PICK = 12};

// поведение героя
enum HERO_STATE { HS_STAND, HS_WALK, HS_PICK, HS_ATTACK, HS_DEAD };

// виды выравнивания спрайтов
enum SPRITE_ALIGN {SA_TOPLEFT, SA_BOTTOMLEFT, SA_TOPRIGHT, SA_BOTTOMRIGHT, SA_CENTER};

// клавиши клавиатуры
enum KEYBOARD_BUTTONS { KB_W, KB_S, KB_A, KB_D };
enum KEYBOARD_BUTSHORT { KS_TAB, KS_SPACE, KS_ENTER, KS_ESCAPE, KS_CTRL, KS_F1, KS_F2, KS_F3 };

// состояние игры
enum GAME_STATE { GS_INGAME_MENU, GS_GAME, GS_MAIN_MENU };

// типы шрифтов
enum FONT_TYPE { FT_SLAVIC, FT_CONSOLE };

// буферные регистры шейдера
enum SHADER_REGISTORS { SR_ANIMMODEL = 0, SR_CAMERA = 1, SR_MODEL = 2, SR_LIGHTVECTOR = 3, SR_FONT = 4, SR_FRAME = 5, SR_LIGHT = 6, SR_ONCE = 7};

// события воздействия на территорию
enum WIELD_EVENTS { E_PICKAXE, E_CHOP};

#endif // STRUCTURES_H
