#ifndef MODEL_H
#define MODEL_H

#include "Utilities.h"
#include "Structures.h"
#include "MathHelper.h"

class Model {

public:

	// объект
	Model(wchar_t* textureFilePath, wchar_t* binFilePath, myXMFLOAT3 position, bool* result); // конструктор
	~Model(); // деструктор

	// важные функции
	bool Draw(); // рисование
	bool Draw(CXMMATRIX boneMatrix); // рисование с привязкой к кости

	// функции
	bool SetPosition(myXMFLOAT3 newPosition) { objPosition = newPosition; return true; }; // установить новые координаты
	ID3D11Buffer* GetVertexBuffer() { return pVertexBuffer; }; // вернуть буфер вершин
	ID3D11Buffer* GetIndexBuffer() { return pIndexBuffer; }; // вернуть буфер индексов
	UINT GetIndicesAmount() { return indicesAmount; }; // вернуть количество индексов

private:

	// функции
	bool CreateVertexBuffer(); // создать буфер вершин
	bool CreateIndexBuffer(); // создать буфер индексов
	bool LoadModelFromFile(wchar_t* binFilePath); // загрузить объект из файла

protected:

	// переменные
	myXMFLOAT3 objPosition; // координаты модели
	UINT indicesAmount = 0; // количество индексов
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин по умолчанию
	ID3D11Buffer* pIndexBuffer = NULL; // буфер индексов по умолчанию
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	bool textured = false; // объект с текстурой
	bool loaded = false; // объект загружается из файла

};

#endif // MODEL_H