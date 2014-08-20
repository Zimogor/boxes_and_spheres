#ifndef CONSOLE_H
#define CONSOLE_H

#include "Sprite.h"
#include "Font.h"

// ���������
#define CONSOLE_WIDTH 400.0f // ������ �������
#define CONSOLE_HEIGHT 450.0f // ������ �������
#define FONT_DISPLACEMENT 2.0f // �������� ������ �� ���� �������
const XMFLOAT3 consolePosition = XMFLOAT3(-20.0f, 20.0f, Const::consoleDepth); // ��������� �������
const XMFLOAT2 fontStartPosition = XMFLOAT2(consolePosition.x - CONSOLE_WIDTH + FONT_DISPLACEMENT, consolePosition.y + FONT_DISPLACEMENT); // ���������� ������ ��� ����� ������� ��������

// ����� �������
class Console : public Sprite {

public:

	// �������
	Console(bool* result); // �����������
	~Console(); // ����������

	// ����� ���������
	bool Push(const wchar_t* message); // �������
	template <typename TYPE>
	bool Push(wchar_t* message, TYPE value); // � ����������
	bool Push(wchar_t* message, CELL_ITEM cellItem); // ���������� ������

	// �������� �������
	bool UpdateMainData(UINT FPS, float dT); // �������� �������� ������
	bool Update(); // ���������� �������
	bool Draw(); // ��������� �������
	bool Reset(); // ����� �������

private:

	// ����������
	Font* font = NULL; // ������ ����������� �������� ��������
	UINT curStringNumber = 1; // ������� ������ ����� ��� �������� ������
	std::wofstream debugFile; // ���� ��� ������
	bool show = Const::showConsole; // ���������� �������
	float fontHeight; // ������ ������
	UINT maxStringNumber; // ������������ ���������� ����� � �������

};

// ��������� � ����������
template <typename TYPE>
bool Console::Push(wchar_t* message, TYPE value) {

	// ������� ���������
	std::wostringstream out;
	out << message << ' ';
	out << value;

	// ����� ������
	BR(font->TextOutXY(out.str().c_str(), XMFLOAT2(fontStartPosition.x, fontStartPosition.y + curStringNumber * fontHeight), SA_TOPRIGHT, false));

	// �������� ������� �����
	if (curStringNumber < maxStringNumber) curStringNumber++;
	else {
		// �������� �����
		BR(font->MoveSentences(1, XMFLOAT2(0.0f, -fontHeight), true));
	}

	// ������ � ���� �������
	if (Const::writeToDebugFile) {
		debugFile.write(out.str().c_str(), out.str().length());
		debugFile.write(L"\r\n", 2);
	}

	return true;

}

#endif // CONSOLE_H