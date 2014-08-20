#include "Font.h"
#include "Mediator.h"

// конструкор предложения
Sentence::Sentence(const wchar_t* message, XMFLOAT2 position, Descriptor* descriptor, FontConstants fontConstants, SPRITE_ALIGN fontAlign, bool center, bool specialColorB, XMFLOAT3 specialColorF, bool* result) {

	// инициализация переменных
	UINT wordLength = wcsnlen_s(message, MAX_STRING_LENGTH);
	this->fontAlign = fontAlign;
	this->position = position;
	this->center = center;
	this->specialColorB = specialColorB;
	this->specialColorF = specialColorF;

	// создать буфер вершин
	BRR(CreateVertextBuffer(message, wordLength, descriptor, fontConstants));

	// построить матрицу преобразования
	BRR(RebuildMatrixTranslation());

	// создать буфер индексов
	BRR(CreateIndexBuffer(wordLength));

	*result = true;

}

// обновить матрицу преобразования
bool Sentence::RebuildMatrixTranslation() {

	// смещение координат
	NumXY<int> displacement = center ? NumXY<int>(dimentions.x / 2, dimentions.y / 2) : NumXY<int>(0, 0);

	// определение матрицы перемещения
	switch (fontAlign) {
	case SA_TOPRIGHT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(Mediator::winDimentions->x + position.x - displacement.x, position.y - displacement.y, 0.0f));
		break;
	}
	case SA_BOTTOMLEFT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(position.x - displacement.x, Mediator::winDimentions->y + position.y - displacement.y, 0.0f));
		break;
	}
	case SA_BOTTOMRIGHT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(Mediator::winDimentions->x + position.x - displacement.x, Mediator::winDimentions->y + position.y - displacement.y, 0.0f));
		break;
	}
	case SA_CENTER: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(Mediator::winDimentions->x / 2 + position.x - displacement.x, Mediator::winDimentions->y / 2 + position.y - displacement.y, 0.0f));
		break;
	}
	default: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(position.x - displacement.x, position.y - displacement.y, 0.0f));
		break;
	}
	}

	return true;

}

// создать буфер вершин
bool Sentence::CreateVertextBuffer(const wchar_t* message, UINT messageLength, Descriptor* descriptor, FontConstants fontConstants) {

	// инициализация переменных
	UINT bufferByteSize = messageLength * LETTER_VERTEX_SIZE * sizeof(VertPos3Tex2); // размер буфера вершин
	VertPos3Tex2* vertexBuffer = new VertPos3Tex2[messageLength * LETTER_VERTEX_SIZE]; // буфер для вершин
	VertPos3Tex2* pointer = vertexBuffer; // указатель на место вставки в буфер
	float leftScreenSide = 0.0f;
	float rightScreenSide = 0.0f;
	float topScreenSide = 0.0f;
	float bottomScreenSide = fontConstants.height;
	float topDisplacement = 0.0f; // если несолько строк "\n"
	float minX(0.0f), maxX(0.0f), minY(0.0f), maxY(0.0f); // предельные значения координат предложения

	// создание буфера вершин
	for (UINT i(0); i < messageLength; i++) {

		// перенос строки
		if (message[i] == '\n') {
			topDisplacement += fontConstants.height;
			rightScreenSide = 0.0f;
			continue;
		}

		// определение символа
		unsigned char letterNumber;

		// латиница
		if (message[i] >= 32 && message[i] <= 126) letterNumber = message[i] - UNICODE_ENG_DISPLACEMENT;
		else {

			// кириллица
			if (message[i] >= 0x410 && message[i] <= 0x44f) {
				letterNumber = message[i] - UNICODE_RUS_DISPLACEMENT;
			}

			else letterNumber = UNDEFINED_SYMBOL; // неопределённый символ
		}

		// координаты вершин
		float leftPicSide = descriptor[letterNumber].left;
		float rightPicSide = descriptor[letterNumber].right;
		float topPicSide = descriptor[letterNumber].top;
		float bottomPicSide = descriptor[letterNumber].bottom;
		leftScreenSide = rightScreenSide + descriptor[letterNumber].xOffset;
		rightScreenSide = leftScreenSide + descriptor[letterNumber].width;
		topScreenSide = descriptor[letterNumber].yOffset + topDisplacement;
		bottomScreenSide = topScreenSide + descriptor[letterNumber].height;

		VertPos3Tex2 curLetter[] = {

			{ XMFLOAT3(leftScreenSide, topScreenSide, fontConstants.zDepth), XMFLOAT2{ leftPicSide, topPicSide } },
			{ XMFLOAT3(rightScreenSide, topScreenSide, fontConstants.zDepth), XMFLOAT2{ rightPicSide, topPicSide } },
			{ XMFLOAT3(leftScreenSide, bottomScreenSide, fontConstants.zDepth), XMFLOAT2{ leftPicSide, bottomPicSide } },
			{ XMFLOAT3(rightScreenSide, bottomScreenSide, fontConstants.zDepth), XMFLOAT2{ rightPicSide, bottomPicSide } },

		};

		memcpy(pointer, curLetter, sizeof(VertPos3Tex2)* LETTER_VERTEX_SIZE);
		pointer += LETTER_VERTEX_SIZE;

		// для определения размерности
		minX = min(minX, leftScreenSide);
		minY = min(minY, topScreenSide);
		maxX = max(maxX, rightScreenSide);
		maxY = max(maxY, bottomScreenSide);

	}

	// структура буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = bufferByteSize;
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфера вершин
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = vertexBuffer;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	dimentions.x = (int)(maxX - minX);
	dimentions.y = (int)(maxY - minY);
	ReleaseNULLS(vertexBuffer);

	return true;

}

