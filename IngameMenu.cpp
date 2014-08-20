#include "IngameMenu.h"
#include "Mediator.h"

// конструктор внутриигровое меню
IngameMenu::IngameMenu(bool* result) : Menu(console, result) {

	BRR(ICONS_INVX_AMOUNT * ICONS_INVY_AMOUNT == MAX_ITEM_INV_AMOUNT);
	BRR(MAX_ITEM_AMOUNT == MAX_ITEM_CRAFT_AMOUNT + MAX_ITEM_INV_AMOUNT + MAX_ITEM_ASSETS_AMOUNT);
	BRR(ITEM_CRAFT_SLOT == MAX_ITEM_INV_AMOUNT + MAX_ITEM_CRAFT_AMOUNT - 1);
	BRR(ITEM_WEAPON_SLOT == MAX_ITEM_INV_AMOUNT + MAX_ITEM_CRAFT_AMOUNT);

	// инициализация переменных
	BRR(*result);
	menuState = MB_INVENTORY;

	// поддерживаемые разрешения (должно совпадать со структурой enum MENU_SIZE)
	{
		// среднее меню
		MenuTypeDescriptions mediumMenu;
		ZeroMemory(&mediumMenu, sizeof(MenuTypeDescriptions));
		mediumMenu.resolutions = NumXY<UINT>(1200, 900);
		mediumMenu.iconDistance = 90;
		mediumMenu.iconDistCraft = 153;
		mediumMenu.invDisp = NumXY<UINT>(456, 77);
		mediumMenu.craftDisp = NumXY<int>(410, -65);
		mediumMenu.mainCraftIcon = NumXY<int>(257, -313);
		mediumMenu.weaponIcon = NumXY<int>(-145, -193);
		mediumMenu.iconSize = 64;
		menuDescs.push_back(mediumMenu);

		// малое меню
		MenuTypeDescriptions smallMenu;
		ZeroMemory(&smallMenu, sizeof(MenuTypeDescriptions));
		smallMenu.resolutions = NumXY<UINT>(800, 600);
		smallMenu.iconDistance = 60;
		smallMenu.iconDistCraft = 106;
		smallMenu.invDisp = NumXY<UINT>(300, 51);
		smallMenu.craftDisp = NumXY<int>(275, -36);
		smallMenu.mainCraftIcon = NumXY<int>(171, -203);
		smallMenu.weaponIcon = NumXY<int>(-98, -126);
		smallMenu.iconSize = 42;
		menuDescs.push_back(smallMenu);
	}

	// определение размера меню
	prevMenuSize = menuSize = DefineMenuSize();

	// создать шрифт
	BRRC2(font = new Font(XMFLOAT3(1.0f, 0.0f, 0.0f), FT_CONSOLE, Const::ingameMenuIconsFontDepth, result));

	// создать буфер экземпляров
	BRR(ReCreateInstanceBuffer());

	// создать буфер вершин
	BRR(ReCreateIconVB());

	if (menuSize == MS_MEDIUM) {
		// создать меню для средних разрешений
		BRR(CreateMediumMenu());
	}
	else {
		// создать меню для низких разрешений
		BRR(CreateSmallMenu());
	}

	// создать рецепты
	BRR(CreateRecipes());

	// создать карту
	BRRC2(map = new Map(result));

}

// буфер вершин иконки (только для сброса и первичной инициализации)
bool IngameMenu::ReCreateIconVB() {

	ReleaseCOM(pVertexBuffer);
	float iconSize = (float)menuDescs[menuSize].iconSize;

	// спрайт с текстурой
	VertPos3Tex2 vertexBufferTex[] = {

		{ XMFLOAT3(0.0f, 0.0f, Const::ingameMenuIconsDepth), XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(iconSize, 0.0f, Const::ingameMenuIconsDepth), XMFLOAT2{ 0.125f, 0.0f } },
		{ XMFLOAT3(0.0f, iconSize, Const::ingameMenuIconsDepth), XMFLOAT2{ 0.0f, 0.125f } },
		{ XMFLOAT3(iconSize, iconSize, Const::ingameMenuIconsDepth), XMFLOAT2{ 0.125f, 0.125f } },

	};

	// структура буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(vertexBufferTex);
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфера вершин
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = (void*)vertexBufferTex;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	return true;

}

