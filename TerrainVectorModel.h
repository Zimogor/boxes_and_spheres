#ifndef VECTOR_MODEL
#define VECTOR_MODEL

//--------------------------------------------------------------------
// класс, который описывает вектор стационарных на территории объектов
// (деревь€, камни и пр).
//--------------------------------------------------------------------

#include "Structures.h"
#include "Model.h"

// структура данных дл€ экземпл€ра
struct PerModelInstance {

	// конструкторы
	PerModelInstance(myXMFLOAT3 position, float rotation, XMFLOAT2 scale) : position(position), rotation(rotation), scale(scale) {};
	PerModelInstance() {};

	// параметры
	myXMFLOAT3 position;
	float rotation; // угол поворота
	XMFLOAT2 scale; // масштаб по горизонтали и вертикали

};

// описатель содержимого вектора
struct ElementDesc {

	ElementDesc(NumXY<int> regionAdress, UINT elementsAmount) : regionAdress(regionAdress), elementsAmount(elementsAmount) {};
	NumXY<int> regionAdress; // адрес региона
	UINT elementsAmount; // количество элементов в регионе

};

// класс вектора моделей
class TerrainVectorModel : public Model {

public:

	// вектор моделей
	TerrainVectorModel(wchar_t* textureFilePath, wchar_t* binFilePath, bool* result); // конструктор
	~TerrainVectorModel() {}; // деструктор

	// важные функции
	bool Draw(bool toShadow, ID3D11Buffer* pInst, UINT start, UINT amount); // рисование

	// функции
	bool Spawn(myXMFLOAT3* position); // родить элемент (измен€ет координаты ввода)
	bool AddDescription(NumXY<int> regionAdress, UINT elementsAmount); // добавить кусок описани€
	bool EraseByRegion(NumXY<int> regionAdress); // удалить все элементы в регионе
	bool RemoveElementFromVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance* modelParams = NULL); // удалить элемент из €чейки, true - если удалилс€
	bool AddElementToVector(NumXY<int> regionAdress, NumXY<int> cellAdress, PerModelInstance modelParams); // добавить элемент в €чейку
	UINT GetElementsAmount() { return elements.size(); }; // получить количество элементов
	myXMFLOAT3 GetPosition(UINT elementNumber) { return elements[elementNumber].position; }; // получить координаты элемента
	PerModelInstance* GetPointer(UINT elementNumber) { return &elements[elementNumber]; }; // получить указатель на элемент

private:

	// переменные
	std::vector<PerModelInstance> elements; // вектор элементов
	std::vector<ElementDesc> elementsDesc; // описание элементов (информаци€ о том, как хран€тс€ элементы)

	// функции
	bool CreateInputLayout(); // создание раскладки вершин

};

#endif // VECTOR_MODEL