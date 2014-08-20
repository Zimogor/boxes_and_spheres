#include "Console.h"
#include "Mediator.h"

// ����������� �������
Console::Console(bool* result) : Sprite(NULL, consolePosition, NumXY<float>(CONSOLE_WIDTH, CONSOLE_HEIGHT), SA_TOPRIGHT, result) {

	// ������������� ����������
	BRR(*result);
	BRRC2(font = new Font(XMFLOAT3(0.0f, 0.0f, 0.0f), FT_CONSOLE, 0.1f, result));
	BRR(font->TextOutXY(L"FPS: dT: ", fontStartPosition, SA_TOPRIGHT, false));
	fontHeight = font->GetFontHeight();
	maxStringNumber = (UINT)(CONSOLE_HEIGHT / fontHeight);

	// ��������(��������) ����� �������
	debugFile.open("debug.txt", std::ios_base::binary);
	BRR(debugFile.is_open());

	Mediator::console = this;
	*result = true;

}

// ����� ���������
bool Console::Push(const wchar_t* message) {

	// ����� ������
	BR(font->TextOutXY(message, XMFLOAT2(fontStartPosition.x, fontStartPosition.y + curStringNumber * fontHeight), SA_TOPRIGHT, false));

	// �������� ������� �����
	if (curStringNumber < maxStringNumber) curStringNumber++;
	else {
		// �������� �����
		BR(font->MoveSentences(1, XMFLOAT2(0.0f, -fontHeight), true));
	}

	// ������ � ���� �������
	if (Const::writeToDebugFile) {
		debugFile.write(message, wcslen(message));
		debugFile.write(L"\r\n", 2);
	}
	return true;

}

// ���������� ������ � ���������
bool Console::Push(wchar_t* message, CELL_ITEM cellItem) {

	std::wostringstream out;
	out << message << ' ';

	switch (cellItem) {
	case CI_TREE: out << "CI_TREE"; break;
	case CI_BRUSH: out << "CI_BRUSH"; break;
	case CI_FERN: out << "CI_FERN"; break;
	case CI_FIR: out << "CI_FIR"; break;
	case CI_MUSHROOM: out << "CI_MUSHROOM"; break;
	case CI_NONE: out << "CI_NONE"; break;
	default: out << "undefined"; break;
	}

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

// ���������� �������
bool Console::Update() {

	if (Mediator::keyboardChange->keysShort[KS_TAB]) {
		show = !show;
	}

	return true;

}

// ��������� �������
bool Console::Draw() {

	if (show) {

		// ��������� �������
		Sprite* parent = this;
		BR(parent->Draw());

		// ��������� ������
		BR(font->Draw());

	}

	return true;

}

// �������� �������� ������
bool Console::UpdateMainData(UINT FPS, float dT) {

	std::wostringstream out;
	out << "FPS: " << FPS << "  dT: " << dT;
	BR(font->ReplaceSentence(0, out.str().c_str()));

	return true;

}

// ����� �������
bool Console::Reset() {

	// ����� �������
	Sprite* parent = this;
	BR(parent->Reset());

	// ����� ������
	BR(font->Reset());

	return true;

}

// ����������
Console::~Console() {

	debugFile.close();
	ReleaseNULL(font);

}