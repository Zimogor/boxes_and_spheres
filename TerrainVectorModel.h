#ifndef VECTOR_MODEL
#define VECTOR_MODEL

//--------------------------------------------------------------------
// �����, ������� ��������� ������ ������������ �� ���������� ��������
// (�������, ����� � ��).
//--------------------------------------------------------------------

#include "Structures.h"
#include "Model.h"

// ��������� ������ ��� ����������
struct PerModelInstance {

	// ������������
	PerModelInstance(myXMFLOAT3 position, float rotation, XMFLOAT2 scale) : position(position), rotation(rotation), scale(scale) {};
	PerModelInstance() {};

	// ���������
	myXMFLOAT3 position;
	float rotation; // ���� ��������
	XMFLOAT2 scale; // ������� �� ����������� � ���������

};

// ��������� ����������� �������
struct ElementDesc {

	ElementDesc(NumXY<int> regionAdress, UINT elementsAmount) : regionAdress(regionAdress), elementsAmount(elementsAmount) {};
	NumXY<int> regionAdress; // ����� �������
	UINT elementsAmount; // ���������� ��������� � �������

};

// ����� ������� �������
class TerrainVectorModel : public Model {

public:

	// ������ �������
	TerrainVectorModel(wchar_t* textureFilePath, wchar_t* binFilePath, bool* result); // �����������
	~TerrainVectorModel() {}; // ����������

	// ������ �������
	bool Draw(bool toShadow, ID3D11Buffer* pInst, UINT start, UINT amount); // ���������

	// �������
	bool Spawn(myXMFLOAT3* position); // ������ ������� (�������� ���������� �����)
	bool AddDescription(NumXY<int> regionAdress, UINT elementsAmount); // �������� ����� ��������
	bool EraseByRegion(NumXY<int> regionAdress); // ������� ��� �������� � �������
	bool RemoveElementFromVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance* modelParams = NULL); // ������� ������� �� ������, true - ���� ��������
	bool AddElementToVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance modelParams); // �������� ������� � ������
	UINT GetElementsAmount() { return elements.size(); }; // �������� ���������� ���������
	myXMFLOAT3 GetPosition(UINT elementNumber) { return elements[elementNumber].position; }; // �������� ���������� ��������
	PerModelInstance* GetPointer(UINT elementNumber) { return &elements[elementNumber]; }; // �������� ��������� �� �������

private:

	// ����������
	std::vector<PerModelInstance> elements; // ������ ���������
	std::vector<ElementDesc> elementsDesc; // �������� ��������� (���������� � ���, ��� �������� ��������)

	// �������
	bool CreateInputLayout(); // �������� ��������� ������

};

#endif // VECTOR_MODEL