// создание буфера экземпляров (только для сброса и первичной инициализации)
bool IngameMenu::ReCreateInstanceBuffer() {

	ReleaseCOM(pInstanceBuffer);

	// создать содержимое буфера экземпляров по инвентарю героя
	PerIconInstance* icons = new PerIconInstance[MAX_ITEM_AMOUNT];
	if (Mediator::inventory) {
		for (UINT i(0); i < MAX_ITEM_AMOUNT; i++) {
			icons[i].position = GetIconPosition(i);
			icons[i].texturePosition = GetTextureFromItem(Mediator::inventory[i].invItem);
		}
	}
	else {
		// первичная инициализация, всё содержимое пустое
		ZeroMemory(icons, sizeof(PerIconInstance)* MAX_ITEM_AMOUNT);
	}

	// структура буфера экземпляров
	D3D11_BUFFER_DESC bufferInstDesc = { 0 };
	bufferInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInstDesc.ByteWidth = sizeof(PerIconInstance)* MAX_ITEM_AMOUNT;
	bufferInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferInstDesc.MiscFlags = 0;
	bufferInstDesc.StructureByteStride = 0;
	bufferInstDesc.Usage = D3D11_USAGE_DYNAMIC;

	// создание буфер экземпляров
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = icons;
	HR(Mediator::pDev->CreateBuffer(&bufferInstDesc, &subIndData, &pInstanceBuffer));

	ReleaseNULLS(icons);

	return true;

}

// извлечь координаты текстуры по иконке
XMFLOAT2 IngameMenu::GetTextureFromItem(INVENTORY_ITEM item) {

	switch (item) {
	case II_NOTHING: return XMFLOAT2(0.0f, 0.0f);
	case II_STICK: return XMFLOAT2(0.125f, 0.0f);
	case II_MUSHROOM: return XMFLOAT2(0.0f, 0.125f);
	case II_FERN: return XMFLOAT2(0.125f, 0.125f);
	case II_CLUB: return XMFLOAT2(0.25f, 0.0f);
	case II_AXE: return XMFLOAT2(0.375f, 0.0f);
	case II_ROPE: return XMFLOAT2(0.25f, 0.125f);
	case II_PICK: return XMFLOAT2(0.375f, 0.125f);
	case II_MEAT: return XMFLOAT2(0.125f, 0.25f);
	case II_FANG: return XMFLOAT2(0.0f, 0.25f);
	case II_ROCK: return XMFLOAT2(0.25f, 0.25f);
	case II_LOG: return XMFLOAT2(0.375, 0.25);
	case II_SWORD: return XMFLOAT2(0.5f, 0.0f);
	}

	return XMFLOAT2(0.0f, 0.0f);

}

// перестроить инвентарь (только при новом открытии меню)
bool IngameMenu::RebuildInventory() {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	PerIconInstance* icons = reinterpret_cast<PerIconInstance*>(mappedData.pData);

	// удалить старые предложения
	BR(font->DeleteAll());

	// создать экземпляры инвентаря
	for (UINT i(0); i < MAX_ITEM_AMOUNT; i++) {

		// содержимое
		XMFLOAT3 iconPos = GetIconPosition(i);
		icons[i].position = iconPos;
		icons[i].texturePosition = GetTextureFromItem(Mediator::inventory[i].invItem);

		// шрифты
		if (Mediator::inventory[i].invItem != II_NOTHING)
			BR(font->TextOutXY(std::to_wstring(Mediator::inventory[i].amount).c_str(), XMFLOAT2(iconPos.x, iconPos.y), SA_TOPLEFT, false));

	}

	Mediator::pDevContext->Unmap(pInstanceBuffer, 0);

	return true;

}

