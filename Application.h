#ifndef APPLICATION_H
#define APPLICATION_H

#include "Console.h"
#include "Timer.h"

#define DEFAULT_WIN_WIDTH 800
#define DEFAULT_WIN_HEIGHT 600
#define DXGI_FORMAT_MINE DXGI_FORMAT_R8G8B8A8_UNORM

// ����� ����������
class Application {

public:

	//����������
	Application(HINSTANCE hInstance, bool* result); // �����������
	~Application(); // ����������

	// ������ ����������
	bool Run();

	// ����������� �������
	bool ImmidiateDraw(); // ���������� ���������
	virtual bool Draw() = 0; // ���������
	virtual bool DrawShadow() = 0; // ��������� � ����� �����
	virtual bool Update(float dT) = 0; // ����������
	virtual bool Reset() = 0; // ����� ����������
	virtual bool ImDrawContent() = 0; // ���������� ����������� ���������

	// ��������� �������
	LRESULT appWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	// ����������
	HINSTANCE hInstance = 0; // ��������� �� ���������
	HWND hWnd = 0; // ��������� �� ����
	ID3D11Device* pDev = NULL; // pDev
	ID3D11DeviceContext* pDevContext = NULL; // pDevContext
	IDXGISwapChain* pSwapChain = NULL; // pSwapChain
	ID3D11RenderTargetView* pRenderTargetView = NULL; // pRenderTargetView
	ID3D11DepthStencilView* pDepthStencilView = NULL; // pDepthStencilView
	bool winMaximized = Const::winMaximized; // ���������
	float updateTime, drawTime, presentTime; // ������� ������� ��� �����
	D3D_FEATURE_LEVEL featureLevel; // ��������� DirectX11
	NumXY<UINT> winDimentions = NumXY<UINT>(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT); // ����������� ���� �� ���������
	NumXY<UINT> minWinDimentions = NumXY<UINT>(800, 600); // ���������� ���������� ������� ����
	NumXY<UINT> screenDimentions = NumXY<UINT>(); // ���������� ��������
	DXGI_SAMPLE_DESC sampleDesc; // �������������������
	float backColor[4]; // ������ ���
	DXGI_RATIONAL refreshRate; // ������� ���������� ��������
	D3D11_VIEWPORT viewport; // ���� ��� �����������
	D3D11_VIEWPORT shadowMapViewport; // ���� ��� �����

	// ������� �������
	bool ApplicationDraw(); // ��������� � ����������
	bool ApplicationReset(); // ����� � ����������
	bool ApplicationUpdate(float dT); // ���������� � ����������

	// ������ ����������
	MouseChange mouseChange; // ���������� �����
	KeyboardChange keyboardChange; // ���������� �����������

	// ������������� ����������
	bool CheckAbility(); // �������� ��������������� �����������
	bool CreateWin(); // �������� ����
	bool InitDirect3D(); // ������������� DirectX

	// ������ �������
	bool GetWMSize(WPARAM wParam); // ��������� �������� ����
	bool GetWMMouseMove(LPARAM lParam); // ��������� ������
	bool GetWMKeyPress(WPARAM wParam, bool down); // ������� ������

};

#endif // APPLICATION_H