#include "Application.h"
#include "Mediator.h"

Application* thisApplication = NULL;

// обработка событий windows
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	return thisApplication->appWinProc(hWnd, msg, wParam, lParam);

}

// обработка событий
LRESULT Application::appWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_DESTROY:
		// выход из приложения
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		// нажатие кнопки
		if (!(lParam & 0x40000000)) BRQ(GetWMKeyPress(wParam, true));
		return 0;
	case WM_KEYUP:
		// отпуск кнопки
		BRQ(GetWMKeyPress(wParam, false));
		return 0;
	case WM_GETMINMAXINFO:
		// ограничение размеров окна
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = minWinDimentions.x;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = minWinDimentions.y;
		return 0;
	case WM_EXITSIZEMOVE:
		// завершение перемещения приложения
		BRQ(ApplicationReset());
		return 0;
	case WM_SIZE:
		// изменение размеров окна
		BRQ(GetWMSize(wParam));
		return 0;
	case WM_MOUSEWHEEL:
		// движение скроллинга
		mouseChange.wheelShift = GET_WHEEL_DELTA_WPARAM(wParam);
		return 0;
	case WM_MOUSEMOVE:
		// шевеление мышкой
		GetWMMouseMove(lParam);
		return 0;
	case WM_LBUTTONDOWN:
		// левая кнопка мыши вниз
		mouseChange.downButtons[0] = true;
		mouseChange.holdButtons[0] = true;
		return 0;
	case WM_RBUTTONDOWN:
		// правая кнопка мыши вниз
		mouseChange.downButtons[1] = true;
		mouseChange.holdButtons[1] = true;
		return 0;
	case WM_LBUTTONUP:
		// левая кнопка мыши вверх
		mouseChange.upButtons[0] = true;
		mouseChange.holdButtons[0] = false;
		return 0;
	case WM_RBUTTONUP:
		// правая кнопка мыши вверх
		mouseChange.upButtons[1] = true;
		mouseChange.holdButtons[1] = false;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