// создать буфер индексов
bool Sentence::CreateIndexBuffer(UINT messageLength) {

	// инициализация переменных
	UINT* indices = new UINT[messageLength * LETTER_INDEX_SIZE];
	UINT* pointer = indices;
	indicesAmount = messageLength * LETTER_INDEX_SIZE;
	UINT bufferByteSize = indicesAmount * sizeof(UINT);

	// создание буфера индексов
	for (UINT i(0); i < messageLength; i++) {

		UINT displacement = i * LETTER_VERTEX_SIZE;
		UINT curLetterArray[] = { displacement, 1 + displacement, 2 + displacement, 2 + displacement, 1 + displacement, 3 + displacement };
		memcpy(pointer, curLetterArray, sizeof(UINT)* LETTER_INDEX_SIZE);
		pointer += LETTER_INDEX_SIZE;

	}

	// структура буфера индексов
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = bufferByteSize;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфер индексов
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indices;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	ReleaseNULLS(indices);

	return true;

}

// рисование предложения
bool Sentence::Draw() {

	// особый цвет
	if (specialColorB) BR(Mediator::shader->SetFontBuffer(&XMFLOAT4(specialColorF.x, specialColorF.y, specialColorF.z, 1.0f)));

	// установка параметров
	UINT vertexStride = sizeof(VertPos3Tex2);
	UINT vertexOffset = 0;
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	BR(Mediator::shader->SetObjectBuffer(&translationF));

	// рисование
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// сброс
bool Sentence::Reset() {

	// обновить матрицу преобразования
	BR(RebuildMatrixTranslation());

	return true;

}

// сместить предложение
bool Sentence::Move(XMFLOAT2 changePosition) {

	translationF._41 += changePosition.x;
	translationF._42 += changePosition.y;

	position.x += changePosition.x;
	position.y += changePosition.y;

	return true;

}

// установить новые координаты предложения
bool Sentence::SetPosition(XMFLOAT3 newPosition) {
	
	if (center) {
		translationF._41 = newPosition.x - dimentions.x / 2;
		translationF._42 = newPosition.y - dimentions.y / 2;
	}
	else {
		translationF._41 = newPosition.x;
		translationF._42 = newPosition.y;
	}
	translationF._43 = newPosition.z;

	position.x = newPosition.x;
	position.y = newPosition.y;

	return true;

}
bool Sentence::SetPosition(XMFLOAT2 newPosition) {

	if (center) {
		translationF._41 = newPosition.x - dimentions.x / 2;
		translationF._42 = newPosition.y - dimentions.y / 2;
	}
	else {
		translationF._41 = newPosition.x;
		translationF._42 = newPosition.y;
	}

	position.x = newPosition.x;
	position.y = newPosition.y;

	return true;

}


// деструктор предложения
Sentence::~Sentence() {

	ReleaseCOM(pIndexBuffer);
	ReleaseCOM(pVertexBuffer);

}