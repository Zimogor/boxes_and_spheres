#include "AnimModel.h"
#include "Mediator.h"

// обновление
bool SingleAnimatedModel::Update(float dT) {

	if (restCurClip) { // смешение анимации отсутствует

		// обновить текущий кадр
		accumulation[curClip] += dT * Const::animationSpeed;
		curFrame[curClip] = (UINT)accumulation[curClip];
		if (curFrame[curClip] >= framesAmount[curClip]) {
			curFrame[curClip] -= framesAmount[curClip];
			accumulation[curClip] -= framesAmount[curClip];
		}

	}
	else { // анимация смешивается

		// обновить веса клипов
		for (UINT i(0); i < clipsAmount; i++) {
			if (i == curClip) blendFactors[i] += dT * clipBlendSpeed;
			else blendFactors[i] -= dT * clipBlendSpeed;
			blendFactors[i] = MathHelper::Clutch(blendFactors[i], 0.0f, 1.0f);
		}

		// обновить текущий кадр массива клипов
		for (UINT i(0); i < clipsAmount; i++) {
			if (blendFactors[i]) {
				accumulation[i] += dT * Const::animationSpeed;
				curFrame[i] = (UINT)accumulation[i];
				if (curFrame[i] >= framesAmount[i]) {
					curFrame[i] -= framesAmount[i];
					accumulation[i] -= framesAmount[i];
				}
			}
			else accumulation[i] = 0.0f;
		}

		// пора переключать анимацию?
		if (waitNextClip) {
			nextClipTime -= dT;
			if (nextClipTime <= 1.0f / clipBlendSpeed) {
				// анимация переключается
				SingleAnimatedModel::SetCurrentClip(nextClip);
			}
		}
		else {
			// анимация смешивается?
			for (UINT i(0); i < clipsAmount; i++) {
				if (i == curClip) {
					if (blendFactors[i] != 1.0f) break;
				}
				else {
					if (blendFactors[i] != 0.0f) break;
				}
				// анимация больше не смешивается
				if (i == (clipsAmount - 1)) restCurClip = true;
			}
		}

	}

	// ограничение углов пределом в 2пи
	if (rotAngle >= XM_2PI) rotAngle -= XM_2PI;
	if (rotAngle < 0.0f) rotAngle += XM_2PI;
	if (drawAngle >= XM_2PI) drawAngle -= XM_2PI;
	if (drawAngle < 0.0f) drawAngle += XM_2PI;

	// угловое перемещение
	float maxRotAngle = angleSpeed * dT;
	if (abs(rotAngle - drawAngle) > XM_PI) {
		// перемещение ближе через край 2Пи
		if (drawAngle - rotAngle > maxRotAngle) drawAngle += maxRotAngle; // по часовой стрелке
		else if (rotAngle - drawAngle > maxRotAngle) drawAngle -= maxRotAngle; // против часовой стрелки
	}
	else {
		// перемещение ближе напрямую
		if (drawAngle - rotAngle > maxRotAngle) drawAngle -= maxRotAngle; // против часовой стрелки
		else if (rotAngle - drawAngle > maxRotAngle) drawAngle += maxRotAngle; // по часовой стрелке
	}

	// обновить матрицу вращения
	XMStoreFloat4x4(&rotationMatrix, XMMatrixRotationY(drawAngle));
	recreateFinalMatrices = true;

	return true;

}

// установить новый клип
float SingleAnimatedModel::SetCurrentClip(UINT clipNumber) { // зациклить

	curClip = clipNumber;
	restCurClip = false;
	waitNextClip = false;
	nextClipTime = 0.0f;

	return framesAmount[curClip] / (float)Const::animationSpeed;

}
float SingleAnimatedModel::SetCurrentClip(UINT clipNumber, UINT nextClipNumber) { // один цикл

	curClip = clipNumber;
	restCurClip = false;
	nextClip = nextClipNumber;
	waitNextClip = true;
	nextClipTime = framesAmount[curClip] / (float)Const::animationSpeed;

	return nextClipTime;

}