// шевеление мышкой
bool Application::GetWMMouseMove(LPARAM lParam) {

	// вычисление коодинат и смещений мышки
	NumXY<int> cursorPosition(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	static NumXY<int> cursorPositionPrev = cursorPosition;
	mouseChange.cursorShift = cursorPosition - cursorPositionPrev;
	mouseChange.cursorPosition = cursorPosition;
	cursorPositionPrev = cursorPosition;

	return true;

}

// нажатие кнопок
bool Application::GetWMKeyPress(WPARAM wParam, bool down) {

	if (down) {

		// кнопка вниз
		switch (wParam) {
		case 'W': case VK_UP: keyboardChange.keys[KB_W] = true; break;
		case 'A': case VK_LEFT: keyboardChange.keys[KB_A] = true; break;
		case 'S': case VK_DOWN: keyboardChange.keys[KB_S] = true; break;
		case 'D': case VK_RIGHT: keyboardChange.keys[KB_D] = true; break;
		case VK_TAB: keyboardChange.keysShort[KS_TAB] = true; break;
		case VK_SPACE: keyboardChange.keysShort[KS_SPACE] = true; break;
		case VK_RETURN: keyboardChange.keysShort[KS_ENTER] = true; break;
		case VK_ESCAPE: keyboardChange.keysShort[KS_ESCAPE] = true; break;
		case VK_CONTROL: keyboardChange.keysShort[KS_CTRL] = true; break;
		case VK_F1:  keyboardChange.keysShort[KS_F1] = true; break;
		case VK_F2:  keyboardChange.keysShort[KS_F2] = true; break;
		case VK_F3:  keyboardChange.keysShort[KS_F3] = true; break;
		}

	}
	else {

		// кнопка вверх
		switch (wParam) {
		case 'W': case VK_UP: keyboardChange.keys[KB_W] = false; break;
		case 'A': case VK_LEFT: keyboardChange.keys[KB_A] = false; break;
		case 'S': case VK_DOWN: keyboardChange.keys[KB_S] = false; break;
		case 'D': case VK_RIGHT: keyboardChange.keys[KB_D] = false; break;
		}
	}

	return true;

}

// изменение размеров окна
bool Application::GetWMSize(WPARAM wParam) {

	// ничего не делать во время запуска
	if (!pDev) return true;

	// изменение размера
	switch (wParam) {
	case SIZE_MAXIMIZED:
		// на весь экран
		BR(ApplicationReset());
		winMaximized = true;
		break;
	case SIZE_RESTORED:
		if (!winMaximized) break;
		// восстановлен из полного экрана
		BR(ApplicationReset());
		winMaximized = false;
		break;
	}

	return true;

}

// конструктор приложения
Application::Application(HINSTANCE hInstance, bool* result) {

	ZeroMemory(&refreshRate, sizeof(DXGI_RATIONAL));
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	ZeroMemory(&shadowMapViewport, sizeof(D3D11_VIEWPORT));

	// копирование ссылок
	thisApplication = this;
	this->hInstance = hInstance;
	for (int i(0); i < 4; i++) {
		backColor[i] = 0.0f;
	}

	// проверка поддерживаемого функционала
	BRR(CheckAbility());

	// создание окна
	BRR(CreateWin());

	// инициализация DirectX
	BRR(InitDirect3D());

	// выходные параметры
	Mediator::featureLevel = featureLevel;
	Mediator::pDevContext = pDevContext;
	Mediator::pDev = pDev;
	Mediator::winDimentions = &winDimentions;
	Mediator::keyboardChange = &keyboardChange;
	Mediator::mouseChange = &mouseChange;

	*result = true;

}

// проверка поддерживаемого функционала
bool Application::CheckAbility() {

	// проверка на запуск двух экземпляров
	CreateMutex(NULL, true, L"zimApplicationNew");
	BR((GetLastError() != ERROR_ALREADY_EXISTS));

	// поддержка XNA
	BR(DirectX::XMVerifyCPUSupport());

	return true;

}

// сброс в приложении
bool Application::ApplicationReset() {

	// удаление устаревших параметров
	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pRenderTargetView);

	// обновление SwapChain
	RECT rect;
	BR(GetClientRect(hWnd, &rect));
	winDimentions.x = rect.right - rect.left;
	winDimentions.y = rect.bottom - rect.top;
	HR(pSwapChain->ResizeBuffers(1, winDimentions.x, winDimentions.y, DXGI_FORMAT_MINE, 0));

	// обновление RenderView
	ID3D11Texture2D* pBackBuffer = NULL;
	HR(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer));
	HR(pDev->CreateRenderTargetView(pBackBuffer, 0, &pRenderTargetView));
	ReleaseCOM(pBackBuffer);

	// обновление DepthStencilView
	D3D11_TEXTURE2D_DESC pTextureDesc = { 0 };
	pTextureDesc.ArraySize = 1;
	pTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pTextureDesc.CPUAccessFlags = 0;
	pTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pTextureDesc.Height = winDimentions.y;
	pTextureDesc.MipLevels = 1;
	pTextureDesc.MiscFlags = 0;
	pTextureDesc.SampleDesc = sampleDesc;
	pTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	pTextureDesc.Width = winDimentions.x;
	ID3D11Texture2D* pDepthStencilBuffer = NULL;
	HR(pDev->CreateTexture2D(&pTextureDesc, NULL, &pDepthStencilBuffer));
	HR(pDev->CreateDepthStencilView(pDepthStencilBuffer, 0, &pDepthStencilView));
	ReleaseCOM(pDepthStencilBuffer);

	// обновление установки RenderView и DepthStencilView
	pDevContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// обновление Viewport
	viewport.Height = (float)winDimentions.y;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)winDimentions.x;
	pDevContext->RSSetViewports(1, &viewport);

	// внутренний сброс
	BR(Reset());

	BR(Mediator::console->Push(L"reset", winDimentions));

	return true;

}

// запуск приложения
bool Application::Run() {

	// инициализация переменных
	BR(Mediator::timer->Init());
	float dT = 0.0f;
	if (featureLevel < D3D_FEATURE_LEVEL_11_0) {
		Mediator::console->Push(L"DirectX11 not supported");
	}
	Mediator::console->Push(L"client resolution ", winDimentions);

	// основной цикл
	MSG msg = { 0 };
	while (true) {

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			// обновление в приложении
			BR(ApplicationUpdate(dT));

			// рисование в приложении
			BR(ApplicationDraw());

			// обновить данные таймера
			if (Mediator::timer->Tick(&dT)) {
				BR(Mediator::console->UpdateMainData(Mediator::timer->FPS, Mediator::timer->frameTime));
			}

			// включение тормозов
			if (dT > 0.03f && Const::breakDT) {
				std::wostringstream out;
				out << "lf: " << MathHelper::Round(dT) << " u:" << MathHelper::Round(updateTime) << " d:" << MathHelper::Round(drawTime - updateTime) << " p:" << MathHelper::Round(presentTime - updateTime - (drawTime - updateTime));
				BR(Mediator::console->Push(out.str().c_str()));
				dT = 0.03f;
			}

		}

	} // while (true)

	return true;

}

