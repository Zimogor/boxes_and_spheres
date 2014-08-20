#include "HUD.h"
#include "Mediator.h"

#define RED_VERTEX_BUFFER(x)\
		{ XMFLOAT3(82.0f, 50.0f, position.z), XMFLOAT2{ 0.0f, 0.54f - x + 0.5f} },\
		{ XMFLOAT3(98.0f, 78.0f, position.z), XMFLOAT2{ 0.0f, 0.32f - x + 0.5f } },\
		{ XMFLOAT3(29.0f, 50.0f, position.z), XMFLOAT2{ 0.0f, 0.54f - x + 0.5f } },\
		{ XMFLOAT3(101.0f, 118.0f, position.z), XMFLOAT2{ 0.0f, 0.0f - x + 0.5f } },\
		{ XMFLOAT3(33.0f, 104.0f, position.z), XMFLOAT2{ 0.0f, 0.12f - x + 0.5f } },\
		{ XMFLOAT3(60.0f, 119.0f, position.z), XMFLOAT2{ 0.0f, 0.0f - x + 0.5f } },

// конструктор красной жидкости
RedLiquid::RedLiquid(bool* result) : Sprite(XMFLOAT3(-227.0f, -137.0f, 1.5f), NumXY<float>(), SA_BOTTOMRIGHT) {

	HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, L"resources//HUD//red.png", NULL, NULL, &pSRtexture, NULL));

	textured = true;

	// создать буфер вершин
	BRR(CreateVertexBuffer());

	// создание матрицы перемещения
	BRR(UpdateMatrixTranslation(*Mediator::winDimentions));

	*result = true;

}

// установить уровень жидкости в процентах
bool RedLiquid::SetLevel(float amount) {

	amount *= 0.5f;

	// сместить текстурные координаты
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	VertPos3Tex2* VB = reinterpret_cast<VertPos3Tex2*>(mappedData.pData);
	VertPos3Tex2 vertexBufferTex[] = {
		RED_VERTEX_BUFFER(amount)
	};
	memcpy(VB, vertexBufferTex, sizeof(vertexBufferTex));
	Mediator::pDevContext->Unmap(pVertexBuffer, 0);

	return true;

}

// создание буфера вершин
bool RedLiquid::CreateVertexBuffer() {

	vertexAmount = 6;

	// спрайт с текстурой
	VertPos3Tex2 vertexBufferTex[] = {
		RED_VERTEX_BUFFER(1.0f)
	};

	// структура буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(vertexBufferTex);
	bufferVertDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_DYNAMIC;

	// создание буфера вершин
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = (void*)vertexBufferTex;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	return true;

}

// конструктор интерфейса
HUD::HUD(bool* result) : Sprite(L"resources//HUD//HUD.dds", XMFLOAT3(-100.0f, -10.0f, 1.0f), NumXY<float> (128.0f, 128.0f), SA_BOTTOMRIGHT, result) {

	BRRC2(redLiquid = new RedLiquid(result)); // создание красной жидкости
	Mediator::hud = this;
	*result = true;

}

// установить новый уровень красной воды в процентах
bool HUD::SetRedLevel(float amount) {

	return redLiquid->SetLevel(amount);

}

// рисование интерфейса
bool HUD::Draw() {

	BR(Sprite::Draw()); // задний фон
	BR(redLiquid->Draw()); // красная жидкость

	return true;

}

// сброс интерфейса
bool HUD::Reset() {

	BR(Sprite::Reset());
	BR(redLiquid->Reset());

	return true;

}

// деструктор
HUD::~HUD() {

	ReleaseNULL(redLiquid);

}