// конструктор модели с костями
AnimModel::AnimModel(wchar_t* binFilePath, wchar_t* textureFilePath, bool* result) {

	// инициализация переменных
	position = Const::spawnPoint; // координаты модели

	// загрузить модель из файла
	BRR(LoadAmimModelFromFile(binFilePath));

	// инициализация элементов после загрузки
	std::fill(curFrame.begin(), curFrame.end(), 0);
	std::fill(accumulation.begin(), accumulation.end(), 0.0f);
	std::fill(blendFactors.begin(), blendFactors.end(), 0.0f);
	blendFactors[0] = 1.0f;

	// создать матрицу порядка умножения финальных костей
	BRR(BuildOrder());

	// загрузить текстуру
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, textureFilePath, NULL, NULL, &pSRtexture, NULL))) {
		BRR(Mediator::errors->Push(textureFilePath));
		*result = false;
		return;
	}

	*result = true;

}

// загрузить модель из файла
bool AnimModel::LoadAmimModelFromFile(wchar_t* binFilePath) {

	// открытие файла
	std::ifstream fin;
	fin.open(binFilePath, std::ios_base::binary);
	BR(fin.is_open());

	// чтение заголовка
	UINT vertexAmount = 0; // количество вершин
	fin.read((char*)&vertexAmount, sizeof(UINT));
	fin.read((char*)&indicesAmount, sizeof(UINT));
	fin.read((char*)&bonesAmount, sizeof(UINT));
	fin.read((char*)&clipsAmount, sizeof(UINT));
	clips.resize(clipsAmount);
	curFrame.resize(clipsAmount);
	framesAmount = new UINT[clipsAmount];
	accumulation.resize(clipsAmount);
	blendFactors.resize(clipsAmount);
	for (UINT i(0); i < clipsAmount; i++) {
		fin.read((char*)&framesAmount[i], sizeof(UINT));
	}
	BR(bonesAmount <= 32);

	// чтение буфера вершин
	VertPos3Nor2Tex2W1Bone2* vertexBuffer = new VertPos3Nor2Tex2W1Bone2[vertexAmount];
	fin.read((char*)vertexBuffer, sizeof(VertPos3Nor2Tex2W1Bone2)* vertexAmount);

	// чтение буфера индексов
	UINT* indexBuffer = new UINT[indicesAmount];
	fin.read((char*)indexBuffer, sizeof(UINT)* indicesAmount);

	// чтение иерархии
	hierarchy.resize(bonesAmount);
	for (UINT i(0); i < bonesAmount; i++) {
		fin.read((char*)&hierarchy[i], sizeof(int));
	}

	// чтение матриц offset
	offsetMatrices.resize(bonesAmount);
	XMFLOAT3 pos;
	XMFLOAT4 quat;
	for (UINT i(0); i < bonesAmount; i++) {
		fin.read((char*)&pos, sizeof(XMFLOAT3));
		fin.read((char*)&quat, sizeof(XMFLOAT4));
		XMMATRIX T = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&quat));
		XMStoreFloat4x4(&offsetMatrices[i], R * T);
	}

	// чтение наследственных матриц костей
	clips.resize(clipsAmount);
	for (UINT clipNumber(0); clipNumber < clipsAmount; clipNumber++) {
		clips[clipNumber].bonesPositions = new XMFLOAT3[framesAmount[clipNumber] * bonesAmount];
		clips[clipNumber].bonesQuaternions = new XMFLOAT4[framesAmount[clipNumber] * bonesAmount];
		XMFLOAT3* posPointer = clips[clipNumber].bonesPositions;
		XMFLOAT4* quatPointer = clips[clipNumber].bonesQuaternions;
		for (UINT i(0); i < framesAmount[clipNumber]; i++)
			for (UINT j(0); j < bonesAmount; j++) {
				fin.read((char*)posPointer, sizeof(XMFLOAT3));
				BR(!fin.eof());
				fin.read((char*)quatPointer, sizeof(XMFLOAT4));
				posPointer++;
				quatPointer++;
			}
	}

	// структура буфера вершин
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(VertPos3Nor2Tex2W1Bone2)* vertexAmount;
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфера вершин
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = vertexBuffer;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	// структура буфера индексов
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT)* indicesAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// создание буфер индексов
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indexBuffer;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	// проверка на чтение всего файла
	UINT endOfFileBuffer;
	fin.read((char*)&endOfFileBuffer, sizeof(UINT));
	BR(fin.eof());

	// освобождение ресурсов
	ReleaseNULLS(indexBuffer);
	ReleaseNULLS(vertexBuffer);
	fin.close();

	return true;

}