// создание окна
bool Application::CreateWin() {

	// описание класса окна
	WNDCLASS wndClass = { 0 };
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(113));
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(110));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WinProc;
	wndClass.lpszClassName = L"zimWndClass";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// создание класса окна
	BR(RegisterClass(&wndClass));

	// создание окна
	NumXY<UINT> leftWinCorner;
	RECT clientRect = { 0, 0, winDimentions.x, winDimentions.y };
	BR(AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, false));
	screenDimentions.x = GetSystemMetrics(SM_CXSCREEN);
	screenDimentions.y = GetSystemMetrics(SM_CYSCREEN);
	leftWinCorner = (screenDimentions - winDimentions) / 2;
	BR(hWnd = CreateWindow(L"zimWndClass", L"zim game", WS_OVERLAPPEDWINDOW, leftWinCorner.x, leftWinCorner.y, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, NULL, NULL, hInstance, NULL));

	// пересчёт минимальных размеров
	RECT minClientRect = { 0, 0, minWinDimentions.x, minWinDimentions.y };
	BR(AdjustWindowRect(&minClientRect, WS_OVERLAPPEDWINDOW, false));
	minWinDimentions.x = minClientRect.right - minClientRect.left;
	minWinDimentions.y = minClientRect.bottom - minClientRect.top;

	// отображение окна
	if (winMaximized) {
		ShowWindow(hWnd, SW_MAXIMIZE);
		BR(GetClientRect(hWnd, &clientRect));
		winDimentions.x = clientRect.right - clientRect.left;
		winDimentions.y = clientRect.bottom - clientRect.top;
	}
	else {
		ShowWindow(hWnd, SW_SHOW);
	}
	BR(UpdateWindow(hWnd));

	return true;

}

// инициализация Direct3D
bool Application::InitDirect3D() {

	// создание pDev и pDevContext
	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined (DEBUG) || defined (_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HR(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, NULL, D3D11_SDK_VERSION, &pDev, &featureLevel, &pDevContext));
	if (featureLevel < D3D_FEATURE_LEVEL_10_0) {
		Mediator::errors->Push(L"DirectX10 not supported");
		return false;
	}

	// проверка мультисэмплирования
	UINT sampleQuality = 0;
	HRESULT hr = pDev->CheckMultisampleQualityLevels(DXGI_FORMAT_MINE, 4, &sampleQuality);
	if (SUCCEEDED(hr) && sampleQuality > 0 && Const::multisampling) {
		// мультисэмплирование поддерживается
		sampleDesc.Count = 4;
		sampleDesc.Quality = sampleQuality - 1;
	}
	else {
		// мультисэмплирование не поддерживается
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;
	}

	// создание SwapChain
	{

		// определение родительских интерфейсов
		IDXGIDevice* dxgiDevice = NULL;
		HR(pDev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
		IDXGIAdapter* dxgiAdapter = NULL;
		HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
		IDXGIFactory* dxgiFactory = NULL;
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

		// определение частоты работы монитора
		IDXGIOutput* pOutput = NULL;
		HR(dxgiAdapter->EnumOutputs(0, &pOutput));
		UINT numModes = 0;
		HR(pOutput->GetDisplayModeList(DXGI_FORMAT_MINE, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));
		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
		HR(pOutput->GetDisplayModeList(DXGI_FORMAT_MINE, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));
		bool findAdapterMode = false;
		for (UINT i(0); i < numModes; i++)
			if (displayModeList[i].Width == screenDimentions.x && displayModeList[i].Height == screenDimentions.y) {
				findAdapterMode = true;
				refreshRate = displayModeList[i].RefreshRate;
				break;
			}
		delete[] displayModeList;
		ReleaseCOM(pOutput);
		BR(findAdapterMode);

		// создание swapChain
		DXGI_MODE_DESC modeDesc = { 0 };
		modeDesc.Format = DXGI_FORMAT_MINE;
		modeDesc.Height = winDimentions.y;
		modeDesc.RefreshRate = refreshRate;
		modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		modeDesc.Width = winDimentions.x;
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc = modeDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Flags = 0;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = true;
		HR(dxgiFactory->CreateSwapChain(pDev, &swapChainDesc, &pSwapChain));

		// отключить Alt+Enter
		HR(dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES));

		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

	} // создание SwapChain

	// создание target view
	ID3D11Texture2D* pBackBuffer = NULL;
	HR(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer));
	HR(pDev->CreateRenderTargetView(pBackBuffer, 0, &pRenderTargetView));
	if (pBackBuffer) pBackBuffer->Release();

	// создание DepthStencilBuffer
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Height = winDimentions.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.MiscFlags = 0;
	depthBufferDesc.SampleDesc = sampleDesc;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.Width = winDimentions.x;
	ID3D11Texture2D* pDepthBuffer = NULL;
	HR(pDev->CreateTexture2D(&depthBufferDesc, 0, &pDepthBuffer));
	HR(pDev->CreateDepthStencilView(pDepthBuffer, NULL, &pDepthStencilView));
	if (pDepthBuffer) pDepthBuffer->Release();

	// итоговое связывание
	pDevContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// установка viewport
	viewport.Height = (float)winDimentions.y;
	viewport.MaxDepth = 1;
	viewport.MinDepth = 0;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)winDimentions.x;
	pDevContext->RSSetViewports(1, &viewport);

	// установка viewport для теней
	shadowMapViewport.Width = (float)Const::shadowMapSize.x;
	shadowMapViewport.Height = (float)Const::shadowMapSize.y;
	shadowMapViewport.MinDepth = 0.0f;
	shadowMapViewport.MaxDepth = 1.0f;
	shadowMapViewport.TopLeftX = 0;
	shadowMapViewport.TopLeftY = 0;

	return true;

}

