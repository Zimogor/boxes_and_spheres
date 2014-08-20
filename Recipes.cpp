#include "IngameMenu.h"
#include "Mediator.h"

// создать рецепты
bool IngameMenu::CreateRecipes() {

	Recipe rec[] =  {
		Recipe(II_ROPE, 1, II_STICK, 10, II_NOTHING, 0, II_CLUB, 1),
		Recipe(II_FERN, 5, II_NOTHING, 0, II_NOTHING, 0, II_ROPE, 1),
		Recipe(II_FANG, 2, II_ROPE, 1, II_STICK, 5, II_PICK, 1),
		Recipe(II_ROCK, 1, II_ROPE, 1, II_STICK, 5, II_AXE, 1),
	};

	// выравнивание рецептов по возрастанию
	for (UINT receptNumber(0); receptNumber < ARRAYSIZE(rec); receptNumber++) {

		// для каждого рецепта
		for (UINT j(0); j < ARRAYSIZE(rec[receptNumber].ingredient) - 1; j++) {
			INVENTORY_ITEM maxItem = rec[receptNumber].ingredient[j].invItem;
			UINT maxIndex = j;
			for (UINT i(j + 1); i < ARRAYSIZE(rec[receptNumber].ingredient); i++) {
				if (rec[receptNumber].ingredient[i].invItem > maxItem) {
					maxItem = rec[receptNumber].ingredient[i].invItem;
					maxIndex = i;
				}
			}
			if (maxIndex != j) MathHelper::Swap(&rec[receptNumber].ingredient[maxIndex], &rec[receptNumber].ingredient[j]);
		} // для каждого рецепта
	}

	recipesAmount = ARRAYSIZE(rec);
	recipes = new Recipe[recipesAmount];
	memcpy(recipes, rec, sizeof(rec));

	// проверка на возрастаемость рецептов
	for (UINT j(0); j < recipesAmount; j ++)
		for (UINT i(0); i < MAX_ITEM_CRAFT_AMOUNT - 2; i++) {
			BR(recipes[j].ingredient[i].invItem >= recipes[j].ingredient[i + 1].invItem);
		}

	return true;

}

// создать новый предмет
bool IngameMenu::CraftNewItem(PerIconInstance* icons) {

	// создать копию инвентаря
	UINT inventorySize = MAX_ITEM_CRAFT_AMOUNT - 1;
	InventoryContent inventory[MAX_ITEM_CRAFT_AMOUNT - 1]; 
	UINT indices[MAX_ITEM_CRAFT_AMOUNT - 1];
	for (UINT i(0); i < inventorySize; i++) indices[i] = i;
	memcpy(inventory, &Mediator::inventory[MAX_ITEM_INV_AMOUNT], sizeof(InventoryContent) * (inventorySize));

	// упорядочить копию инвентаря
	for (UINT j(0); j < inventorySize - 1; j++) {
		UINT maxIndex = j;
		InventoryContent maxValue = inventory[j];
		for (UINT i(j + 1); i < inventorySize; i++) {
			if (maxValue.invItem < inventory[i].invItem) {
				maxIndex = i;
				maxValue = inventory[i];
			}
		}
		if (maxIndex != j) {
			MathHelper::Swap(&inventory[maxIndex], &inventory[j]);
			MathHelper::Swap(&indices[maxIndex], &indices[j]);
		}
	}

	// поиск рецепта
	bool recFound = false;
	UINT recNumber;
	for (UINT j(0); j < recipesAmount; j++) {
		for (UINT i(0); i < inventorySize; i++) {
			if (inventory[i].invItem != recipes[j].ingredient[i].invItem || inventory[i].amount < recipes[j].ingredient[i].amount) {
				recFound = false;
				break;
			}
			if (i == inventorySize - 1) {
				recFound = true;
				recNumber = j;
			}
		}
		if (recFound) break;
	}

	// применение рецепта
	INVENTORY_ITEM* invItemCraft = &Mediator::inventory[ITEM_CRAFT_SLOT].invItem;
	if (recFound && ((*invItemCraft == II_NOTHING) || (*invItemCraft == recipes[recNumber].result.invItem))) {

		// сокращение потреблённых ресурсов
		for (UINT i(0); i < inventorySize; i++) {
			inventory[i].amount -= recipes[recNumber].ingredient[i].amount;
		}
		// восстановление упорядоченного массива
		for (UINT i(0); i < inventorySize; i++) {
			Mediator::inventory[MAX_ITEM_INV_AMOUNT + indices[i]] = inventory[i];
		}
		// создание нового элемента
		*invItemCraft = recipes[recNumber].result.invItem;
		Mediator::inventory[ITEM_CRAFT_SLOT].amount += recipes[recNumber].result.amount;
		icons[ITEM_CRAFT_SLOT].texturePosition = GetTextureFromItem(recipes[recNumber].result.invItem);
		// удаление ненужных текстур
		for (UINT i(0); i < inventorySize; i++){
			if (Mediator::inventory[MAX_ITEM_INV_AMOUNT + i].amount == 0) {
				Mediator::inventory[MAX_ITEM_INV_AMOUNT + i].invItem = II_NOTHING;
				icons[MAX_ITEM_INV_AMOUNT + i].texturePosition = GetTextureFromItem(II_NOTHING);
			}
		}

	} // применение рецепта


	return true;

}