// координаты иконки по её номеру
XMFLOAT3 IngameMenu::GetIconPosition(UINT iconNumber) {

	// инвентарь
	if (iconNumber < MAX_ITEM_INV_AMOUNT) {

		UINT k1 = Mediator::winDimentions->x / 2 - menuDescs[menuSize].invDisp.x; // левый край
		UINT k2 = Mediator::winDimentions->y / 2 + menuDescs[menuSize].invDisp.y; // верхний край
		UINT iconColumn = iconNumber % ICONS_INVX_AMOUNT;
		UINT iconRow = iconNumber / ICONS_INVX_AMOUNT;

		return XMFLOAT3((float)(k1 + iconColumn * menuDescs[menuSize].iconDistance), (float)(k2 + iconRow * menuDescs[menuSize].iconDistance), 0.0f);

	}
	// крафтинг
	else if (iconNumber < MAX_ITEM_INV_AMOUNT + MAX_ITEM_CRAFT_AMOUNT - 1) {

		UINT startNumber = iconNumber - MAX_ITEM_INV_AMOUNT;
		UINT k1 = Mediator::winDimentions->x / 2 - menuDescs[menuSize].craftDisp.x; // левый край
		UINT k2 = Mediator::winDimentions->y / 2 + menuDescs[menuSize].craftDisp.y; // верхний край
		UINT iconColumn = startNumber % ICONS_CRAFTX_AMOUNT;
		UINT iconRow = startNumber / ICONS_CRAFTX_AMOUNT;

		return XMFLOAT3((float)(k1 + iconColumn * menuDescs[menuSize].iconDistCraft), (float)(k2 + iconRow * menuDescs[menuSize].iconDistCraft), 0.0f);

	}
	// уникальные ячейки
	else {

		switch (iconNumber) {
		case ITEM_CRAFT_SLOT: { // главный слот крафта
			return XMFLOAT3((float)(Mediator::winDimentions->x / 2 - menuDescs[menuSize].mainCraftIcon.x), (float)(Mediator::winDimentions->y / 2 + menuDescs[menuSize].mainCraftIcon.y), 0.0f);
		}
		case ITEM_WEAPON_SLOT: { // слот оружия
			return XMFLOAT3((float)(Mediator::winDimentions->x / 2 - menuDescs[menuSize].weaponIcon.x), (float)(Mediator::winDimentions->y / 2 + menuDescs[menuSize].weaponIcon.y), 0.0f);
		}
		}

	}

	return XMFLOAT3(0.0f, 0.0f, 0.0f);

}

