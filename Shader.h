#ifndef SHADER_H
#define SHADER_H

#include "Utilities.h"
#include "Nums.h"
#include "Constants.h"

class Shader {

public:

	// шейдер
	Shader(bool* result); // конструктор
	~Shader(); // деструктор

	// важные функции
	bool Update(); // обновить

	// функции
	bool SetAlphaBlending(bool enable); // установить/снять alpha blending
	bool SetObjectBuffer(XMFLOAT4X4* worldMatrix); // установить мировую матрицу
	bool SetObjectBuffer(CXMMATRIX worldMatrix); // установить мировую матрицу
	bool SetFontBuffer(XMFLOAT4* fontColor); // установить цвет шрифта
	bool SetResetBuffer(CXMMATRIX orthoMatrix); // установить параметры сброса
	bool SetCamBuffer(CXMMATRIX viewProjMatrix, XMFLOAT4 camPos, XMFLOAT4 camFocus); // установить параметры камеры
	bool SetWireframe(bool wireframe); // установить/снять wireframe

	// извлечение параметров
	XMMATRIX GetViewLightMatrix(); // матрица источника света
	XMMATRIX GetProjLightMatrix(); // матрица проекции света
	ID3D11DepthStencilView* GetShadowMapView() { return pShadowMapDepthView; }; // карта теней
	ID3D11ShaderResourceView* GetShadowResourceView() { return pShadowResourceView; }; // ресурс для карты теней

private:

	// переменные
	ID3D11BlendState* pBlendState = NULL; // blendState
	ID3D10Blob* VSBlob = NULL; // для создание вершинных шейдеров
	ID3D10Blob* PSBlob = NULL; // для создания пиксельных шейдеров
	ID3D11RasterizerState* pRastStandard = NULL; // сплошная стандартная визуализация
	ID3D11RasterizerState* pRastWireframe = NULL; // визуализация сеткой
	ID3D11DepthStencilView* pShadowMapDepthView = NULL; // карта теней
	ID3D11ShaderResourceView* pShadowResourceView = NULL; // ресурс для карты теней

	// функции
	bool CreateShadowMap(); // создание карты теней
	bool CreateVertexShadersAndLayouts(); // создать вершинные шейдеры и расклаки вершин
	bool CreatePixelShaders(); // создать пиксельные шейдеры
	bool CreateConstBuffers(); // создать константные буферы
	bool CreateStates(); // создать blend state, sampler state и render state

	// раскладки вершин
	ID3D11InputLayout* pInputLayoutPos3Tex2 = NULL; // для спрайтов с текстурой
	ID3D11InputLayout* pInputLayoutSpriteInstance = NULL; // для иконок меню
	ID3D11InputLayout* pInputLayoutPos3Nor3 = NULL; // для моделей без текстуры
	ID3D11InputLayout* pInputLayoutPos3Nor3Tex2 = NULL; // для моделей c текстурой
	ID3D11InputLayout* pInputLayoutTerrain = NULL; // для территории
	ID3D11InputLayout* pInputLayoutPos3Nor3Tex2W1Bone2 = NULL; // для анимационных моделей
	ID3D11InputLayout* pInputLayoutInstance = NULL; // для вектора моделей с текстурой
	ID3D11InputLayout* pInputLayoutPosCol = NULL; // для карты

	// вершинные шейдеры
	ID3D11VertexShader* pVSPos = NULL; // для спрайтов без текстуры
	ID3D11VertexShader* pVSPosTex = NULL; // для спрайтов с текстурой
	ID3D11VertexShader* pVSSpriteInstance = NULL; // для иконок меню
	ID3D11VertexShader* pVSPosNor = NULL; // для моделей без текстуры
	ID3D11VertexShader* pVSPosNorTex = NULL; // для моделей с текстурой
	ID3D11VertexShader* pVSTerrain = NULL; // для регионов территории
	ID3D11VertexShader* pVSPosNorTexWBone = NULL; // для анимационных моделей с костями
	ID3D11VertexShader* pVSShadow = NULL; // тени для анимационных моделей
	ID3D11VertexShader* pVSInstance = NULL; // для векторов моделей
	ID3D11VertexShader* pVSShadowInstance = NULL; // для теней векторов моделей
	ID3D11VertexShader* pVSPosCol = NULL; // для карты

	// пиксельные шейдеры
	ID3D11PixelShader* pPSPos = NULL; // для спрайтов без текстуры
	ID3D11PixelShader* pPSPosTex = NULL; // для спрайтов с текстурой
	ID3D11PixelShader* pPSFont = NULL; // для шрифтов
	ID3D11PixelShader* pPSSolidModel = NULL; // для сплошных моделей
	ID3D11PixelShader* pPSPosNor = NULL; // для моделей без текстуры
	ID3D11PixelShader* pPSPosNorTex = NULL; // для моделей с текстурой
	ID3D11PixelShader* pPSClipping = NULL; // для моделей с текстурой и обрезанием
	ID3D11PixelShader* pPSTerrain = NULL; // для регионов территории
	ID3D11PixelShader* pPSTerrainNoShadow = NULL; // для регионов территории (без тени)
	ID3D11PixelShader* pPSEmpty = NULL; // теневой пустой
	ID3D11PixelShader* pPSPosCol = NULL; // для карты

	// шейдерные константные буферы
	ID3D11Buffer* cbufLightVector = NULL; // единственная инициализация
	ID3D11Buffer* cbufAnimModel = NULL; // для анимационных моделей
	ID3D11Buffer* cbufReset = NULL; // для сброса
	ID3D11Buffer* cbufObject = NULL; // для объектов
	ID3D11Buffer* cbufFont = NULL; // для шрифтов
	ID3D11Buffer* cbufFrame = NULL; // для каждого кадра
	ID3D11Buffer* cbufLight = NULL; // для карты теней
	ID3D11Buffer* cbufOnce = NULL; // вечные константы

};

#endif // SHADER_H