#include "Hero.h"
#include "Mediator.h"

// конструктор героя 
Hero::Hero(bool* result) : AnimModel(L"resources\\animated_models\\hero.bin", L"resources\\animated_models\\hero.dds", result) {

	// проверка данных
	BRR(*result);
	BRR(clips.size() == 13);

	// инициализация переменных
	heroState = HS_STAND;
	InitFinalMatrices(bonesAmount);

	// инициализация инвентаря
	for (int i(0); i < MAX_ITEM_AMOUNT; i++) {
		inventory[i] = InventoryContent(II_NOTHING, 0);
	}
	if (Const::fullInventory) {
		inventory[0] = InventoryContent(II_MUSHROOM, 50);
		inventory[1] = InventoryContent(II_FERN, 75);
		inventory[2] = InventoryContent(II_STICK, 150);
		inventory[3] = InventoryContent(II_CLUB, 1);
		inventory[4] = InventoryContent(II_ROPE, 10);
		inventory[5] = InventoryContent(II_PICK, 1);
		inventory[6] = InventoryContent(II_AXE, 1);
		inventory[7] = InventoryContent(II_MEAT, 10);
		inventory[8] = InventoryContent(II_FANG, 15);
		inventory[9] = InventoryContent(II_ROCK, 20);
		inventory[10] = InventoryContent(II_LOG, 30);
		inventory[11] = InventoryContent(II_SWORD, 1);
		inventory[ITEM_WEAPON_SLOT] = InventoryContent(II_CLUB, 1);
	}

	// создание оборудования !!! должно соответствовать INVENNORY_ITEM !!!
	BRRC2(Tool* club = new Tool(L"resources\\textures\\club.dds", L"resources\\static_models\\club.bin", 5, HC_BATSTAND, HC_BATATTACK0, result)); // дубина
	BRRC2(Tool* axe = new Tool(L"resources\\textures\\axe.dds", L"resources\\static_models\\axe.bin", 2, HC_STAND, HC_BATATTACK2, result)); // топор
	BRRC2(Tool* sword = new Tool(L"resources\\textures\\sword.dds", L"resources\\static_models\\sword.bin", 10, HC_BATSTAND, HC_BATATTACK2, result)); // меч
	BRRC2(Tool* pickaxe = new Tool(L"resources\\textures\\pickaxe.dds", L"resources\\static_models\\pickaxe.bin", 2, HC_STAND, HC_BATATTACK2, result)); // кирка

	// функции для орудий
	pickaxe->Action = &Tool::PickAction;
	axe->Action = &Tool::AxeAction;

	// создание вектора орудий
	tools.push_back(club);
	tools.push_back(axe);
	tools.push_back(sword);
	tools.push_back(pickaxe);

	// создать boundingbox
	boundBox = BoundingBox(NumXY<float>(Const::spawnPoint.x, Const::spawnPoint.z), HERO_RADIUS);

	// начальная анимация и начальное оружие
	if (inventory[ITEM_WEAPON_SLOT].invItem >= II_MINTOOL) {
		SetCurrentClip(HC_BATSTAND);
		curToolIndex = inventory[ITEM_WEAPON_SLOT].invItem - II_MINTOOL;
	}
	else {
		SetCurrentClip(HC_STAND);
	}

	// отдать переменные посреднику
	Mediator::inventory = inventory;
	Mediator::hero = this;
	Mediator::heroBoundBox = &boundBox;

	*result = true;

}

// сменить инструмент
void Hero::ChangeTool() {

	SetCurrentClip((inventory[ITEM_WEAPON_SLOT].invItem == II_CLUB || inventory[ITEM_WEAPON_SLOT].invItem == II_SWORD) ? HC_BATSTAND : HC_STAND);
	curToolIndex = max(-1, inventory[ITEM_WEAPON_SLOT].invItem - II_MINTOOL);

}

// сопоставить предмет ячейки предмету инвентаря
INVENTORY_ITEM Hero::CellItemIntoInvenotryItem(CELL_ITEM cellItem) {

	switch (cellItem) {
	case CI_FERN: return II_FERN;
	case CI_BRUSH: return II_STICK;
	case CI_MUSHROOM: return II_MUSHROOM;
	}

	return II_NOTHING;

}

