#include "SubModels.h"
#include "Mediator.h"
#include "Hero.h"

// ������ ����� �������� ���������
void VectorBoars::GiveSomething() {

	Mediator::hero->ThrustIntoInventory(InventoryContent(II_MEAT, 1));
	if (rand() % 3 == 0) Mediator::hero->ThrustIntoInventory(InventoryContent(II_FANG, 1));

}