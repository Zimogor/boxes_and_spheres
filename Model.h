#ifndef MODEL_H
#define MODEL_H

#include "Utilities.h"
#include "Structures.h"
#include "MathHelper.h"

class Model {

public:

	// ������
	Model(wchar_t* textureFilePath, wchar_t* binFilePath, myXMFLOAT3 position, bool* result); // �����������
	~Model(); // ����������

	// ������ �������
	bool Draw(); // ���������
	bool Draw(CXMMATRIX boneMatrix); // ��������� � ��������� � �����

	// �������
	bool SetPosition(myXMFLOAT3 newPosition) { objPosition = newPosition; return true; }; // ���������� ����� ����������
	ID3D11Buffer* GetVertexBuffer() { return pVertexBuffer; }; // ������� ����� ������
	ID3D11Buffer* GetIndexBuffer() { return pIndexBuffer; }; // ������� ����� ��������
	UINT GetIndicesAmount() { return indicesAmount; }; // ������� ���������� ��������

private:

	// �������
	bool CreateVertexBuffer(); // ������� ����� ������
	bool CreateIndexBuffer(); // ������� ����� ��������
	bool LoadModelFromFile(wchar_t* binFilePath); // ��������� ������ �� �����

protected:

	// ����������
	myXMFLOAT3 objPosition; // ���������� ������
	UINT indicesAmount = 0; // ���������� ��������
	ID3D11Buffer* pVertexBuffer = NULL; // ����� ������ �� ���������
	ID3D11Buffer* pIndexBuffer = NULL; // ����� �������� �� ���������
	ID3D11ShaderResourceView* pSRtexture = NULL; // ������ �� ��������
	bool textured = false; // ������ � ���������
	bool loaded = false; // ������ ����������� �� �����

};

#endif // MODEL_H