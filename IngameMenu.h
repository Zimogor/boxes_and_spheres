#ifndef INGAMEMENU_H
#define INGAMEMENU_H

#include "Menu.h"
#include "Map.h"

#define ICONS_INVX_AMOUNT 11 // длина инвентаря
#define ICONS_INVY_AMOUNT 4 // высота инвентаря
#define ICONS_CRAFTX_AMOUNT 3 // длина крафта
#define ICONS_CRAFTY_AMOUNT 1 // высота крафта
#define VERT_AMOUNT_PER_INSTANCE 4 // количество вершин на один экземпляр иконки
#define MAX_ITEM_ASSETS_AMOUNT 1 // количество слотов для одежды

// структура экземпляра-иконки
struct PerIconInstance {

	XMFLOAT3 position;
	XMFLOAT2 texturePosition;

};

// структура рецепта
struct Recipe {

	Recipe() {};
	Recipe(INVENTORY_ITEM item0, UINT amount0, INVENTORY_ITEM item1, UINT amount1, INVENTORY_ITEM item2, UINT amount2, INVENTORY_ITEM resultItem, UINT resultAmount) {
		ingredient[0].invItem = item0;
		ingredient[0].amount = amount0;
		ingredient[1].invItem = item1;
		ingredient[1].amount = amount1;
		ingredient[2].invItem = item2;
		ingredient[2].amount = amount2;
		result.invItem = resultItem;
		result.amount = resultAmount;
	};
	InventoryContent ingredient[MAX_ITEM_CRAFT_AMOUNT - 1];
	InventoryContent result;

};

// структура для описаний параметро разных разрешений
struct MenuTypeDescriptions {

	NumXY<UINT> resolutions; // поддерживаемое разрешение
	UINT iconDistance; // растояние между иконками
	UINT iconDistCraft; // растояние между иконками в крафте
	NumXY<UINT> invDisp; // смещение от центра экрана для инвентаря
	NumXY<int> craftDisp; // смещение крафта от центра экрана для инветаря
	NumXY<int> mainCraftIcon; // смещение основой ячейки крафта
	NumXY<int> weaponIcon; // икнока с оружием
	UINT iconSize; // размер иконки

};

// размеры меню в зависимости от разрешения экрана (должно совпадать с содержимым resolutions)
enum MENU_SIZE { MS_MEDIUM = 0, MS_SMALL = 1 };

// перечисление кнопок внутриигрового меню (должно совпадать с последовательность создания задних фонов и созданием кнопок)
enum INGAME_MENU_BUTTON { MB_INVENTORY = 0, MB_MAP = 1, MB_SKILLTREE = 2, MB_CRAFT = 3 };

// класс внутриигрового меню
class IngameMenu : public Menu {

public:

	// внутриигровое мению
	IngameMenu(bool* result); // конструктор
	~IngameMenu(); // деструктор

	// важные функции
	bool Update(); // обновление
	bool MapUpdate() { return map->Update(); }; // обновление карты
	bool Draw(); // рисование
	bool Reset(); // сброс

	// функции
	bool Open(); // открытие меню
	bool Close(); // закрытие меню

private:

	// переменные
	std::vector<MenuTypeDescriptions> menuDescs; // поддерживаемые меню в порядке уменьшения размеров
	MENU_SIZE menuSize; // размер меню
	MENU_SIZE prevMenuSize; // предыдущий размер меню
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	ID3D11Buffer* pInstanceBuffer = NULL; // буфер экземпляров иконок
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин иконки
	Font* font = NULL; // шрифт иконок
	Recipe* recipes = NULL; // массив рецептов
	UINT recipesAmount = 0; // количество рецептов
	Map* map = NULL; // карта

	// функции
	MENU_SIZE DefineMenuSize(); // определить подходящий размер меню
	bool ChangeMenuSize(MENU_SIZE menuSize); // изменить размер меню
	bool CreateSmallMenu(); // создать меню для низких разрешений
	bool CreateMediumMenu(); // создать меню для низких разрешений
	bool ReCreateInstanceBuffer(); // создать буфер экземпляров (только для сброса и первичной инициализации)
	bool ReCreateIconVB(); // создать буфер вершин иконки (только для сброса и первичной инициализации)
	bool RecreateFont(); // пересоздать шрифт заново
	XMFLOAT2 GetTextureFromItem(INVENTORY_ITEM item); // извлечь координаты текстуры по иконке
	bool RebuildInventory(); // перестроить инвентарь (только при новом открытии меню)
	XMFLOAT3 GetIconPosition(UINT iconNumber); // координаты иконки по её номеру
	int GetIconNumber(NumXY<int> position); // номер иконки по координатам (-1, если промазал)
	bool CalculateIcons(); // работа с иконками
	bool makeCraft = false; // создать новый предмет
	bool CreateRecipes(); // создать рецепты
	bool CraftNewItem(PerIconInstance* icons); // создать новый предмет

};

#endif // INGAMEMENU_H