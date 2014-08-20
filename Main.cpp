#include "Main.h"

// ������� �������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// ����� ������
#if defined (DEBUG) || defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �������� ��������
	if (Const::loadConstants)
		if (!Const::LoadFromFile()) return 0;

	// ���������� ������
	Errors errors; // ����� ��������� ������
	Timer timer; // ����� �������

	// �������� ��������� ����
	{
		bool result;

		BRR0(ZimApplication zimApplication(hInstance, &result)); // �������� ����������
		BRR0(zimApplication.shader = new Shader(&result)); // �������� �������
		BRR0(zimApplication.console = new Console(&result)); // �������� �������
		BRR0(zimApplication.camera = new Camera(&result)); // �������� ������
		BRR0(zimApplication.mainMenu = new MainMenu(&result)); // �������� ��������� ����

		// ������ ����������
		BRR0(result = zimApplication.Run());

	} // �������� ��������� ����

	errors.ShowTotalMessage();
	return 0;

}

// ����� ����������
bool ZimApplication::Reset() {

	// ����� ��������
	BR(camera->Reset()); // ����� ������
	BR(console->Reset()); // ����� �������
	if (ingameMenu) { BR(ingameMenu->Reset()); } // ����� �������������� ����
	BR(mainMenu->Reset()); // ����� ��������� ����
	if (hud) { BR(hud->Reset()); } // ����� ����������

	return true;

}

// ����������
bool ZimApplication::Update(float dT) {

	MAIN_MENU_RESULT menuResult = MMR_NOTHING; // ��������� �������� ����

	// ��������� ����
	if (Mediator::keyboardChange->keysShort[KS_ENTER]) {
		if (gameState == GS_GAME) {
			// ������������� ���� �����������
			gameState = GS_INGAME_MENU;
			BR(ingameMenu->Open());
		}
		else if (gameState == GS_INGAME_MENU) {
			// ������������� ���� �����������
			BR(ingameMenu->Close());
			gameState = GS_GAME;
		}
	}
	if (Mediator::keyboardChange->keysShort[KS_ESCAPE] && resLoaded) {
		if (gameState == GS_MAIN_MENU) gameState = GS_GAME; // ���������� ����
		else {
			if (gameState == GS_INGAME_MENU) {
				// ������������� ���� �����������
				BR(ingameMenu->Close());
				gameState = GS_GAME;
			}
			else gameState = GS_MAIN_MENU; // ����� � ������� ����
		}
	}

	// ���������� ������� ��������
	switch (gameState) {
	case GS_GAME:
		BR(camera->Update(dT, hero->GetPosition())); // ���������� ������
		BR(shader->Update()); // ���������� �������
		BR(terrain->Update(camera->GetCamFocus())); // ���������� ����������
		BR(ingameMenu->MapUpdate()); // ���������� �����
		BR(hero->Update(dT, camera->GetCamAzimuth())); // ���������� �����
		BR(artInt->Update(dT, terrain->GetShowPoint())); // ���������� ��
		BR(helper->Update(dT)); // ���������� ���������
		break;
	case GS_INGAME_MENU:
		BR(ingameMenu->Update()); // ���������� �������������� ����
		break;
	case GS_MAIN_MENU:
		BR(mainMenu->Update(&menuResult)); // ���������� ��������� ����
		if (menuResult != MMR_NOTHING) {
			switch (menuResult) {
			case MMR_START: {
				if (!resLoaded) {
					// ������ ������ ����� ����
					BR(ImmidiateDraw()); // ��������� ������ ��������
					BR(LoadResources(Const::location)); // �������� �������� ����
				}
				gameState = GS_GAME;
				break;
			}
			case MMR_QUIT: PostQuitMessage(0); break;
			}
		}
		break;
	}

	// ���������� �������
	BR(console->Update()); // ���������� �������

	return true;

}

// ���������� ���������
bool ZimApplication::ImDrawContent() {

	BR(mainMenu->DrawLoadScreen());
	BR(console->Draw());

	return true;

}

// ��������� � ����� �����
bool ZimApplication::DrawShadow() {
	
	if (gameState == GS_MAIN_MENU) return true;
	BR(shader->SetAlphaBlending(false));
	BR(terrain->Draw(true));
	BR(hero->Draw(true));
	BR(artInt->Draw(true));
	return true;

}

// ���������
bool ZimApplication::Draw() {

	switch (gameState) {
	case GS_INGAME_MENU:
		BR(shader->SetAlphaBlending(true));
		BR(ingameMenu->Draw()); // ��������� �������������� ����
	case GS_GAME:
		// ��������� ��������
		BR(shader->SetAlphaBlending(false));
		BR(hero->Draw(false)); // ��������� �����
		BR(artInt->Draw(false)); // ��������� ��
		BR(terrain->Draw(false)); // ��������� ����������
		BR(shader->SetAlphaBlending(true));
		BR(helper->Draw()); // ��������� ���������
		BR(hud->Draw()); // ��������� ����������
		break;
	case GS_MAIN_MENU:
		BR(shader->SetAlphaBlending(true));
		BR(mainMenu->Draw()); // ��������� ��������� ����
		break;
	}

	// ��������� �������
	BR(shader->SetAlphaBlending(true));
	BR(console->Draw()); // ��������� �������

	return true;

}

// ��������� ������� ����
bool ZimApplication::LoadResources(int location) {

	mainMenu->ChangeButtonCaption(L"����������", MMR_START);
	bool result;
	BRRC1(superFunct = new SuperFunct(&result)); // �������� ������������
	Const::spawnPoint.y = superFunct->HeightFunction(Const::spawnPoint.x, Const::spawnPoint.z); // ��������� ����� ��������
	BRRC1(ingameMenu = new IngameMenu(&result)); // �������� �������������� ����
	BRRC1(helper = new Helper(&result)); // �������� ���������
	BRRC1(hero = new Hero(&result)); // �������� �����
	BRRC1(terrain = new Terrain(location, &result)); // �������� ����������
	BRRC1(artInt = new ArtInt(&result)); // �������� ������������� ����������
	BRRC1(hud = new HUD(&result)); // �������� ����������
	resLoaded = true;

	return true;

}

// ���������� �����������
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

	Const::UnloadConstants(); // ��������� ���������

}