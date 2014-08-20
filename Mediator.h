#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "Utilities.h"
#include "Structures.h"
#include "Errors.h"
#include "Camera.h"
#include "ArtInt.h"
#include "Helper.h"
#include "Shader.h"
#include "Hero.h"
#include "HUD.h"
#include "Map.h"

class Console;
class Terrain;
class Hero;
class Errors;
class SuperFunct;
class Timer;
class DynamicReg;
class HUD;
class Map;

// класс посредника
class Mediator {

public:

	// функции наблюдателя
	static bool SendHeroEventDamage(myXMFLOAT3 position, float damageAmount); // удар героя в точку
	static bool SendMobEventDamage(myXMFLOAT3 position, float damageAmount); // удар моба в точку
	static bool SendMobEventDamage(float damageAmount); // повреждение герою
	static bool SendEventWorking(WIELD_EVENTS events, myXMFLOAT3 position); // работа в точку
	static bool SendPickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult); // подбирание (true - если подобрал)
	static bool SendLifeChange(float amount); // новое количество жизней в процентах
	static bool SendEventMenuClose(); // закрытие внутриигрового меню
	static bool SendEventNewRegion(DynamicReg* dynamicReg); // создание нового региона

	// функции
	static XMFLOAT3 GetTerrainUnderCursor(); // извлечь координаты на поверхности земли под курсором

	// указатели на элементы игры
	static Console* console; // консоль
	static Terrain* terrain; // территория
	static Hero* hero; // герой
	static Errors* errors; // обработчик обшибок
	static SuperFunct* superFunct; // суперфункции
	static Timer* timer; // таймер
	static ArtInt* artInt; // ИИ
	static Helper* helper; // помощник
	static Shader* shader; // шейдер
	static Camera* camera; // камера
	static HUD* hud; // интерфейс
	static Map* map; // карта

	// дополнительные указатели и переменные
	static DynamicReg*** regionField; // поле указателей динамических регионов
	static InventoryContent* inventory; // инвентарь героя
	static MouseChange* mouseChange; // кнопки мыши
	static KeyboardChange* keyboardChange; // кнопки клавиатуры
	static BoundingBox* heroBoundBox; // boundbox героя
	static NumXY<int>* cornerPoint; // относительный угол поля в координатах региона

	// указатели на элементы приложения
	static D3D_FEATURE_LEVEL featureLevel; // уровень DirectX
	static ID3D11Device* pDev; // указатель на pDev
	static ID3D11DeviceContext* pDevContext; // указатель на pDevContext
	static NumXY<UINT>* winDimentions; // размерность окна

	// раскладки вершин
	static ID3D11InputLayout* pInputLayoutPos3Tex2; // для спрайтов с текстурой
	static ID3D11InputLayout* pInputLayoutSpriteInstance; // для иконок меню
	static ID3D11InputLayout* pInputLayoutPos3Nor3; // для моделей без текстуры
	static ID3D11InputLayout* pInputLayoutPos3Nor3Tex2; // для моделей c текстурой
	static ID3D11InputLayout* pInputLayoutTerrain; // для територии
	static ID3D11InputLayout* pInputLayoutPos3Nor3Tex2W1Bone2; // для анимационных моделей
	static ID3D11InputLayout* pInputLayoutInstance; // для вектора моделей с текстурой
	static ID3D11InputLayout* pInputLayoutPosCol; // для карты

	// вершинные шейдеры
	static ID3D11VertexShader* pVSPos; // для спрайтов без текстуры
	static ID3D11VertexShader* pVSPosTex; // для спрайтов с текстурой
	static ID3D11VertexShader* pVSSpriteInstance; // для иконок меню
	static ID3D11VertexShader* pVSPosNor; // для моделей без текстуры
	static ID3D11VertexShader* pVSPosNorTex; // для моделей с текстурами
	static ID3D11VertexShader* pVSTerrain; // для регионов территории
	static ID3D11VertexShader* pVSPosNorTexWBone; // для анимационных моделей с костями
	static ID3D11VertexShader* pVSShadow; // тень анимационных моделей с костями
	static ID3D11VertexShader* pVSInstance; // для векторов моделей
	static ID3D11VertexShader* pVSShadowInstance; // для теней векторов моделей
	static ID3D11VertexShader* pVSPosCol; // для карты

	// пиксельные шейдеры
	static ID3D11PixelShader* pPSPos; // для спрайтов без текстуры
	static ID3D11PixelShader* pPSPosTex; // для спрайтов с текстурой
	static ID3D11PixelShader* pPSFont; // для шрифтов
	static ID3D11PixelShader* pPSSolid; // для сплошных моделей
	static ID3D11PixelShader* pPSPosNor; // для моделей без текстуры
	static ID3D11PixelShader* pPSPosNorTex; // для моделей с текстурой
	static ID3D11PixelShader* pPSClipping; // для моделей с текстурой и обрезанием
	static ID3D11PixelShader* pPSTerrain; // для регионов территории
	static ID3D11PixelShader* pPSTerrainNoShadow; // для регионов территории (без тени)
	static ID3D11PixelShader* pPSEmpty; // пустой теневой шейдер
	static ID3D11PixelShader* pPSPosCol; // lkz rfhns

	// шейдерные буферы
	static ID3D11Buffer* cbufAnimModel; // для анимационных моделей
	static ID3D11Buffer* cbufReset; // для сброса
	static ID3D11Buffer* cbufObject; // для объектов
	static ID3D11Buffer* cbufFont; // для шрифтов
	static ID3D11Buffer* cbufFrame; // для каждого кадра

};

#endif // MEIDATOR_H