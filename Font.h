#ifndef FONT_H
#define FONT_H

#include "Utilities.h"
#include "Structures.h"
#include "MathHelper.h"

// ���������
#define LETTER_VERTEX_SIZE 4 // ���������� ������ �� �����
#define LETTER_INDEX_SIZE 6 // ���������� �������� �� �����
#define UNICODE_ENG_DISPLACEMENT 32 // �������� �������� � Unicode
#define UNICODE_RUS_DISPLACEMENT 941 // �������� ������� �������� � Unicode
#define MAX_STRING_LENGTH 128 // ������������ ����� �����
#define UNDEFINED_SYMBOL 94 // ������������� ������

// ����� �������� ��� �������� �����������
struct FontConstants {

	// ������������
	FontConstants() : height(0), symbolsAmount(0) {};
	FontConstants(float height, UINT symbolsAmount, XMFLOAT4 color) : height(height), symbolsAmount(symbolsAmount), color(color) {};

	float height; // ������ ������
	UINT symbolsAmount; // ���������� ��������
	XMFLOAT4 color; // ���� ������
	float zDepth; // ������� ������

};

// ��������� ���������
struct Descriptor {

	float left; // ����� ����
	float right; // ����� ����
	float top; // ������� ����
	float bottom; // ������ ����
	unsigned char width; // ������
	unsigned char height; // ������
	char xOffset; // �������� �� X
	char yOffset; // �������� �� Y

};

// ����� �����������
class Sentence {

public:

	// �����������
	Sentence(const wchar_t* message, XMFLOAT2 position, Descriptor* descriptor, FontConstants fontConstants, SPRITE_ALIGN fontAlign, bool center, bool specialColorB, XMFLOAT3 specialColorF3, bool* result); // �����������
	~Sentence(); // ����������

	// ������ �������
	bool Draw(); // ���������
	bool Move(XMFLOAT2 changePosition); // �������� �����������
	bool Reset(); // �����

	// �������
	SPRITE_ALIGN getAlignment() { return fontAlign; }; // ������� ������������ �����������
	XMFLOAT2 getPosition() { return position; }; // ������� ���������� �����������
	bool SetPosition(XMFLOAT3 newPosition); // �������� ����������
	bool SetPosition(XMFLOAT2 newPosition); // �������� ����������
	bool specialColorB = false; // ������������ ������ ����

private:

	// �������
	bool CreateVertextBuffer(const wchar_t* message, UINT messageLength, Descriptor* descriptor, FontConstants fontConstants); // ������� ����� ������
	bool CreateIndexBuffer(UINT messageLength); // ������� ����� ��������
	bool RebuildMatrixTranslation(); // �������� ������� ��������������

	// ����������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������
	ID3D11Buffer* pIndexBuffer = NULL; // ����� ��������
	UINT indicesAmount = 0; // ���������� ��������
	XMFLOAT4X4 translationF; // ������� ��������
	SPRITE_ALIGN fontAlign; // ������������ �����������
	XMFLOAT2 position; // ���������� ����������� (������� ����� ����)
	NumXY<int> dimentions = NumXY<int>(); // ����������� �����������
	bool center; // ���������� ������ ������������ ������
	XMFLOAT3 specialColorF; // ������ ����

};

// ����� ������
class Font {

public:

	// �����
	Font(XMFLOAT3 color, FONT_TYPE fontType, float zDehtp, bool* result); // �����������
	~Font(); // ����������

	// ������ �������
	bool TextOutXY(const wchar_t* text, XMFLOAT2 position, SPRITE_ALIGN fontAlign, bool center, bool specialColorB = false, XMFLOAT3 specialColorF = XMFLOAT3()); // �������� ����� ������ (�� ����������� �������� ������ ���� ������ � �����������)
	bool Draw(); // ���������
	bool Draw(ButtonsDesc* buttonsDesc); // ��������� � ���������� ������
	bool Reset(); // �����

	// ������� ����������
	float GetFontHeight() { return fontConstants.height; }; // �������� ������ ������
	UINT GetSentenceAmount() { return sentences.size(); }; // ���������� �����������

	// ��������������� �������
	bool MoveSentences(UINT start, XMFLOAT2 translation, bool delStart); // �������� ��� ����������� ������� �� start
	bool ReplaceSentence(UINT number, const wchar_t* newText); // �������� �����������
	bool DeleteAll(); // ������� ��� �����������
	bool Delete(UINT number); // ������� �����������
	bool SetPosition(UINT sentenceNumber, XMFLOAT3 newPosition); // �������� ���������� ������ �����������
	bool SetPosition(UINT sentenceNumber, XMFLOAT2 newPosition); // �������� ���������� ������ �����������

private:

	// �������
	bool CreateConsoleDescriptor(); // ������� ���������� ���������
	bool CreateSlavicDescriptor(); // ������� ��������-���������� ���������

	// ����������
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	Descriptor* descriptor = NULL; // ��������� ������

	// �����������
	std::vector<Sentence*> sentences;
	FontConstants fontConstants;

};

#endif // FONT_H