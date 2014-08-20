#include "Helper.h"
#include "Mediator.h"

// конструктор
Helper::Helper(bool *result) {

	// создать знак
	BRRC2(box = new SimpleVectorModel(result));

	// создать шрифт
	BRRC2(font = new Font(XMFLOAT3(0.0f, 1.0f, 0.0f), FT_CONSOLE, Const::helpFontDepth, result));

	// создать цилиндр
	BRR(CreateCylinder());

	// параметры рисования бокса
	pVertexBoxBuffer = box->GetVertexBuffer();
	pIndexBoxBuffer = box->GetIndexBuffer();
	boxIndicesAmount = box->GetIndicesAmount();

	Mediator::helper = this;
	return;

}

// обновление
bool Helper::Update(float dT) {

	// обновление текста
	for (UINT i(0); i < sentenceParams.size(); i++) {

		// переменные
		sentenceParams[i].position.y += dT * FLOAT_TEXT_VELOCITY;
		XMFLOAT2 pointF2;
		XMVECTOR point = XMVectorSet(sentenceParams[i].position.x, sentenceParams[i].position.y, sentenceParams[i].position.z, 1.0f);
		sentenceParams[i].lifeTime -= dT;
		if (sentenceParams[i].lifeTime <= 0.0f) {
			// удалить предложение
			sentenceParams.erase(sentenceParams.begin());
			BR(font->Delete(i));
			break;
		}

		// проекция на экран
		point = XMVector3Project(point, 0.0f, 0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, 1.0f, Mediator::camera->GetProjMatrix(), Mediator::camera->GetViewMatrix(), XMMatrixIdentity());
		XMStoreFloat2(&pointF2, point);

		// обновление предложения
		BR(font->SetPosition(i, pointF2));
	}

	return true;

}

// рисование
bool Helper::Draw() {

	// рисование текста
	BR(font->Draw());

	// рисование кубиков
	BR(box->Draw());

	return true;

}

// установить плавающий текст
bool Helper::SetFloatText(myXMFLOAT3 position, const wchar_t* text, bool specialColorB, XMFLOAT3 specialColorF) {

	// переменные
	XMFLOAT2 pointF2; XMFLOAT3 pointF3;
	XMVECTOR point = XMVectorSet(position.x, position.y, position.z, 1.0f);
	XMStoreFloat3(&pointF3, point);

	// проекция на экран
	point = XMVector3Project(point, 0.0f, 0.0f, (float)Mediator::winDimentions->x, (float)Mediator::winDimentions->y, 0.0f, 1.0f, Mediator::camera->GetProjMatrix(), Mediator::camera->GetViewMatrix(), XMMatrixIdentity());
	XMStoreFloat2(&pointF2, point);

	// создание предложения
	BR(font->TextOutXY(text, pointF2, SA_TOPLEFT, true, specialColorB, specialColorF));
	sentenceParams.push_back(PerFloatText(pointF3));

	return true;

}

// создать кубик
bool Helper::SetBox(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	BR(box->CreateInstance(position, scale, rotation));

	return true;

}

// создать кубик подёнок
bool Helper::SetFlyBox(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	BR(box->ShowFlyInstance(position, scale, rotation));

	return true;

}

// создать цилиндр
bool Helper::CreateCylinder() {

	const UINT capVertAmount = 8; // количество вершин одного круга
	const float cylSize = 0.3f;
	UINT vertAmount = capVertAmount * 2 + 2; // полное количество вершин
	VertPos3Nor3* total = new VertPos3Nor3[vertAmount];

	// верхняя крышка
	total[0] = { XMFLOAT3(0.0f, cylSize, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) };
	float angle = 0.0f;
	float dAngle = XM_2PI / capVertAmount;
	for (UINT i(0); i < capVertAmount; i++) {
		total[i + 1] = { XMFLOAT3(cos(angle), cylSize, sin(angle)), XMFLOAT3(1.0f, 1.0f, 1.0f) };
		angle += dAngle;
	}

	// нижняя крышка
	total[capVertAmount + 1] = { XMFLOAT3(0.0f, -cylSize, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) };
	angle = 0.0f;
	for (UINT i(0); i < capVertAmount; i++) {
		total[i + capVertAmount + 2] = { XMFLOAT3(cos(angle), -cylSize, sin(angle)), XMFLOAT3(1.0f, 1.0f, 1.0f) };
		angle += dAngle;
	}

	UINT capIndAmount = capVertAmount * 3;
	UINT sideSliceAmount = capVertAmount;
	cylinderIndicesAmount = capIndAmount * 2 + sideSliceAmount * 6;
	UINT* totalInd = new UINT[cylinderIndicesAmount];
	UINT bottomCenterIndex = capVertAmount + 1;

	// верхняя крышка
	UINT indNumber = 0;
	for (UINT i(0); i < capVertAmount; i ++) {
		totalInd[indNumber] = 0;
		totalInd[indNumber + 2] = 1 + i;
		totalInd[indNumber + 1] = (i == capVertAmount - 1) ? 1 : 2 + i;
		indNumber += 3;
	}

	// нижняя крышка
	for (UINT i(0); i < capVertAmount; i++) {
		totalInd[indNumber] = bottomCenterIndex;
		totalInd[indNumber + 1] = bottomCenterIndex + 1 + i;
		totalInd[indNumber + 2] = (i == capVertAmount - 1) ? bottomCenterIndex + 1 : bottomCenterIndex + 2 + i;
		indNumber += 3;
	}

	// боковушки
	UINT simDistance = capVertAmount + 1;
	for (UINT i(1); i < sideSliceAmount + 1; i++) {
		totalInd[indNumber] = i;
		totalInd[indNumber + 1] = (i == sideSliceAmount) ? 1 : 1 + i;
		totalInd[indNumber + 2] = simDistance + i;
		totalInd[indNumber + 3] = (i == sideSliceAmount) ? 1 : 1 + i;
		totalInd[indNumber + 4] = (i == sideSliceAmount) ? 1 + simDistance : simDistance + 1 + i;
		totalInd[indNumber + 5] = simDistance + i;
		indNumber += 6;
	}

	// структура буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(VertPos3Nor3) * vertAmount;
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфера вершин
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = total;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexCylinderBuffer));

	// структура буфера индексов
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT)* cylinderIndicesAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфер индексов
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = totalInd;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexCylinderBuffer));

	// удалить остатки
	delete[] total;
	delete[] totalInd;

	return true;

}

// деструктор
Helper::~Helper() {

	ReleaseNULL(box);
	ReleaseNULL(font);
	ReleaseCOM(pVertexCylinderBuffer);
	ReleaseCOM(pIndexCylinderBuffer);

};