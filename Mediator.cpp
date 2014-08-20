#include "Mediator.h"

#define DEPTH_PRECISION 0.1f

// указатели на элементы игры
Console* Mediator::console = NULL; // консоль
Terrain* Mediator::terrain = NULL; // территория
Hero* Mediator::hero = NULL; // герой
Errors* Mediator::errors = NULL; // обработчик ошибок
SuperFunct* Mediator::superFunct = NULL; // суперфункции
Timer* Mediator::timer = NULL; // таймер
ArtInt* Mediator::artInt = NULL; // ИИ
Helper* Mediator::helper = NULL; // помощник
Shader* Mediator::shader = NULL; // шейдер
Camera* Mediator::camera = NULL; // камера
HUD* Mediator::hud = NULL; // интерфейс
Map* Mediator::map = NULL; // карта

// дополнительные указатели и переменные
DynamicReg*** Mediator::regionField = NULL; // поле указателей динамических регионов
InventoryContent* Mediator::inventory = NULL; // инвентарь героя
MouseChange* Mediator::mouseChange = NULL; // кнопки мыши
KeyboardChange* Mediator::keyboardChange = NULL; // кнопки клавиатуры
BoundingBox* Mediator::heroBoundBox = NULL; // boundingbox героя
NumXY<int>* Mediator::cornerPoint = NULL; // относительный угол поля в координатах региона

// указатели на элементы приложения
D3D_FEATURE_LEVEL Mediator::featureLevel = D3D_FEATURE_LEVEL_9_1; // уровень DirectX
ID3D11Device* Mediator::pDev = NULL; // указатель на pDev
ID3D11DeviceContext* Mediator::pDevContext = NULL; // указатель на pDevContext
NumXY<UINT>* Mediator::winDimentions = NULL; // размерность окна

// раскладки вершин
ID3D11InputLayout* Mediator::pInputLayoutPos3Tex2 = NULL; // для спрайтов с текстурой
ID3D11InputLayout* Mediator::pInputLayoutSpriteInstance; // для иконок меню
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3 = NULL; // для моделей без текстуры
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3Tex2 = NULL; // для моделей c текстурой
ID3D11InputLayout* Mediator::pInputLayoutTerrain = NULL; // для територии
ID3D11InputLayout* Mediator::pInputLayoutPos3Nor3Tex2W1Bone2 = NULL; // для анимационных моделей
ID3D11InputLayout* Mediator::pInputLayoutInstance = NULL; // для вектора моделей с текстурой
ID3D11InputLayout* Mediator::pInputLayoutPosCol = NULL; // для карты

// вершинные шейдеры
ID3D11VertexShader* Mediator::pVSPos = NULL; // шейдер для спрайтов без текстуры
ID3D11VertexShader* Mediator::pVSPosTex = NULL; // шейдер для спрайтов с текстурой
ID3D11VertexShader* Mediator::pVSSpriteInstance = NULL; // для иконок меню
ID3D11VertexShader* Mediator::pVSPosNor = NULL; // для моделей без текстуры
ID3D11VertexShader* Mediator::pVSPosNorTex = NULL; // для моделей с текстурами
ID3D11VertexShader* Mediator::pVSTerrain = NULL; // для регионов территории
ID3D11VertexShader* Mediator::pVSPosNorTexWBone = NULL; // для моделей с костями
ID3D11VertexShader* Mediator::pVSShadow = NULL; // тень для моделей с костями
ID3D11VertexShader* Mediator::pVSInstance = NULL; // для векторов моделей
ID3D11VertexShader* Mediator::pVSShadowInstance = NULL; // для теней векторов моделей
ID3D11VertexShader* Mediator::pVSPosCol = NULL; // для карты