// построить ветор order для умножния матриц
bool AnimModel::BuildOrder() {

	std::vector<int> newParent;
	std::vector<int> newChild;

	// собрать родительские кости
	for (UINT i(0); i < bonesAmount; i++) {
		if (hierarchy[i] == -1) {
			newParent.push_back(i);
			order.push_back(i);
		}
	}

	// собрать все остальные кости
	for (UINT k(0); k < bonesAmount; k++) {
		for (UINT i(0); i < bonesAmount; i++) {
			for (UINT j(0); j < newParent.size(); j++) {
				if (hierarchy[i] == newParent[j])
					newChild.push_back(i);
			}
		}
		newParent.clear();
		for (UINT i(0); i < newChild.size(); i++) {
			order.push_back(newChild[i]);
			if (order.size() == bonesAmount) return true;
			newParent.push_back(newChild[i]);
		}
		newChild.clear();
	}

	return false;

}

// создать финальные матрицы для шейдера
void AnimModel::CreateFinalMatricesOldStyle(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices) { // без смешивания клипов по древнему алгоритму (для отладки)

	// загрузка матриц
	std::vector<XMFLOAT4X4> reserve(bonesAmount);
	std::vector<XMFLOAT4X4> reserve2(bonesAmount);
	for (UINT i(0); i < bonesAmount; i++) {
		XMFLOAT3 position = clips[clipNumber].bonesPositions[i + bonesAmount * frameNumber];
		XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);
		XMFLOAT4 quaternion = clips[clipNumber].bonesQuaternions[i + bonesAmount * frameNumber];
		XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion));
		XMStoreFloat4x4(&reserve[i], R * T);
		XMStoreFloat4x4(&reserve2[i], R * T);
	}

	// перемножение матриц для получения final
	for (UINT i(0); i < bonesAmount; i++) {
		XMMATRIX fin = XMMatrixIdentity();
		UINT curMatrix = i;
		fin = fin * XMLoadFloat4x4(&reserve[curMatrix]);
		while (curMatrix != -1) {
			curMatrix = hierarchy[curMatrix];
			if (curMatrix == -1) break;
			fin = fin * XMLoadFloat4x4(&reserve[curMatrix]);
		}
		XMMATRIX offSet = XMLoadFloat4x4(&offsetMatrices[i]);
		XMStoreFloat4x4(&finalMatrices[i], offSet * fin);
	}

}
void AnimModel::CreateFinalMatrices(UINT clipNumber, UINT frameNumber, XMFLOAT4X4* finalMatrices) { // без смешивания клипов

	// загрузка матриц
	std::vector<XMFLOAT4X4> toRoot(bonesAmount);
	for (UINT i(0); i < bonesAmount; i++) {
		XMFLOAT3 position = clips[clipNumber].bonesPositions[i + bonesAmount * frameNumber];
		XMMATRIX T = XMMatrixTranslation(position.x, position.y, position.z);
		XMFLOAT4 quaternion = clips[clipNumber].bonesQuaternions[i + bonesAmount * frameNumber];
		XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion));
		XMStoreFloat4x4(&toRoot[i], R * T);
	}

	// перемножение матриц для получения final
	for (UINT i(0); i < bonesAmount; i++) {
		UINT coef = order[i];
		if (hierarchy[coef] == -1) continue;
		XMStoreFloat4x4(&toRoot[coef], XMLoadFloat4x4(&toRoot[coef]) * XMLoadFloat4x4(&toRoot[hierarchy[coef]]));
	}
	for (UINT i(0); i < bonesAmount; i++) {
		XMMATRIX offSet = XMLoadFloat4x4(&offsetMatrices[i]);
		XMStoreFloat4x4(&finalMatrices[i], offSet * XMLoadFloat4x4(&toRoot[i]));
	}

}
void AnimModel::CreateFinalMatrices(UINT clipNumber1, UINT frameNumber1, UINT clipNumber2, UINT frameNumber2, float blendFactor, XMFLOAT4X4* finalMatrices) { // смешивание двух клипов

	XMFLOAT3 positionF3;
	XMVECTOR quatF4;

	std::vector<XMFLOAT4X4> toRoot(bonesAmount);
	for (UINT i(0); i < bonesAmount; i++) {

		// загрузка матриц
		XMVECTOR position1 = XMLoadFloat3(&clips[clipNumber1].bonesPositions[i + bonesAmount * frameNumber1]);
		XMVECTOR position2 = XMLoadFloat3(&clips[clipNumber2].bonesPositions[i + bonesAmount * frameNumber2]);
		XMStoreFloat3(&positionF3, XMVectorLerp(position1, position2, blendFactor));
		XMVECTOR quaternion1 = XMLoadFloat4(&clips[clipNumber1].bonesQuaternions[i + bonesAmount * frameNumber1]);
		XMVECTOR quaternion2 = XMLoadFloat4(&clips[clipNumber2].bonesQuaternions[i + bonesAmount * frameNumber2]);
		quatF4 = XMQuaternionSlerp(quaternion1, quaternion2, blendFactor);

		// интерполяция матриц
		XMMATRIX T = XMMatrixTranslation(positionF3.x, positionF3.y, positionF3.z);
		XMMATRIX R = XMMatrixRotationQuaternion(quatF4);
		XMStoreFloat4x4(&toRoot[i], R * T);
	}

	// перемножение матриц для получения final
	for (UINT i(0); i < bonesAmount; i++) {
		UINT coef = order[i];
		if (hierarchy[coef] == -1) continue;
		XMStoreFloat4x4(&toRoot[coef], XMLoadFloat4x4(&toRoot[coef]) * XMLoadFloat4x4(&toRoot[hierarchy[coef]]));
	}
	for (UINT i(0); i < bonesAmount; i++) {
		XMMATRIX offSet = XMLoadFloat4x4(&offsetMatrices[i]);
		XMStoreFloat4x4(&finalMatrices[i], offSet * XMLoadFloat4x4(&toRoot[i]));
	}

}
void AnimModel::CreateFinalMatrices(std::vector<UINT> frameNumbers, std::vector<float> blendFactors, XMFLOAT4X4* finalMatrices) { // смешивание нескольких клипов

	std::vector<XMFLOAT4X4> toRoot(bonesAmount);

	// поиск ненулевых весов
	std::vector<UINT> iterators;
	for (UINT i(0); i < clips.size(); i++)
		if (blendFactors[i]) iterators.push_back(i);

	// для каждой кости
	for (UINT i(0); i < bonesAmount; i++) {

		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT4 quaternion = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		// создание вектора для обработки
		std::vector<XMFLOAT4> quats;
		std::vector<XMFLOAT3> posits;
		std::vector<float> weights;
		for (UINT j(0); j < iterators.size(); j++) {
			quats.push_back(clips[iterators[j]].bonesQuaternions[i + bonesAmount * frameNumbers[iterators[j]]]);
			posits.push_back(clips[iterators[j]].bonesPositions[i + bonesAmount * frameNumbers[iterators[j]]]);
			weights.push_back(blendFactors[iterators[j]]);
		}

		// обработка вектора
		while (quats.size() > 1) {
			XMFLOAT4 newQuat;
			XMFLOAT3 newPosit;
			float relWeight = weights[1] / (weights[1] + weights[0]);
			XMStoreFloat4(&newQuat, XMQuaternionSlerp(XMLoadFloat4(&quats[0]), XMLoadFloat4(&quats[1]), relWeight));
			XMStoreFloat3(&newPosit, XMVectorLerp(XMLoadFloat3(&posits[0]), XMLoadFloat3(&posits[1]), relWeight));
			float newWeight = weights[0] + weights[1];
			quats.erase(quats.begin());
			quats.erase(quats.begin());
			quats.push_back(newQuat);
			posits.erase(posits.begin());
			posits.erase(posits.begin());
			posits.push_back(newPosit);
			weights.erase(weights.begin());
			weights.erase(weights.begin());
			weights.push_back(newWeight);
		}

		// запись интерполированных матриц матриц
		XMVECTOR quatResult = XMLoadFloat4(&quats[0]);
		XMMATRIX R = XMMatrixRotationQuaternion(quatResult);
		XMMATRIX T = XMMatrixTranslation(posits[0].x, posits[0].y, posits[0].z);
		XMStoreFloat4x4(&toRoot[i], R * T);

	}

	// перемножение матриц для получения final
	for (UINT i(0); i < bonesAmount; i++) {
		UINT coef = order[i];
		if (hierarchy[coef] == -1) continue;
		XMStoreFloat4x4(&toRoot[coef], XMLoadFloat4x4(&toRoot[coef]) * XMLoadFloat4x4(&toRoot[hierarchy[coef]]));
	}
	for (UINT i(0); i < bonesAmount; i++) {
		XMMATRIX offSet = XMLoadFloat4x4(&offsetMatrices[i]);
		XMStoreFloat4x4(&finalMatrices[i], offSet * XMLoadFloat4x4(&toRoot[i]));
	}

}

