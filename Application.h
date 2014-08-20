#ifndef APPLICATION_H
#define APPLICATION_H

#include "Console.h"
#include "Timer.h"

#define DEFAULT_WIN_WIDTH 800
#define DEFAULT_WIN_HEIGHT 600
#define DXGI_FORMAT_MINE DXGI_FORMAT_R8G8B8A8_UNORM

// класс приложения
class Application {

public:

	//приложение
	Application(HINSTANCE hInstance, bool* result); // конструктор
	~Application(); // деструктор

	// запуск приложения
	bool Run();

	// виртуальные функции
	bool ImmidiateDraw(); // мгновенное рисование
	virtual bool Draw() = 0; // рисование
	virtual bool DrawShadow() = 0; // рисование в карту теней
	virtual bool Update(float dT) = 0; // обновление
	virtual bool Reset() = 0; // сброс приложения
	virtual bool ImDrawContent() = 0; // содержимое мгновенного рисования

	// обработка событий
	LRESULT appWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	// переменные
	HINSTANCE hInstance = 0; // указатель на приложние
	HWND hWnd = 0; // указатель на окно
	ID3D11Device* pDev = NULL; // pDev
	ID3D11DeviceContext* pDevContext = NULL; // pDevContext
	IDXGISwapChain* pSwapChain = NULL; // pSwapChain
	ID3D11RenderTargetView* pRenderTargetView = NULL; // pRenderTargetView
	ID3D11DepthStencilView* pDepthStencilView = NULL; // pDepthStencilView
	bool winMaximized = Const::winMaximized; // развёрнуто
	float updateTime, drawTime, presentTime; // периоды времени для тиков
	D3D_FEATURE_LEVEL featureLevel; // поддержка DirectX11
	NumXY<UINT> winDimentions = NumXY<UINT>(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT); // размерность окна по умолчанию
	NumXY<UINT> minWinDimentions = NumXY<UINT>(800, 600); // минимально допустимые размеры окна
	NumXY<UINT> screenDimentions = NumXY<UINT>(); // разрешение монитора
	DXGI_SAMPLE_DESC sampleDesc; // мультисэмплирование
	float backColor[4]; // задний фон
	DXGI_RATIONAL refreshRate; // частота обновления монитора
	D3D11_VIEWPORT viewport; // окно для изображений
	D3D11_VIEWPORT shadowMapViewport; // окно для теней

	// главные функции
	bool ApplicationDraw(); // рисование в приложении
	bool ApplicationReset(); // сброс в приложении
	bool ApplicationUpdate(float dT); // обновление в приложении

	// важные переменные
	MouseChange mouseChange; // управление мышью
	KeyboardChange keyboardChange; // управление клавиатурой

	// инициализация приложения
	bool CheckAbility(); // проверка поддерживаемого функционала
	bool CreateWin(); // создание окна
	bool InitDirect3D(); // инициализация DirectX

	// просто функции
	bool GetWMSize(WPARAM wParam); // изменение размеров окна
	bool GetWMMouseMove(LPARAM lParam); // шевеление мышкой
	bool GetWMKeyPress(WPARAM wParam, bool down); // нажатие кнопки

};

#endif // APPLICATION_H