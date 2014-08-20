#ifndef SPRITE_H
#define SPRITE_H

#include "Utilities.h"
#include "Structures.h"

class Sprite {

public:

	// ������
	Sprite(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, SPRITE_ALIGN spriteAlight, bool* result); // �����������
	~Sprite(); // ����������

	// ������ �������
	bool Draw(); // ��������� �������
	bool Reset(); // ����� �������

	// �������
	XMFLOAT3 GetSpriteRelPosition() { return position; }; // �������� ������������� ���������� �������

private:

	// ����������

	// ���������
	const SPRITE_ALIGN spriteAlign; // ������������ �������

protected:

	// �����������
	Sprite(XMFLOAT3 position, NumXY<float> dimentions, SPRITE_ALIGN spriteAlign) : position(position), dimentions(dimentions), spriteAlign(spriteAlign) {}; // ��� �����������

	// ����������
	bool textured = false; // ������ � ���������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	XMFLOAT4X4 translationF; // ������� �����������
	UINT vertexAmount = 4; // ���������� ��������

	// ���������
	const XMFLOAT3 position; // ���������� �������
	const NumXY<float> dimentions; // ����������� �������

	// �������
	bool CreateVertexBuffer(); // ������� ����� ������
	bool UpdateMatrixTranslation(NumXY<UINT> winDimentions); // �������� ������� �����������

};

#endif // SPRITE_H