// номер иконки по координатам (-1 если промазал)
int IngameMenu::GetIconNumber(NumXY<int> position) {

	float relIconSize = (float)menuDescs[menuSize].iconSize / menuDescs[menuSize].iconDistance; // относительный размер иконки
	int leftBorder, rightBorder, topBorder, bottomBorder;
	UINT halfWinX = Mediator::winDimentions->x / 2;
	UINT halfWinY = Mediator::winDimentions->y / 2;

	// попадание в основной инвентарь
	leftBorder = halfWinX - menuDescs[menuSize].invDisp.x;
	rightBorder = leftBorder + ICONS_INVX_AMOUNT * menuDescs[menuSize].iconDistance;
	topBorder = halfWinY + menuDescs[menuSize].invDisp.y;
	bottomBorder = topBorder + ICONS_INVY_AMOUNT * menuDescs[menuSize].iconDistance;
	if (position.x >= leftBorder && position.x <= rightBorder && position.y >= topBorder && position.y <= bottomBorder) {
		float iconPosFX = ((float)position.x - leftBorder) / (float)menuDescs[menuSize].iconDistance;
		int iconPosUX = (int)floor(iconPosFX);
		float iconPosFY = ((float)position.y - topBorder) / (float)menuDescs[menuSize].iconDistance;
		int iconPosUY = (int)floor(iconPosFY);
		if (iconPosFX - iconPosUX > relIconSize || iconPosFY - iconPosUY > relIconSize) return -1; // попадание между строк
		return iconPosUY * ICONS_INVX_AMOUNT + iconPosUX;
	}

	// попадание в крафт
	leftBorder = halfWinX - menuDescs[menuSize].craftDisp.x;
	rightBorder = leftBorder + ICONS_CRAFTX_AMOUNT * menuDescs[menuSize].iconDistCraft;
	topBorder = halfWinY + menuDescs[menuSize].craftDisp.y;
	bottomBorder = topBorder + ICONS_CRAFTY_AMOUNT * menuDescs[menuSize].iconDistCraft;
	if (position.x >= leftBorder && position.x <= rightBorder && position.y >= topBorder && position.y <= bottomBorder) {
		float iconPosFX = ((float)position.x - leftBorder) / (float)menuDescs[menuSize].iconDistCraft;
		int iconPosUX = (int)floor(iconPosFX);
		float iconPosFY = ((float)position.y - topBorder) / (float)menuDescs[menuSize].iconDistCraft;
		int iconPosUY = (int)floor(iconPosFY);
		if (iconPosFX - iconPosUX > relIconSize || iconPosFY - iconPosUY > relIconSize) return -1;
		return iconPosUX + MAX_ITEM_INV_AMOUNT;
	}

	{// уникальные ячейки

		// слот главного крафта
		leftBorder = halfWinX - menuDescs[menuSize].mainCraftIcon.x;
		rightBorder = leftBorder + menuDescs[menuSize].iconSize;
		topBorder = halfWinY + menuDescs[menuSize].mainCraftIcon.y;
		bottomBorder = topBorder + menuDescs[menuSize].iconSize;
		if (position.x >= leftBorder && position.x <= rightBorder && position.y >= topBorder && position.y <= bottomBorder) {
			return ITEM_CRAFT_SLOT;
		}

		// слот оружия
		leftBorder = halfWinX - menuDescs[menuSize].weaponIcon.x;
		rightBorder = leftBorder + menuDescs[menuSize].iconSize;
		topBorder = halfWinY + menuDescs[menuSize].weaponIcon.y;
		bottomBorder = topBorder + menuDescs[menuSize].iconSize;
		if (position.x >= leftBorder && position.x <= rightBorder && position.y >= topBorder && position.y <= bottomBorder) {
			return ITEM_WEAPON_SLOT;
		}

	} // уникальные ячйка

	return -1;

}

// обновление
bool IngameMenu::Update() {

	// отлов нажатия кнопки меню
	for (UINT i(0); i < buttons.size(); i++)
		if (buttons[i]->UnderCursor() && Mediator::mouseChange->downButtons[0] && buttons[i]->visible) {
			if (i == MB_CRAFT) {
				// скрафтить новый предмет
				makeCraft = true;
				break;
			}
			menuState = (INGAME_MENU_BUTTON)i;
			buttons[MB_CRAFT]->visible = (menuState == MB_INVENTORY);
			break;
		}

	// работа с иконками
	if (menuState == MB_INVENTORY) BR(CalculateIcons());

	makeCraft = false;
	return true;

}

// открытие меню
bool IngameMenu::Open() {

	RebuildInventory();

	return true;

}

// закрытие меню
bool IngameMenu::Close() {

	// закрытие меню
	BR(Mediator::SendEventMenuClose());

	return true;

}