// пиксельные шейдеры
ID3D11PixelShader* Mediator::pPSPos = NULL; // для спрайтов без текстуры
ID3D11PixelShader* Mediator::pPSPosTex = NULL; // для спрайтов с текстурой
ID3D11PixelShader* Mediator::pPSFont = NULL; // для шрифтов
ID3D11PixelShader* Mediator::pPSSolid; // для сплошных моделей
ID3D11PixelShader* Mediator::pPSPosNor = NULL; // для моделей без текстуры
ID3D11PixelShader* Mediator::pPSPosNorTex = NULL; // для моделей с текстурой
ID3D11PixelShader* Mediator::pPSClipping = NULL; // для моделей с текстурой и обрезанием
ID3D11PixelShader* Mediator::pPSTerrain = NULL; // для регионов территории
ID3D11PixelShader* Mediator::pPSTerrainNoShadow = NULL; // для регионов территории (без тени)
ID3D11PixelShader* Mediator::pPSEmpty = NULL; // пустой теневой
ID3D11PixelShader* Mediator::pPSPosCol = NULL; // для карты

// шейдерные константные буферы
ID3D11Buffer* Mediator::cbufAnimModel = NULL; // для анимационных моделей
ID3D11Buffer* Mediator::cbufReset = NULL; // для сброса
ID3D11Buffer* Mediator::cbufObject = NULL; // для объектов
ID3D11Buffer* Mediator::cbufFont = NULL; // для шрифтов
ID3D11Buffer* Mediator::cbufFrame = NULL; // для каждого кадра

// извлечь координаты на поверхнсти земли под курсором
XMFLOAT3 Mediator::GetTerrainUnderCursor() {

	// создание матриц
	XMMATRIX P = camera->GetProjMatrix();
	XMMATRIX V = camera->GetViewMatrix();

	// создать луч в пространстве камеры
	float vx = (2.0f * mouseChange->cursorPosition.x / winDimentions->x - 1.0f) / P(0, 0);
	float vy = (-2.0f * mouseChange->cursorPosition.y / winDimentions->y + 1.0f) / P(1, 1);
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // начало луча
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f); // направление луча

	// преобразовать луч в мировое пространство
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
	rayOrigin = XMVector3TransformCoord(rayOrigin, invView);
	rayDir = XMVector3Normalize(XMVector3TransformNormal(rayDir, invView));

	// упирание в землю наращиванием луча
	XMVECTOR rayPoint = rayOrigin + rayDir * camera->GetRadius();
	XMFLOAT3 rayPointF;
	XMStoreFloat3(&rayPointF, rayPoint);
	float depth = superFunct->HeightFunction(rayPointF.x, rayPointF.z) - rayPointF.y;
	while (abs(depth) >= DEPTH_PRECISION) {
		rayPoint -= rayDir * depth;
		XMStoreFloat3(&rayPointF, rayPoint);
		depth = superFunct->HeightFunction(rayPointF.x, rayPointF.z) - rayPointF.y;
	}

	return rayPointF;

}

// ----------------------------------
// фукнции наблюдателя
// ----------------------------------

// новое количество жизней в процентах
bool Mediator::SendLifeChange(float amount) {
	return hud->SetRedLevel(amount);
}

// удар героя в точку
bool Mediator::SendHeroEventDamage(myXMFLOAT3 position, float damageAmount) {
	return artInt->TakeDamage(position, damageAmount);
}

// удар моба в точку
bool Mediator::SendMobEventDamage(myXMFLOAT3 position, float damageAmount) {
	return hero->TakeDamage(position, damageAmount); // возможный урон герою
}
bool Mediator::SendMobEventDamage(float damageAmount) {
	return hero->TakeDamage(damageAmount); // урон герою
}

// работа в точку
bool Mediator::SendEventWorking(WIELD_EVENTS events, myXMFLOAT3 position) {
	return terrain->TakePick(events, position);
}

// подбирание (true - если подобрал)
bool Mediator::SendPickup(myXMFLOAT3 position, CELL_ITEM* cellItemResult) {
	return terrain->TakePickup(position, cellItemResult);
}

// закрытие внутриигрового меню
bool Mediator::SendEventMenuClose() {
	hero->ChangeTool();
	return true;
}

// создание нового региона
bool Mediator::SendEventNewRegion(DynamicReg* dynamicReg) {
	return map->NewRegion(dynamicReg);
}