// обновление в приложении
bool Application::ApplicationUpdate(float dT) {

	if (Const::sleepTime) Sleep(Const::sleepTime);

	// внутреннее обновление
	BR(Update(dT));

	// очистить переменные
	BR(mouseChange.Clear());
	BR(keyboardChange.Clear());

	updateTime = Mediator::timer->GetTickTime();

	return true;

}

// мгновенное рисование
bool Application::ImmidiateDraw() {

	// настройки для обычного рендера
	pDevContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	pDevContext->ClearRenderTargetView(pRenderTargetView, backColor);
	pDevContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pDevContext->RSSetViewports(1, &viewport);

	// внутреннее рисование
	BR(ImDrawContent());

	// presenting
	HR(pSwapChain->Present(0, 0));

	return true;

}

// рисование в приложении
bool Application::ApplicationDraw() {

	ID3D11ShaderResourceView* pSRV = Mediator::shader->GetShadowResourceView();

	{ // первый проход по теням

		if (Const::shadowsOn) {
			pDevContext->OMSetRenderTargets(0, NULL, Mediator::shader->GetShadowMapView());
			pDevContext->ClearDepthStencilView(Mediator::shader->GetShadowMapView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
			pDevContext->RSSetViewports(1, &shadowMapViewport);
			BR(DrawShadow());
		}

	} // первый проход по теням

	{ // основноый проход

		// настройки для обычного рендера
		pDevContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
		pDevContext->ClearRenderTargetView(pRenderTargetView, backColor);
		pDevContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		pDevContext->RSSetViewports(1, &viewport);
		// передать shadow map в шейдер
		if (Const::shadowsOn) Mediator::pDevContext->PSSetShaderResources(SHADOW_BUFFER, 1, &pSRV);
		// внутреннее рисование
		BR(Draw());
		drawTime = Mediator::timer->GetTickTime();
		// забрать shadow map из шейдера
		ID3D11ShaderResourceView* nullResource[] = { NULL };
		Mediator::pDevContext->PSSetShaderResources(SHADOW_BUFFER, 1, nullResource);

	} // основной проход

	// presenting
	HR(pSwapChain->Present(Const::vSync, 0));
	presentTime = Mediator::timer->GetTickTime();

	return true;

}

// деструктор
Application::~Application() {

	ReleaseCOM(pDepthStencilView);
	ReleaseCOM(pRenderTargetView);
	ReleaseCOM(pSwapChain);
	ReleaseCOM(pDev);
	ReleaseCOM(pDevContext);

}