// работа с иконками
bool IngameMenu::CalculateIcons() {

	static bool sticked = false; // иконка схвачена
	static int stickedIcon = 0; // индекс схваченой иконки
	int newIcon = 0; // новый индекс иконки
	bool returnIcon = false; // вернуть иконку на место
	bool changeIcons = false; // поменять иконки местами
	int edibleItem = -1; // предмед для съедания правой кнопкой
	static XMFLOAT3 oldIconPos; // старые координаты схваченой иконки
	static UINT stickedFont = 0; // координаты схваченого шрифта

	// съесть гриб
	if (Mediator::mouseChange->downButtons[1]) {
		edibleItem = GetIconNumber(Mediator::mouseChange->cursorPosition);
		if (edibleItem != -1) {
			if (Mediator::inventory[edibleItem].invItem == II_MUSHROOM) {
				
				// удалить гриб
				if (Mediator::inventory[edibleItem].amount == 1) {					
					// удалить иконку
					Mediator::inventory[edibleItem].amount = 0;
					Mediator::inventory[edibleItem].invItem = II_NOTHING;
					BR(RecreateFont());
				}
				else {
					// уменьшить количество
					Mediator::inventory[edibleItem].amount -= 1;
					BR(RecreateFont());
				}

				// повреждение герою
				Mediator::SendMobEventDamage(-5.0f);

			}
		}
	}

	// управление мышью и параметрами
	if (Mediator::mouseChange->downButtons[0]) {
		stickedIcon = GetIconNumber(Mediator::mouseChange->cursorPosition);
		if (stickedIcon != -1) {
			// иконка прилипла
			if (Mediator::inventory[stickedIcon].invItem != II_NOTHING) {
				sticked = true;
				oldIconPos = GetIconPosition(stickedIcon);
				stickedFont = 0;
				// определить номер строки прилипшей иконки
				for (int i(0); i < stickedIcon; i++)
					if (Mediator::inventory[i].invItem != II_NOTHING) stickedFont++;
			}
		}
	}
	if (Mediator::mouseChange->upButtons[0] && sticked) {
		sticked = false;
		newIcon = GetIconNumber(Mediator::mouseChange->cursorPosition);
		if (newIcon == -1) returnIcon = true; // возврат иконки на место
		else changeIcons = true; // иконка перемещана

	}

	// иконка схвачена
	if (sticked || returnIcon || changeIcons || makeCraft || edibleItem != -1) {

		// копировать иконки
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(Mediator::pDevContext->Map(pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		PerIconInstance* icons = reinterpret_cast<PerIconInstance*>(mappedData.pData);
		for (UINT i(0); i < MAX_ITEM_AMOUNT; i++) {
			icons[i].position = GetIconPosition(i);
			icons[i].texturePosition = GetTextureFromItem(Mediator::inventory[i].invItem);
		}

		// иконка летает с курсором
		if (sticked) {
			int iconHalfSize = menuDescs[menuSize].iconSize / 2;
			XMFLOAT3 iconPosition = XMFLOAT3((float)Mediator::mouseChange->cursorPosition.x - iconHalfSize, (float)Mediator::mouseChange->cursorPosition.y - iconHalfSize, -Const::ingameMenuIconsUpDepth);
			icons[stickedIcon].position = iconPosition;
			BR(font->SetPosition(stickedFont, XMFLOAT3(iconPosition.x, iconPosition.y, -Const::ingameMenuIconsUpDepth)));
		}

		// поменять иконки местами
		if (changeIcons) {

			// поменять иконки
			icons[stickedIcon].texturePosition = GetTextureFromItem(Mediator::inventory[newIcon].invItem);
			icons[newIcon].texturePosition = GetTextureFromItem(Mediator::inventory[stickedIcon].invItem);

			// поменять инвентарь
			InventoryContent swapInventory = Mediator::inventory[stickedIcon];
			Mediator::inventory[stickedIcon] = Mediator::inventory[newIcon];
			Mediator::inventory[newIcon] = swapInventory;
			BR(RecreateFont());

		}

		// вернуть иконку на место
		if (returnIcon) {
			BR(font->SetPosition(stickedFont, XMFLOAT3(oldIconPos.x, oldIconPos.y, 0.0f)));
		}

		// скрафтить новый предмет
		if (makeCraft) {
			BR(CraftNewItem(icons));
			BR(RecreateFont());
		}

		Mediator::pDevContext->Unmap(pInstanceBuffer, 0);

	}

	return true;

}

// пересоздать шрифты заново
bool IngameMenu::RecreateFont() {

	// обновить шрифты целиком
	BR(font->DeleteAll());
	for (UINT i(0); i < MAX_ITEM_AMOUNT; i++) {
		XMFLOAT3 iconPos = GetIconPosition(i);
		if (Mediator::inventory[i].invItem != II_NOTHING)
			BR(font->TextOutXY(std::to_wstring(Mediator::inventory[i].amount).c_str(), XMFLOAT2(iconPos.x, iconPos.y), SA_TOPLEFT, false));
	}

	return true;

}

// рисование
bool IngameMenu::Draw() {

	Menu* parent = this;
	BR(parent->Draw());

	switch (menuState) {

	// рисование иконок
	case MB_INVENTORY: {
		// установить параметры
		UINT vertexStrides[2] = { sizeof(VertPos3Tex2), sizeof (PerIconInstance) };
		UINT vertexOffsets[2] = { 0, 0 };
		ID3D11Buffer* vertInstBuffers[2] = { pVertexBuffer, pInstanceBuffer };
		Mediator::pDevContext->IASetVertexBuffers(0, 2, vertInstBuffers, vertexStrides, vertexOffsets);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutSpriteInstance);
		Mediator::pDevContext->VSSetShader(Mediator::pVSSpriteInstance, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPosTex, NULL, 0);
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);

		// рисование иконок
		Mediator::pDevContext->DrawInstanced(VERT_AMOUNT_PER_INSTANCE, MAX_ITEM_AMOUNT, 0, 0);

		// рисование шрифтов
		BR(font->Draw());

	} break;

	// рисование карты
	case MB_MAP:

		// рисование карты
		BR(map->Draw());

		break;
	}

	return true;

}

