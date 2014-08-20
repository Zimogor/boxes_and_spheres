#ifndef ANIMMODEL_H
#define ANIMMODEL_H

#include "Utilities.h"
#include "Structures.h"
#include "Constants.h"
#include "MathHelper.h"

#define ANGLE_SPEED 5.0f // угловая скорость на поворотах
#define ATACK_ANGLE_SPEED 20.0f // скорость поворота во время атаки
#define FAST_CLIP_BLEND_SPEED 10.0f // повышенная (для удара) скорость смешения анимации
#define ATTACK_DISTANCE 1.0f // расстояние удара
#define MODEL_LIFE 15.0f // количество жизней модели

// всё, что похоже на модель с костями
class SingleAnimatedModel {

public:

	// класс
	SingleAnimatedModel() { hitPoints = maxHitPoints; }; // конструктор
	~SingleAnimatedModel() { delete[] finalMatrices; }; // деструктор

	// переменные
	myXMFLOAT3 position; // координаты объекта

	// установить новый клип
	float SetCurrentClip(UINT clipNumber); // зациклить, возвращает время клипа
	float SetCurrentClip(UINT clipNumber, UINT nextClipNumber); // сделать один цикл нового клипа, возвращает время клипа

	// функции
	bool Update(float dT); // обновление

protected:

	// функции
	XMFLOAT4X4 GetRotationMatrix() { return rotationMatrix; }; // вернуть матрицу вращения
	float GetDrawAngle() { return drawAngle; }; // вернуть угол поворота
	void SetAngleSpeed(float angleSpeed) { this->angleSpeed = angleSpeed; }; // установить скорость поворота
	void SetClipBlendSpeed(float clipBlendSpeed) { this->clipBlendSpeed = clipBlendSpeed; }; // установить скорость смешения анимации
	void SetRotAngle(float rotAngle) { this->rotAngle = rotAngle; }; // установить угол для поворота
	void InitFinalMatrices(UINT bonesAmount) { finalMatrices = new XMFLOAT4X4[bonesAmount]; }; // создать массив финальных матриц костей

	// переменные
	std::vector<UINT> curFrame; // текущий кадр для каждой из анимаций
	std::vector<float> accumulation; // накопление текущего кадра для каждой из анимаций
	std::vector<float> blendFactors; // веса для каждого из клипов
	UINT* framesAmount = NULL; // количество кадров анимации каждого клипа
	UINT clipsAmount = 0; // количество клипов анимации
	UINT nextClip = 0; // следующий клип
	UINT curClip = 0; // номер текущего клипа
	float drawAngle = DRAW_ANGLE_DEFAULT; // угол рисования объекта
	XMFLOAT4X4 rotationMatrix; // матрица вращения
	bool restCurClip = true; // состояние покоя текущего клипа (нет смешения анимации)
	bool recreateFinalMatrices = true; // необходимо обновить матрицы костей
	XMFLOAT4X4* finalMatrices = NULL; // для хранения матриц
	float damageWaitPoint = 0.0f; // время между замахом и нанесения урона
	float maxHitPoints = MODEL_LIFE; // максимальное количество жизней
	float hitPoints = 0.0f; // очки жизней

private:

	float rotAngle = 0.0f; // угол поворота
	float clipBlendSpeed = Const::clipBlendSpeed; // скорость смешения анимации
	float angleSpeed = ANGLE_SPEED; // скорость углового поворота
	bool waitNextClip = false; // одноразовый клип, ждём когда закончится
	float nextClipTime = 0.0f; // оставшееся до время одноразового клипа

};

class PerModel;

// структура клипа
struct Clip {

	~Clip() { ReleaseNULLS(bonesPositions); ReleaseNULLS(bonesQuaternions); }; // деструктор
	XMFLOAT3* bonesPositions = NULL; // массив позиций костей
	XMFLOAT4* bonesQuaternions = NULL; // массив кватернионов

};

// модель с костями
class AnimModel : public SingleAnimatedModel {

	friend PerModel;

public:

	// модель с костями
	AnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, bool* result); // конструктор
	~AnimModel(); // деструктор

	// важные функции
	bool Draw(bool toShadowMap); // рисовать модель
	myXMFLOAT3 GetPosition() { return position; }; // вернуть координаты объекта

protected:

	// создать финальные матрицы для шейдера
	void CreateFinalMatricesOldStyle(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices); // без смешивания клипов по древнему алгоритму (для отладки)
	void CreateFinalMatrices(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices); // без смешивания клипов
	void CreateFinalMatrices(UINT clipNumber1, UINT frameNumber1, UINT clipNumber2, UINT frameNumber2, float blendFactor, XMFLOAT4X4* finalMatrices); // смешивание двух клипов
	void CreateFinalMatrices(std::vector<UINT> frameNumbers, std::vector<float> blendFactors, XMFLOAT4X4* finalMatrices); // смешивание несколькоих клипов

	// переменные
	ID3D11Buffer* pVertexBuffer = NULL; // буфер вершин
	ID3D11Buffer* pIndexBuffer = NULL; // буфер индексов
	ID3D11ShaderResourceView* pSRtexture = NULL; // ссылка на текстуру
	UINT bonesAmount = 0; // количество костей
	UINT indicesAmount = 0; // количество индексов
	std::vector<Clip> clips; // вектор клипов

private:

	// функции
	bool BuildOrder(); // построить вектор order для умножния матриц
	bool LoadAmimModelFromFile(wchar_t* binFilePath); // загрузить модель из файла

	// переменные
	std::vector<int> hierarchy; // иерархия костей (родитель для каждой кости)
	std::vector<int> order; // порядок умножения матриц для нажождения final
	std::vector<XMFLOAT4X4> offsetMatrices; // матрицы offset

};

#endif // ANIMMODEL_H