// рисовать модель
bool AnimModel::Draw(bool toShadowMap) {

	// установить константы
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	XMMATRIX R = XMLoadFloat4x4(&GetRotationMatrix());
	if (recreateFinalMatrices) {
		if (restCurClip) CreateFinalMatrices(curClip, curFrame[curClip], finalMatrices);
		else CreateFinalMatrices(curFrame, blendFactors, finalMatrices);
		recreateFinalMatrices = false;
	}
	UINT vertexStride = sizeof(VertPos3Nor2Tex2W1Bone2);
	UINT vertexOffset = 0;

	// установить параметры
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2W1Bone2);
	if (toShadowMap) {
		Mediator::pDevContext->VSSetShader(Mediator::pVSShadow, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSEmpty, NULL, 0);
	}
	else {
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
		Mediator::pDevContext->VSSetShader(Mediator::pVSPosNorTexWBone, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPosNorTex, NULL, 0);
	}

	// обновить константный буфер шейдера
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(Mediator::cbufAnimModel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufAnimModel* cbuf = reinterpret_cast<CbufAnimModel*>(mappedData.pData);
	XMStoreFloat4x4(&cbuf->gWorld, XMMatrixTranspose(R * W_JP));
	for (UINT i(0); i < bonesAmount; i++) {
		XMStoreFloat4x4(&cbuf->gBones[i], XMMatrixTranspose(XMLoadFloat4x4(&finalMatrices[i])));
	}
	Mediator::pDevContext->Unmap(Mediator::cbufAnimModel, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_ANIMMODEL, 1, &Mediator::cbufAnimModel);

	// рисование
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// деструктор
AnimModel::~AnimModel() {

	ReleaseNULLS(framesAmount);
	ReleaseCOM(pSRtexture);
	ReleaseCOM(pVertexBuffer);
	ReleaseCOM(pIndexBuffer);

}