// сброс
bool IngameMenu::Reset() {

	// сброс меню
	menuSize = DefineMenuSize();
	if (prevMenuSize == menuSize) {
		// размер прежний
		Menu* parent = this;
		BR(parent->Reset());
	}
	else {
		// размер изменился
		prevMenuSize = menuSize;
		BR(ChangeMenuSize(menuSize));
	}

	// сброс иконок
	BR(ReCreateInstanceBuffer());
	BR(ReCreateIconVB());
	BR(RecreateFont());

	return true;

}

// определить подходящий размер меню
MENU_SIZE IngameMenu::DefineMenuSize() {

	if (Mediator::winDimentions->x >= menuDescs[MS_MEDIUM].resolutions.x && Mediator::winDimentions->y >= menuDescs[MS_MEDIUM].resolutions.y) {
		return MS_MEDIUM;
	}

	return MS_SMALL;

}

// изменить размер меню
bool IngameMenu::ChangeMenuSize(MENU_SIZE menuSize) {

	// удаление кнопок
	for (UINT i(0); i < buttons.size(); i++) {
		ReleaseNULL(buttons[i]);
	}
	buttons.clear();

	// удаление задних фонов
	for (UINT i(0); i < backgrounds.size(); i++) {
		ReleaseNULL(backgrounds[i]);
	}
	backgrounds.clear();

	// удаление пиктограмм
	ReleaseCOM(pSRtexture);

	this->menuSize = menuSize;

	// создать новое меню
	if (menuSize == MS_MEDIUM) {
		// создать меню для средних разрешений
		BR(CreateMediumMenu());
	}
	else {
		// создать меню для низких разрешений
		BR(CreateSmallMenu());
	}

	return true;

}

