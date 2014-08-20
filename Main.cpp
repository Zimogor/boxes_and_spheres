#include "Main.h"

// главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// поиск утечек
#if defined (DEBUG) || defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// загрузка констант
	if (Const::loadConstants)
		if (!Const::LoadFromFile()) return 0;

	// глобальные классы
	Errors errors; // класс обработки ошибок
	Timer timer; // класс таймера

	// создание элементов игры
	{
		bool result;

		BRR0(ZimApplication zimApplication(hInstance, &result)); // создание приложения
		BRR0(zimApplication.shader = new Shader(&result)); // создание шейдера
		BRR0(zimApplication.console = new Console(&result)); // создание консоли
		BRR0(zimApplication.camera = new Camera(&result)); // создание камеры
		BRR0(zimApplication.mainMenu = new MainMenu(&result)); // создание основного меню

		// запуск приложения
		BRR0(result = zimApplication.Run());

	} // создание элементов игры

	errors.ShowTotalMessage();
	return 0;

}

// сброс приложения
bool ZimApplication::Reset() {

	// сброс объектов
	BR(camera->Reset()); // сброс камеры
	BR(console->Reset()); // сброс консоли
	if (ingameMenu) { BR(ingameMenu->Reset()); } // сброс внутриигрового меню
	BR(mainMenu->Reset()); // сброс основного меню
	if (hud) { BR(hud->Reset()); } // сброс интерфейса

	return true;

}

// обновление
bool ZimApplication::Update(float dT) {

	MAIN_MENU_RESULT menuResult = MMR_NOTHING; // результат главного меню

	// состояние игры
	if (Mediator::keyboardChange->keysShort[KS_ENTER]) {
		if (gameState == GS_GAME) {
			// внутриигровое меню открывается
			gameState = GS_INGAME_MENU;
			BR(ingameMenu->Open());
		}
		else if (gameState == GS_INGAME_MENU) {
			// внутриигровое меню закрывается
			BR(ingameMenu->Close());
			gameState = GS_GAME;
		}
	}
	if (Mediator::keyboardChange->keysShort[KS_ESCAPE] && resLoaded) {
		if (gameState == GS_MAIN_MENU) gameState = GS_GAME; // продолжить игру
		else {
			if (gameState == GS_INGAME_MENU) {
				// внутриигровое меню закрывается
				BR(ingameMenu->Close());
				gameState = GS_GAME;
			}
			else gameState = GS_MAIN_MENU; // выход в главное меню
		}
	}

	// обновление игровых объектов
	switch (gameState) {
	case GS_GAME:
		BR(camera->Update(dT, hero->GetPosition())); // обновление камеры
		BR(shader->Update()); // обновление шейдера
		BR(terrain->Update(camera->GetCamFocus())); // обновление территории
		BR(ingameMenu->MapUpdate()); // обновление карты
		BR(hero->Update(dT, camera->GetCamAzimuth())); // обновление героя
		BR(artInt->Update(dT, terrain->GetShowPoint())); // обновление ИИ
		BR(helper->Update(dT)); // обновление помощника
		break;
	case GS_INGAME_MENU:
		BR(ingameMenu->Update()); // обновление внутриигрового меню
		break;
	case GS_MAIN_MENU:
		BR(mainMenu->Update(&menuResult)); // обновление основного меню
		if (menuResult != MMR_NOTHING) {
			switch (menuResult) {
			case MMR_START: {
				if (!resLoaded) {
					// первый запуск новой игры
					BR(ImmidiateDraw()); // рисование экрана загрузки
					BR(LoadResources(Const::location)); // загрузка ресурсов игры
				}
				gameState = GS_GAME;
				break;
			}
			case MMR_QUIT: PostQuitMessage(0); break;
			}
		}
		break;
	}

	// обновление консоли
	BR(console->Update()); // обновление консоли

	return true;

}

// мгновенное рисование
bool ZimApplication::ImDrawContent() {

	BR(mainMenu->DrawLoadScreen());
	BR(console->Draw());

	return true;

}

// рисование в катру теней
bool ZimApplication::DrawShadow() {
	
	if (gameState == GS_MAIN_MENU) return true;
	BR(shader->SetAlphaBlending(false));
	BR(terrain->Draw(true));
	BR(hero->Draw(true));
	BR(artInt->Draw(true));
	return true;

}

// рисование
bool ZimApplication::Draw() {

	switch (gameState) {
	case GS_INGAME_MENU:
		BR(shader->SetAlphaBlending(true));
		BR(ingameMenu->Draw()); // рисование внутриигрового меню
	case GS_GAME:
		// рисование объектов
		BR(shader->SetAlphaBlending(false));
		BR(hero->Draw(false)); // рисование героя
		BR(artInt->Draw(false)); // рисование ИИ
		BR(terrain->Draw(false)); // рисование территории
		BR(shader->SetAlphaBlending(true));
		BR(helper->Draw()); // рисование помощника
		BR(hud->Draw()); // рисование интерфейса
		break;
	case GS_MAIN_MENU:
		BR(shader->SetAlphaBlending(true));
		BR(mainMenu->Draw()); // рисование основного меню
		break;
	}

	// рисование консоли
	BR(shader->SetAlphaBlending(true));
	BR(console->Draw()); // рисование консоли

	return true;

}

// загрузить ресурсы игры
bool ZimApplication::LoadResources(int location) {

	mainMenu->ChangeButtonCaption(L"ПРОДОЛЖИТЬ", MMR_START);
	bool result;
	BRRC1(superFunct = new SuperFunct(&result)); // создание суперфункций
	Const::spawnPoint.y = superFunct->HeightFunction(Const::spawnPoint.x, Const::spawnPoint.z); // коррекция точки рождения
	BRRC1(ingameMenu = new IngameMenu(&result)); // создание внутриигрового меню
	BRRC1(helper = new Helper(&result)); // создание помощника
	BRRC1(hero = new Hero(&result)); // создание героя
	BRRC1(terrain = new Terrain(location, &result)); // создание территории
	BRRC1(artInt = new ArtInt(&result)); // создание искуственного интеллекта
	BRRC1(hud = new HUD(&result)); // создание интерфейса
	resLoaded = true;

	return true;

}

// деструктор приложнения
ZimApplication::~ZimApplication() {

	ReleaseNULL(hud);
	ReleaseNULL(helper);
	ReleaseNULL(artInt);
	ReleaseNULL(hero);
	ReleaseNULL(terrain);
	ReleaseNULL(ingameMenu);
	ReleaseNULL(mainMenu);
	ReleaseNULL(camera);
	ReleaseNULL(console);
	ReleaseNULL(shader);
	ReleaseNULL(superFunct);

	Const::UnloadConstants(); // выгрузить константы

}