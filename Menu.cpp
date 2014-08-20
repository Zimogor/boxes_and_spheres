#include "Menu.h"
#include "Mediator.h"

// ����������� ������
Button::Button(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, const wchar_t* caption, bool* result) : Sprite(filePath, position, dimentions, SA_CENTER, result) {

	BRR(*result);
	if (!filePath) transarent = true;

	// ������������� ���������
	this->absPosition.x = position.x + Mediator::winDimentions->x / 2;
	this->absPosition.y = position.y + Mediator::winDimentions->y / 2;
	this->dimentions = dimentions;
	BRR(SetCaption(caption));

}

// ������ ��� �������
bool Button::UnderCursor() {

	return (MathHelper::InBetween((float)Mediator::mouseChange->cursorPosition.x, absPosition.x - dimentions.x * 0.5f, absPosition.x + dimentions.x * 0.5f) && MathHelper::InBetween((float)Mediator::mouseChange->cursorPosition.y, absPosition.y - dimentions.y * 0.5f, absPosition.y + dimentions.y * 0.5f));

}

// ���������� ���������
bool Button::SetCaption(const wchar_t* caption) {

	if (!caption) return true;
	this->caption = new wchar_t[wcslen(caption) + 1];
	wcscpy_s(this->caption, wcslen(caption) + 1, caption);

	return true;

}

// ������� ���������
bool Button::ChangeCaption(wchar_t* newCaption) {

	ReleaseNULLS(caption);

	if (!newCaption) return true;
	caption = new wchar_t[wcslen(newCaption) + 1];
	wcscpy_s(caption, wcslen(newCaption) + 1, newCaption);

	return true;

}

// ����� ������
bool Button::Reset() {

	// ����� ��������
	Sprite* parent = this;
	BR(parent->Reset());

	// ����� ����������
	absPosition.x = GetSpriteRelPosition().x + Mediator::winDimentions->x / 2;
	absPosition.y = GetSpriteRelPosition().y + Mediator::winDimentions->y / 2;

	return true;

}

// ����������� ����
Menu::Menu(Console* console, bool* result) : Sprite(NULL, XMFLOAT3(0.0f, 0.0f, 10.0f), NumXY<float>(800.0f, 600.0f), SA_CENTER, result) {

	BRR(*result);

	// ������������� ����������
	this->console = console;

}

// ��������� ����
bool Menu::Draw() {

	// ��������� ������� ����
	BR(backgrounds[menuState]->Draw());

	// ��������� ������
	for (auto iter = buttons.begin(); iter != buttons.end(); iter++) {
		if ((*iter)->visible && !(*iter)->transarent) BR((*iter)->Draw());
	}

	return true;

}

// �����
bool Menu::Reset() {

	// ����� ��������
	Sprite* parent = this;
	BR(parent->Reset());

	// ����� ������
	for (auto iter = buttons.begin(); iter != buttons.end(); iter++) {
		BR((*iter)->Reset());
	}

	// ����� ������ �����
	for (auto iter = backgrounds.begin(); iter != backgrounds.end(); iter++) {
		BR((*iter)->Reset());
	}

	return true;

}

// ����������
Menu::~Menu() {

	// �������� ������
	for (UINT i(0); i < buttons.size(); i++) {
		ReleaseNULL(buttons[i]);
	}

	// �������� ������ �����
	for (UINT i(0); i < backgrounds.size(); i++) {
		ReleaseNULL(backgrounds[i]);
	}

};