// создать меню для низких разрешений
bool IngameMenu::CreateSmallMenu() {

	bool result;

	// создание пиктограмм
	HR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources\\menu\\pics_small.dds", NULL, NULL, &pSRtexture, NULL));

	// создание кнопок (должно совпадать с enum INGAME_MENU_STATE и с созданием задних фонов)
	BRRC1(Button* butInventory = new Button(NULL, XMFLOAT3(-300.0f, -275.0f, Const::ingameMenuButtonsDepth), NumXY<float>(168.0f, 48.0f), NULL, &result));
	buttons.push_back(butInventory);
	BRRC1(Button* butMap = new Button(NULL, XMFLOAT3(-141.0f, -275.0f, Const::ingameMenuButtonsDepth), NumXY<float>(147.0f, 48.0f), NULL, &result));
	buttons.push_back(butMap);
	BRRC1(Button* butSkilltree = new Button(NULL, XMFLOAT3(6.0f, -275.0f, Const::ingameMenuButtonsDepth), NumXY<float>(144.0f, 48.0f), NULL, &result));
	buttons.push_back(butSkilltree);
	// кнопка крафта
	BRRC1(Button* butCraft = new Button(NULL, XMFLOAT3(-156.0f, -103.0f, Const::ingameMenuButtonsDepth), NumXY<float>(69.0f, 67.0f), NULL, &result));
	buttons.push_back(butCraft);

	// создание задних фонов (должно совпадать с enum INGAME_MENU_STATE и с созданием кнопок)
	BRRC1(Sprite* spriteInventory = new Sprite(L"resources//menu//s_inventory_small.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteInventory);
	BRRC1(Sprite* spriteMap = new Sprite(L"resources//menu//s_map_small.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteMap);
	BRRC1(Sprite* spriteSkilltree = new Sprite(L"resources//menu//s_skilltree_small.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(800.0f, 600.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteSkilltree);

	return true;

}

// создать меню для средних разрешений
bool IngameMenu::CreateMediumMenu() {

	bool result;

	// создание пиктограмм
	HR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources\\menu\\pics_medium.dds", NULL, NULL, &pSRtexture, NULL));

	// создание кнопок (должно совпадать с enum INGAME_MENU_STATE и с созданием задних фонов)
	BRRC1(Button* butInventory = new Button(NULL, XMFLOAT3(-445.0f, -413.0f, Const::ingameMenuButtonsDepth), NumXY<float>(248.0f, 67.0f), NULL, &result));
	buttons.push_back(butInventory);
	BRRC1(Button* butMenu = new Button(NULL, XMFLOAT3(-208.0f, -413.0f, Const::ingameMenuButtonsDepth), NumXY<float>(220.0f, 67.0f), NULL, &result));
	buttons.push_back(butMenu);
	BRRC1(Button* butSkilltree = new Button(NULL, XMFLOAT3(10.0f, -413.0f, Const::ingameMenuButtonsDepth), NumXY<float>(212.0f, 67.0f), NULL, &result));
	buttons.push_back(butSkilltree);
	// кнопка крафта
	BRRC1(Button* butCraft = new Button(NULL, XMFLOAT3(-239.0f, -158, Const::ingameMenuButtonsDepth), NumXY<float>(99.0f, 101.0f), NULL, &result));
	buttons.push_back(butCraft);

	// создание задних фонов (должно совпадать с enum INGAME_MENU_STATE и с созданием кнопок)
	BRRC1(Sprite* spriteInventory = new Sprite(L"resources//menu//s_inventory_medium.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(1200, 900.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteInventory);
	BRRC1(Sprite* spriteMap = new Sprite(L"resources//menu//s_map_medium.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(1200.0f, 900.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteMap);
	BRRC1(Sprite* spriteSkilltree = new Sprite(L"resources//menu//s_skilltree_medium.dds", XMFLOAT3(0.0f, 0.0f, Const::ingameMenuBackgroundDepth), NumXY<float>(1200.0f, 900.0f), SA_CENTER, &result));
	backgrounds.push_back(spriteSkilltree);

	return true;

}

// деструктор
IngameMenu::~IngameMenu() {

	ReleaseNULL(map);
	ReleaseNULL(recipes);
	ReleaseNULL(font);
	ReleaseCOM(pInstanceBuffer);
	ReleaseCOM(pVertexBuffer);
	ReleaseCOM(pSRtexture);

};