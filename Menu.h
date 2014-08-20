#ifndef MENU_H
#define MENU_H

#include "Sprite.h"
#include "Console.h"

// ����� ������
class Button : public Sprite {

public: 

	// ����� ������
	Button(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, const wchar_t* caption, bool* result); // �����������
	~Button() { ReleaseNULLS(caption); }; // ����������

	// ������ �������
	bool Reset(); // �����

	// �������
	bool UnderCursor(); // ������ ��� �������
	NumXY<float> GetDimentions() { return dimentions; }; // ������� ����������� ������
	bool ChangeCaption(wchar_t* newCaption); // ������� ���������

	// ����������
	wchar_t* caption = NULL; // ���������
	bool enable = true; // �����������
	bool visible = true; // ��������� � �����������
	bool transarent = false; // ������ �����������

private:

	// �������
	bool SetCaption(const wchar_t* caption); // ���������� ���������

	// ����������
	XMFLOAT3 absPosition; // ���������� ���������� ������
	NumXY<float> dimentions; // ����������� ������

};

// ����� ����
class Menu : public Sprite {

public:

	// ����� ����
	Menu(Console* console, bool* result); // �����������
	~Menu(); // ����������

	// ������ �������
	bool Draw(); // ���������
	bool Reset(); // �����

protected:

	// ����� ����������
	Console* console = NULL;

	// ����������
	UINT menuState; // ��������� ����
	std::vector<Button*> buttons; // ������ ����
	std::vector<Sprite*> backgrounds; // ������ ��� ��� ������ �������

};

#endif // MENU_H