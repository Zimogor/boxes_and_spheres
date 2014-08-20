#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Utilities.h"
#include "Nums.h"
#include "Structures.h"

const XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // вектор вверх

#define MAX_ITEM_INV_AMOUNT 44 // количество слотов для инвентаря
#define MAX_ITEM_CRAFT_AMOUNT 4 // количество слотов для крафта (включая основную ячейку)
#define ITEM_CRAFT_SLOT 47 // слот для результатов крафта
#define ITEM_WEAPON_SLOT 48 // слот для оружия
#define MAX_ITEM_AMOUNT 49 // полное количестве всех иконок
#define TEXTURE_BUFFER 0 // константа для текстуры шейдера
#define SHADOW_BUFFER 1 // константа для карты теней
#define NORMALMAP_BUFFER 2 // текстура для normalmapping
#define SAMPLER_BUFFER 0 // константа для сэмплера шейдера
#define SAMPLER_SHADOW 1 // константа для сэмплера шейдера

class Const {

public:

	// приложение
	static bool winMaximized; // окно на весь экран
	static UINT animationSpeed; // скорость анимации (кадров в секунду)
	static float clipBlendSpeed; // скорость смешения анимации
	static bool writeToDebugFile; // запись в файл отладки
	static bool loadConstants; // загрузить константы из файла
	static bool breakDT; // включение искусственных тормозов
	static UINT sleepTime; // искусственное снижение производительности
	static bool showConsole; // отображать консоль

	// видео
	static bool backCulling; // режим backCulling
	static bool detalization; // высокая детализация
	static bool wireFrame; // режим показа сетки
	static bool alpha; // альфа-канал
	static UINT vSync; // вертикальныя синхронизация
	static bool multisampling; // мультисэмплирование
	static bool anisotrophicFilter; // анизотропная фильтрация
	static bool showBoundingBox; // показывать boundingbox

	// территория
	static int regionCellSize; // размер региона территории
	static UINT regionVertexAmount; // количество вершин в регионе
	static UINT regionIndexAmount; // количество индексов в регионе
	static float updateDistance; // растояние на котором обновляется поле регионов
	static float regionHalfCellSize; // половина размера региона в ячейках
	static int regionFieldSize; // размер поля регионов
	static UINT tilesInSquadForest; // количество тайлов в одной текстуре леса
	static UINT tilesInSquadMeadow; // количество тайлов в одной текстуре луга
	static UINT tilesInSquadSwamp; // количество тайлов в одной текстуре болота
	static float widthScatter; // разброс по ширине
	static float heightScatter; // разброс по высоте
	static float positionScatter; // разброс по координатам
	static bool showTiles; // показывать клетки
	static wchar_t* testFileName; // имя тестируемого объекта
	static int location; // номер стартовой локации
	static UINT patchFieldSize; // размер поля патчей
	static int patchCellSize; // размер лоскута в ячейках

	// свет и тени
	static bool shadowsOn; // включить тени
	static NumXY<UINT> shadowMapSize; // разрешение карты теней
	static myXMFLOAT3 lightPosition; // координаты источника света
	static XMFLOAT4 viewRect; // размер проекции света
	static NumXY<float> nearFarLightZ; // отрезание света (x - near, y - far)

	// камера
	static float scrollSpeed; // скорость скроллинга
	static float showRadius; // радиус отображения регионов
	static float maxRadius; // максимальное отдаление
	static float minRadius; // минимальное приближение
	static NumXY<float> pitchLimit; // ограничения тангажа
	static bool releaseCamera; // камера для релиза с ограничениями

	// герой
	static myXMFLOAT3 spawnPoint; // место зарождения героя
	static float heroSpeed; // скорость героя
	static bool fullInventory; // полный инвентарь
	static float distanceToPick; // расстояние подбирания с территории

	// глубины спрайтов
	static float mainMenuBackgroundDepth; // глубина фона основного меню
	static float mainMenuButtonsDepth; // глубина кнопок основного меню
	static float mainMenuButtonsFontDepth; // глубина текста кнопок основного меню
	static float ingameMenuBackgroundDepth; // глубина фона игрового меню
	static float ingameMenuButtonsDepth; // глубина кнопок игрового меню
	static float ingameMenuIconsDepth; // глубина иконок игрового меню
	static float ingameMenuIconsFontDepth; // глубина шрифта иконок игрового меню
	static float ingameMenuIconsUpDepth; // наращивание глубины выделеных иконок
	static float consoleDepth; // глубина консоли
	static float helpFontDepth; // глубина вспомогательного шрифта
	static float mapDepth; // глубина карты

	// функции
	static bool LoadFromFile(); // загрузить константы из файла
	static void UnloadConstants(); // выгрузить константы

private:

	// переменные
	static std::ifstream config; // конфигурационный файл

};

#endif // CONSTANTS_H