// обновление героя
bool Hero::Update(float dT, float camAzimuth) {

	if (freezeTime) freezeTime -= dT; // время заморозки

	// предыдущее состояние героя
	static HERO_STATE prevState = heroState;

	if (heroState == HS_DEAD) { // мёртвые не шевелятся
		if (freezeTime >= 0.0f) BR(SingleAnimatedModel::Update(dT));
		return true;
	}

	// родительское обновление
	BR(SingleAnimatedModel::Update(dT));

	// подбирание предметов
	if (CalculatePicking(&prevState)) return true;

	// атака
	if (CalculateAttacking(&prevState)) return true;

	// локальное перемещение
	myXMFLOAT3 localTranslation;
	if (Mediator::keyboardChange->keysShort[KS_F1]) { BR(Die()); return true; }; // самоубийство
	if (Mediator::keyboardChange->keysShort[KS_F2]) { BR(TakeDamage(position, 1)); return true; }; // убавить HP
	if (Mediator::keyboardChange->keysShort[KS_F3]) { BR(TakeDamage(position, -1)); return true; }; // добавить HP
	if (Mediator::keyboardChange->keys[KB_W]) localTranslation.x += 1.0f;
	if (Mediator::keyboardChange->keys[KB_S]) localTranslation.x -= 1.0f;
	if (Mediator::keyboardChange->keys[KB_A]) localTranslation.z += 1.0f;
	if (Mediator::keyboardChange->keys[KB_D]) localTranslation.z -= 1.0f;
	float absTranslation = abs(localTranslation.x) + abs(localTranslation.z);
	if (absTranslation == 2) {
		localTranslation.x *= 0.71f;
		localTranslation.z *= 0.71f;
	}
	localTranslation *= Const::heroSpeed * dT;

	// определение клипа анимации
	heroState = absTranslation ? HS_WALK : HS_STAND;
	if (prevState != heroState) {
		if (inventory[ITEM_WEAPON_SLOT].invItem == II_CLUB || inventory[ITEM_WEAPON_SLOT].invItem == II_SWORD) SetCurrentClip(heroState == PS_WALK ? HC_BATRUN : HC_BATSTAND);
		else SetCurrentClip(heroState == PS_WALK ? HC_RUN : HC_STAND);
		prevState = heroState;
	}

	// глобальное перемещение
	XMMATRIX R = XMMatrixRotationY(camAzimuth + XM_PIDIV2);
	XMVECTOR translation = { localTranslation.x, localTranslation.y, localTranslation.z };
	translation = XMVector3Transform(translation, R);
	float transX = XMVectorGetX(translation);
	float transZ = XMVectorGetZ(translation);
	NumXY<float> potentialPos(position.x + transX, position.z + transZ); // предполагамемое перемещение
	boundBox.SetPosition(potentialPos);
	NumXY<int> regAdr = MathHelper::GetDynamicRegionAdress(potentialPos);
	NumXY<int> contAdr = MathHelper::GetContentAdress(potentialPos);

	// упёрся
	if (Mediator::regionField[regAdr.x][regAdr.y]->getContent()[contAdr.x][contAdr.y].blocked || Mediator::artInt->CheckObject(boundBox, true)) {
		boundBox.SetPosition(NumXY<float>(position.x, position.z));
		return true;
	}

	position.x += transX;
	position.z += transZ;
	position.y = Mediator::superFunct->HeightFunction(position.x, position.z);

	// угол поворота
	if (absTranslation) {
		SetRotAngle(atan2f(-transX, -transZ));
	}

	// обновление оружия
	if (curToolIndex != -1) BR(tools[curToolIndex]->SetPosition(position));

	return true;

}

// сунуть предмет в инвентарь
void Hero::ThrustIntoInventory(InventoryContent invContent) {

	// сунуть в инвентарь
	int firstEmpty = -1; // первая пустая ячейка
	int alreadyExists = -1; // ближайшая ячейка с аналогичным предметом
	for (int i(0); i < MAX_ITEM_AMOUNT; i++) {
		if (inventory[i].invItem == invContent.invItem) {
			alreadyExists = i;
			break;
		}
		if (firstEmpty == -1 && inventory[i].invItem == II_NOTHING) {
			firstEmpty = i;
		}
	}
	if (alreadyExists != -1) inventory[alreadyExists].amount++;
	else {
		inventory[firstEmpty].invItem = invContent.invItem;
		inventory[firstEmpty].amount = invContent.amount;
	}

}

// обработка действия, true - если сработал
bool Hero::CalculateAttacking(HERO_STATE* prevState) {

	const float defaultDistance = 0.5f;

	// процесс действия
	if (heroState == HS_ATTACK) {
		if (freezeTime < waitPoint0) {

			// нанесение действия в точку
			waitPoint0 = -FLT_MAX;
			myXMFLOAT3 damagePoint = MathHelper::GetPositionFromAngle(GetDrawAngle(), position, ATTACK_DISTANCE);
			if (curToolIndex != -1) {
				BR(Mediator::SendHeroEventDamage(damagePoint, tools[curToolIndex]->damage));
				if (tools[curToolIndex]->Action) {
					tools[curToolIndex]->ToolAction(damagePoint);
				}
			}
			else {
				// безоружный удар
				BR(Mediator::SendHeroEventDamage(damagePoint, 1));
			}

		}
		if (freezeTime < waitPoint1) {
			// можно двигаться
			SetAngleSpeed(ANGLE_SPEED);
			*prevState = heroState = HS_STAND;
		}
		else return true;
	}

	// инициализация действия
	if (Mediator::mouseChange->downButtons[0] && freezeTime <= 0.0f) {

		// точка земли под курсором
		XMFLOAT3 rayPointF = Mediator::GetTerrainUnderCursor();

		// начало атаки
		if (curToolIndex != -1) {
			// вооружён
			freezeTime = SetCurrentClip( tools[curToolIndex]->attackClip, tools[curToolIndex]->standClip);
		}
		else {
			// голые руки
			freezeTime = SetCurrentClip(HC_ATTACK0, HC_STAND);
		}

		waitPoint0 = freezeTime * 0.5f;
		waitPoint1 = freezeTime * 0.25f;
		*prevState = heroState = HS_ATTACK;

		// поворот героя
		float transX = rayPointF.x - position.x;
		float transZ = rayPointF.z - position.z;
		SetRotAngle(atan2f(-transX, -transZ));
		SetAngleSpeed(ATACK_ANGLE_SPEED);

		return true;

	} // инициализация действия

	return false;

}

