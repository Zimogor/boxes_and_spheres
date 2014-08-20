#ifndef HERO_H
#define HERO_H

#include "AnimModel.h"
#include "Model.h"

#define HERO_RADIUS 0.3f // радиус геро€

// класс инструмента (оружи€)
class Tool;

// класс геро€
class Hero : public AnimModel {

public:

	// герой
	Hero(bool* result); // конструктор
	~Hero(); // деструктор

	// важные функции
	bool Update(float dT, float camAzimuth); // обновление геро€
	bool Draw(bool toShadowMap); // рисование геро€

	// функции
	void ThrustIntoInventory(InventoryContent invContent); // сунуть предмет в инвентарь
	bool TakeDamage(myXMFLOAT3 damagePoint, float damage); // попытатьс€ получить урон
	bool TakeDamage(float damage); // получить стопудовый урон
	void ChangeTool(); // сменить орудие

private:

	// переменные
	HERO_STATE heroState; // состо€ние геро€
	float freezeTime = 0.0f; // врем€ обездвиживани€ при каком либо процессе
	float waitPoint0 = 0.0f; // вспомогательный временной интервал
	float waitPoint1 = 0.0f; // вспомогательный временной интервал
	InventoryContent inventory[MAX_ITEM_AMOUNT]; // инвентарь
	BoundingBox boundBox; // bounding box
	std::vector<Tool*> tools; // контейнер дл€ орудий
	int curToolIndex = -1; // номер текущего оруди€ в контейнере

	// функции
	INVENTORY_ITEM CellItemIntoInvenotryItem(CELL_ITEM cellItem); // сопроставить предмет €чейки предмету инвентар€
	bool CalculatePicking(HERO_STATE* prevState); // обработка подбирани€, true - если сработал
	bool CalculateAttacking(HERO_STATE *prevState); // обработка действи€, true - если сработал
	bool Die(); // герой погиб

};

// орудие (то, что можно держать в руке)
class Tool : public Model {

public:

	// класс инструмента
	Tool(wchar_t* textureFilePath, wchar_t* binFilePath, float damage, HERO_CLIPS standClip, HERO_CLIPS attackClip, bool* result) : Model(textureFilePath, binFilePath, myXMFLOAT3(0.0f, 3.0f, 0.0f), result), damage(damage), standClip(standClip), attackClip(attackClip) {}; // конструктор

	// используема€ функци€
	bool(Tool::*Action)(myXMFLOAT3 actionPoint) = NULL;
	bool ToolAction(myXMFLOAT3 actionPoint) { return (this->*Action)(actionPoint); };

	// возможные действи€
	bool PickAction(myXMFLOAT3 actionPoint); // удар киркой
	bool AxeAction(myXMFLOAT3 actionPoint); // удар топором

	// переменные
	float damage = 0.0f; // повреждение орудием
	HERO_CLIPS standClip = HC_BATSTAND;
	HERO_CLIPS attackClip = HC_ATTACK0;

};

#endif // HERO_H