// попытаться получить урон
bool Hero::TakeDamage(myXMFLOAT3 damagePoint, float damage) {

	if (MathHelper::Distance(position, damagePoint) <= 1.0f) {
		
		BR(TakeDamage(damage));
	}

	return true;

}
bool Hero::TakeDamage(float damage) {

	// летающий текст
	Mediator::helper->SetFloatText(position, (int)damage, true, XMFLOAT3(1.0f, 0.0f, 0.0f));

	// получение урона
	hitPoints -= damage;
	Mediator::SendLifeChange(hitPoints / maxHitPoints);
	if (hitPoints <= 0) {
		BR(Die());
	};
	if (hitPoints > maxHitPoints) hitPoints = maxHitPoints;

	return true;

}

// обработка подбирания, true - если сработал
bool Hero::CalculatePicking(HERO_STATE* prevState) {

	// процесс подбирания
	if (heroState == HS_PICK) {
		if (freezeTime <= waitPoint0) {

			*prevState = heroState = HS_STAND;

			// вырвать с территории
			CELL_ITEM cellItemResult;
			if (Mediator::SendPickup(position, &cellItemResult)) {

				// сунуть в инвентарь
				INVENTORY_ITEM toInvItem = CellItemIntoInvenotryItem(cellItemResult);
				ThrustIntoInventory(InventoryContent(toInvItem, 1));
			}
		}
		else return true;
	}

	// инициализация подбирания
	if (Mediator::keyboardChange->keysShort[KS_SPACE] && freezeTime <= 0.0f) {

		freezeTime = (inventory[ITEM_WEAPON_SLOT].invItem == II_CLUB) ? SetCurrentClip(HC_PICK, HC_BATSTAND) : SetCurrentClip(HC_PICK, HC_STAND);
		*prevState = heroState = HS_PICK;
		waitPoint0 = freezeTime * 0.2f;
		return true;

	}

	return false;

}

// рисование героя
bool Hero::Draw(bool toShadowMap) {

	// рисование родителя
	AnimModel* parent = this;
	BR(parent->Draw(toShadowMap));

	if (toShadowMap) return true;

	// рисование оборудования
	if (inventory[ITEM_WEAPON_SLOT].invItem != II_NOTHING) {
		XMMATRIX boneAnim = XMLoadFloat4x4(&finalMatrices[11]);
		XMMATRIX R = XMLoadFloat4x4(&GetRotationMatrix());
		XMMATRIX W = boneAnim * R;
		if (curToolIndex != -1) { BR(tools[curToolIndex]->Draw(W)); }

	}

	// рисовать bounding box
	if (Const::showBoundingBox) {

		UINT vertexStride = sizeof(VertPos3Nor3);
		UINT vertexOffset = 0;
		Mediator::pDevContext->IASetVertexBuffers(0, 1, &Mediator::helper->pVertexCylinderBuffer, &vertexStride, &vertexOffset);
		Mediator::pDevContext->IASetIndexBuffer(Mediator::helper->pIndexCylinderBuffer, DXGI_FORMAT_R32_UINT, 0);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3);
		Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSSolid, NULL, 0);

		// рисование
		BR(boundBox.Draw(position));

	}

	return true;

}

// герой погиб
bool Hero::Die() {

	Mediator::console->Push(L"die");
	heroState = HS_DEAD;
	freezeTime = SetCurrentClip(HC_DIE);

	return true;

}

// дополнительные действия для оборудования
bool Tool::PickAction(myXMFLOAT3 actionPoint) { // действие для кирки

	BR(Mediator::SendEventWorking(E_PICKAXE, actionPoint));
	return true;
}
bool Tool::AxeAction(myXMFLOAT3 actionPoint) { // действие для топора

	BR(Mediator::SendEventWorking(E_CHOP, actionPoint));
	return true;

}

// деструктор
Hero::~Hero() {

	// удаление инструментария и оружия
	for (UINT i(0); i < tools.size(); i ++) {
		ReleaseNULL(tools[i]);
	}

}
