#include "Shader.h"
#include "Mediator.h"

XMMATRIX viewLightMatrix; // матрица источника света
XMMATRIX projLightMatrix; // матрица проекции света

// конструктор
Shader::Shader(bool* result) {

	// создать вершинные буферы и раскладки вершин
	BRR(CreateVertexShadersAndLayouts());

	// создать пиксельные шейдеры
	BRR(CreatePixelShaders());

	// создать константные буферы
	BRR(CreateConstBuffers());

	// создать blend state, sampler state и render state
	BRR(CreateStates());

	// создание карты теней
	BRR(CreateShadowMap());

	// вернуть раскладки
	Mediator::pInputLayoutPos3Tex2 = pInputLayoutPos3Tex2; // для спрайтов с текстурой
	Mediator::pInputLayoutSpriteInstance = pInputLayoutSpriteInstance; // для иконок меню
	Mediator::pInputLayoutPos3Nor3 = pInputLayoutPos3Nor3; // для моделей без текстуры
	Mediator::pInputLayoutPos3Nor3Tex2 = pInputLayoutPos3Nor3Tex2; // для моделей c текстурой
	Mediator::pInputLayoutTerrain = pInputLayoutTerrain; // для территории
	Mediator::pInputLayoutPos3Nor3Tex2W1Bone2 = pInputLayoutPos3Nor3Tex2W1Bone2; // для анимационных моделей
	Mediator::pInputLayoutInstance = pInputLayoutInstance; // для вектора моделей с текстурой
	Mediator::pInputLayoutPosCol = pInputLayoutPosCol; // для карты

	// вершинные шейдеры
	Mediator::pVSPos = pVSPos; // для спрайтов без текстуры
	Mediator::pVSPosTex = pVSPosTex; // для спрайтов с текстурой
	Mediator::pVSSpriteInstance = pVSSpriteInstance; // для иконок меню
	Mediator::pVSPosNor = pVSPosNor; // для моделей без текстуры
	Mediator::pVSPosNorTex = pVSPosNorTex; // для моделей с текстурами
	Mediator::pVSTerrain = pVSTerrain; // для регионов территории
	Mediator::pVSPosNorTexWBone = pVSPosNorTexWBone; // для анимационных моделей с костями
	Mediator::pVSShadow = pVSShadow; // тени анимационный моделей с костями
	Mediator::pVSInstance = pVSInstance; // для векторов моделей
	Mediator::pVSShadowInstance = pVSShadowInstance; // для теней векторов моделей
	Mediator::pVSPosCol = pVSPosCol; // для карты

	// пиксельные шейдеры
	Mediator::pPSPos = pPSPos; // для спрайтов без текстуры
	Mediator::pPSPosTex = pPSPosTex; // для спрайтов с текстурой
	Mediator::pPSFont = pPSFont; // для шрифтов
	Mediator::pPSSolid = pPSSolidModel; // для сплошных моделей
	Mediator::pPSPosNor = pPSPosNor; // для моделей без текстуры
	Mediator::pPSPosNorTex = pPSPosNorTex; // для моделей с текстурой
	Mediator::pPSClipping = pPSClipping; // для моделей с текстурой и обрезанием
	Mediator::pPSTerrain = pPSTerrain; // для регионов территории
	Mediator::pPSTerrainNoShadow = pPSTerrainNoShadow; // для регионов территории
	Mediator::pPSEmpty = pPSEmpty; // теневой пустой
	Mediator::pPSPosCol = pPSPosCol; // для карты

	// шейдерные буферы
	Mediator::cbufAnimModel = cbufAnimModel; // для анимационных моделей
	Mediator::cbufReset = cbufReset; // для сброса
	Mediator::cbufObject = cbufObject; // для объектов
	Mediator::cbufFont = cbufFont; // для шрифтов
	Mediator::cbufFrame = cbufFrame; // для каждого кадра

	Mediator::shader = this;

	*result = true;

}

// обновить
bool Shader::Update() {	

	XMVECTOR lightPos = XMVectorSet(Const::lightPosition.x, Const::lightPosition.y, Const::lightPosition.z, 1.0f);
	myXMFLOAT3 lightFocusF = Mediator::hero->GetPosition();
	XMVECTOR lightFocus = XMVectorSet(lightFocusF.x, lightFocusF.y, lightFocusF.z, 1.0f);
	lightPos += lightFocus;
	XMMATRIX viewLightMatrixAbs = XMMatrixLookAtLH(lightPos, lightFocus, camUp);
	myXMFLOAT3 lightPositionJP = Mediator::hero->GetPosition() + Const::lightPosition - Mediator::camera->GetPosition();
	myXMFLOAT3 lightFocusJP = Mediator::hero->GetPosition() - Mediator::camera->GetPosition();
	myXMFLOAT3 lightDirection = lightPositionJP - lightFocusJP;
	XMVECTOR lightV = XMVector3Normalize(XMLoadFloat3(&lightDirection));
	viewLightMatrix = XMMatrixLookAtLH(XMVectorSet(lightPositionJP.x, lightPositionJP.y, lightPositionJP.z, 1.0f), XMVectorSet(lightFocusJP.x, lightFocusJP.y, lightFocusJP.z, 1.0f), camUp);
	projLightMatrix = XMMatrixOrthographicOffCenterLH(Const::viewRect.x, Const::viewRect.y, Const::viewRect.z, Const::viewRect.w, Const::nearFarLightZ.x, Const::nearFarLightZ.y);

	// обновить вершинный буфер шейдера (матрица вида и проекции)
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFrame3* cbufVS = reinterpret_cast<CbufFrame3*>(mappedData.pData);
	XMStoreFloat4x4(&cbufVS->gViewLightMatrix, XMMatrixTranspose(viewLightMatrix));
	XMStoreFloat4x4(&cbufVS->gViewLightMatrixAbs, XMMatrixTranspose(viewLightMatrixAbs));
	XMStoreFloat4x4(&cbufVS->gProjLightMatrix, XMMatrixTranspose(projLightMatrix));
	Mediator::pDevContext->Unmap(cbufLight, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_LIGHT, 1, &cbufLight);

	// обновить пиксельный буфер шейдера (направление вектора)
	HR(Mediator::pDevContext->Map(cbufLightVector, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFrame1* cbufPS = reinterpret_cast<CbufFrame1*>(mappedData.pData);
	XMStoreFloat4(&cbufPS->lightVector, lightV);
	Mediator::pDevContext->Unmap(cbufLightVector, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_LIGHTVECTOR, 1, &cbufLightVector);

	return true;

}

// вернуть матрицу источника света
XMMATRIX Shader::GetViewLightMatrix() {
	return viewLightMatrix;
}
// вернуть проекцию света
XMMATRIX Shader::GetProjLightMatrix() {
	return projLightMatrix;
}

// создать вершинные шейдеры и расклаки вершин
bool Shader::CreateVertexShadersAndLayouts() {
	
	UINT shaderFlags = 0;
#if defined (DEBUG) | defined (_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // DEBUG

	ID3D10Blob* pErrorMsg = NULL;

	// выбор версии шейдеров
	char D10profileVS[] = "vs_4_0"; char D11profileVS[] = "vs_5_0";
	char* profilePointerVS = NULL;
	profilePointerVS = Mediator::featureLevel >= D3D_FEATURE_LEVEL_11_0 ? D11profileVS : D10profileVS;

	// для спрайтов без текстуры
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPos", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPos));
	ReleaseCOM(VSBlob);

	// для спрайтов с текстурой
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosTex", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosTex));
	D3D11_INPUT_ELEMENT_DESC elementDescPosTex[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosTex, ARRAYSIZE(elementDescPosTex), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Tex2));
	ReleaseCOM(VSBlob);

	// для иконок меню
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSSpriteInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSSpriteInstance));
	// для иконок меню
	D3D11_INPUT_ELEMENT_DESC elementDescSpriteInstance[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // смещение координат
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(XMFLOAT3), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // смещение текстуры
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescSpriteInstance, ARRAYSIZE(elementDescSpriteInstance), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutSpriteInstance));
	ReleaseCOM(VSBlob);

	// для моделей без текстуры
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosNor", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosNor));
	D3D11_INPUT_ELEMENT_DESC elementDescPosNor[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosNor, ARRAYSIZE(elementDescPosNor), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Nor3));
	ReleaseCOM(VSBlob);

	// для моделей с текстурой
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosNorTex", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosNorTex));
	D3D11_INPUT_ELEMENT_DESC elementDescPosNorTex[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosNorTex, ARRAYSIZE(elementDescPosNorTex), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Nor3Tex2));
	ReleaseCOM(VSBlob);

	// для регионов территории
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSTerrain", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSTerrain));
	D3D11_INPUT_ELEMENT_DESC elementDescTerrain[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // для normal mapping
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3 + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3 + sizeof(XMFLOAT2)* 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // маска
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescTerrain, ARRAYSIZE(elementDescTerrain), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutTerrain));
	ReleaseCOM(VSBlob);

	// для анимационных моделей
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosNorTexWBone", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosNorTexWBone));
	D3D11_INPUT_ELEMENT_DESC elementDescPosNorTexWBone[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)* 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)* 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(float)* 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32_UINT, 0, sizeof(float)* 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosNorTexWBone, ARRAYSIZE(elementDescPosNorTexWBone), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Nor3Tex2W1Bone2));
	ReleaseCOM(VSBlob);

	// тени для анимационных моделей
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSShadow", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSShadow));
	ReleaseCOM(VSBlob);

	// для вектора моделей
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSInstance));
	D3D11_INPUT_ELEMENT_DESC elementDescInstance[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // смещение
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32_FLOAT, 1, sizeof(XMFLOAT3), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // вращение
		{ "INSTANCECOLOR", 1, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(XMFLOAT3)+sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // масштабирование
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescInstance, ARRAYSIZE(elementDescInstance), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutInstance));
	ReleaseCOM(VSBlob);

	// тень для вектора моделей
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSShadowInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSShadowInstance));
	ReleaseCOM(VSBlob);

	// для карты
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosCol", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosCol));
	D3D11_INPUT_ELEMENT_DESC elementDescPosCol[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosCol, ARRAYSIZE(elementDescPosCol), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPosCol));
	ReleaseCOM(VSBlob);

	return true;

}

// создание карты теней
bool Shader::CreateShadowMap() {

	// текстура 2D
	D3D11_TEXTURE2D_DESC textDesc = { 0 };
	textDesc.ArraySize = 1;
	textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textDesc.CPUAccessFlags = 0;
	textDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textDesc.Height = Const::shadowMapSize.y;
	textDesc.MipLevels = 1;
	textDesc.MiscFlags = 0;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.Width = Const::shadowMapSize.x;
	ID3D11Texture2D* pShadowTexture = NULL;
	HR(Mediator::pDev->CreateTexture2D(&textDesc, NULL, &pShadowTexture));

	// карта теней
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(Mediator::pDev->CreateDepthStencilView(pShadowTexture, &dsvDesc, &pShadowMapDepthView));

	// ресурс для карты теней
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(Mediator::pDev->CreateShaderResourceView(pShadowTexture, &srvDesc, &pShadowResourceView));

	ReleaseCOM(pShadowTexture);

	return true;

}

// создать пиксельные шейдеры
bool Shader::CreatePixelShaders() {

	UINT shaderFlags = 0;
#if defined (DEBUG) | defined (_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // DEBUG

	ID3D10Blob* pErrorMsg = NULL;

	// выбор версии шейдеров
	char D10profilePS[] = "ps_4_0"; char D11profilePS[] = "ps_5_0";
	char* profilePointerVS = NULL; char* profilePointerPS = NULL;
	profilePointerPS = Mediator::featureLevel >= D3D_FEATURE_LEVEL_11_0 ? D11profilePS : D10profilePS;

	// для спрайта без текстуры
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPos", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPos));
	ReleaseCOM(PSBlob);

	// для спрайта с текстурой
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosTex", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosTex));
	ReleaseCOM(PSBlob);

	// для шрифтов
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSFont", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSFont));
	ReleaseCOM(PSBlob);

	// для сплошных моделей
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSSolid", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSSolidModel));
	ReleaseCOM(PSBlob);

	// для модели без текстуры
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosNor", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosNor));
	ReleaseCOM(PSBlob);

	// для модели с текстурой
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosNorTex", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosNorTex));
	ReleaseCOM(PSBlob);

	// для модели с текстурой и обрезанием
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSClipping", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSClipping));
	ReleaseCOM(PSBlob);

	// для регионов территории
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSTerrain", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSTerrain));
	ReleaseCOM(PSBlob);

	// для регионов территории (без тени)
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSTerrainNoShadow", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSTerrainNoShadow));
	ReleaseCOM(PSBlob);

	// пустой теневой
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSEmpty", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSEmpty));
	ReleaseCOM(PSBlob);

	// для карты
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosCol", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosCol));
	ReleaseCOM(PSBlob);

	return true;

}

// создать константные буферы
bool Shader::CreateConstBuffers() {

	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_MAPPED_SUBRESOURCE mappedData;

	// единственная инициализация
	cbbd.ByteWidth = sizeof(CbufFrame1);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufLightVector));

	// для анимационных моделей
	cbbd.ByteWidth = sizeof(CbufAnimModel);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufAnimModel));

	// для камеры
	cbbd.ByteWidth = sizeof(CbufReset);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufReset));

	// для объектов
	cbbd.ByteWidth = sizeof(CbufObject);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufObject));

	// для шрифтов
	cbbd.ByteWidth = sizeof(CbufFont);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufFont));

	// для каждого кадра
	cbbd.ByteWidth = sizeof(CbufFrame2);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufFrame));

	// для карты теней
	cbbd.ByteWidth = sizeof(CbufFrame3);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufLight));

	// вечные константы
	cbbd.ByteWidth = sizeof(CbufOnce);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufOnce));
	HR(Mediator::pDevContext->Map(cbufOnce, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufOnce* cbuf = reinterpret_cast<CbufOnce*>(mappedData.pData);
	cbuf->shadowMapDimentons = XMFLOAT4((float)Const::shadowMapSize.x, (float)Const::shadowMapSize.y, 0.0f, 0.0f);
	Mediator::pDevContext->Unmap(cbufOnce, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_ONCE, 1, &cbufOnce);

	return true;

}

// создать blend state, sampler state и render state
bool Shader::CreateStates() {

	// blend state
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	HR(Mediator::pDev->CreateBlendState(&blendDesc, &pBlendState));

	// создание и установка RendesStates
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	Const::backCulling ? rastDesc.CullMode = D3D11_CULL_BACK : rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	HR(Mediator::pDev->CreateRasterizerState(&rastDesc, &pRastStandard));
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	HR(Mediator::pDev->CreateRasterizerState(&rastDesc, &pRastWireframe));
	if (Const::wireFrame) Mediator::pDevContext->RSSetState(pRastWireframe);
	else Mediator::pDevContext->RSSetState(pRastStandard);

	{// установка SamplerStates

		// стандартный
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = Const::anisotrophicFilter ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.MaxAnisotropy = 1;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		float borderColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		memcpy(&sampDesc.BorderColor, borderColor, sizeof(float)* 4);
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		ID3D11SamplerState* pSampState = NULL;
		HR(Mediator::pDev->CreateSamplerState(&sampDesc, &pSampState));
		Mediator::pDevContext->PSSetSamplers(SAMPLER_BUFFER, 1, &pSampState);
		ReleaseCOM(pSampState);

		// для теней
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		HR(Mediator::pDev->CreateSamplerState(&sampDesc, &pSampState));
		Mediator::pDevContext->PSSetSamplers(SAMPLER_SHADOW, 1, &pSampState);
		ReleaseCOM(pSampState);

	}

	return true;

}

// установить мировую матрицу
bool Shader::SetObjectBuffer(XMFLOAT4X4* worldMatrix) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	XMMATRIX M = XMLoadFloat4x4(worldMatrix);
	HR(Mediator::pDevContext->Map(cbufObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufObject* cbuf = reinterpret_cast<CbufObject*>(mappedData.pData);
	XMStoreFloat4x4(&cbuf->gWorld, XMMatrixTranspose(M));
	Mediator::pDevContext->Unmap(cbufObject, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_MODEL, 1, &cbufObject);

	return true;

}
bool Shader::SetObjectBuffer(CXMMATRIX worldMatrix) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufObject* cbuf = reinterpret_cast<CbufObject*>(mappedData.pData);
	XMStoreFloat4x4(&cbuf->gWorld, XMMatrixTranspose(worldMatrix));
	Mediator::pDevContext->Unmap(cbufObject, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_MODEL, 1, &cbufObject);

	return true;

}

// установить параметры камеры
bool Shader::SetCamBuffer(CXMMATRIX viewProjMatrix, XMFLOAT4 camPos, XMFLOAT4 camFocus) {

	// передача данных в шейдер
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFrame2* cbuf = reinterpret_cast<CbufFrame2*>(mappedData.pData);
	cbuf->camPos = camPos;
	cbuf->camFocus = camFocus;
	XMStoreFloat4x4(&cbuf->gViewProjMatrix, XMMatrixTranspose(viewProjMatrix));
	Mediator::pDevContext->Unmap(cbufFrame, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_FRAME, 1, &cbufFrame);

	return true;

}

// установить цвет шрифта
bool Shader::SetFontBuffer(XMFLOAT4* fontColor) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFont* cbuf = reinterpret_cast<CbufFont*>(mappedData.pData);
	cbuf->fontColor = *fontColor;
	Mediator::pDevContext->Unmap(cbufFont, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_FONT, 1, &cbufFont);

	return true;

}

// установить параметры сброса
bool Shader::SetResetBuffer(CXMMATRIX orthoMatrix) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufReset, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufReset* cbuf = reinterpret_cast<CbufReset*>(mappedData.pData);

	XMStoreFloat4x4(&cbuf->gOrthoMatrix, XMMatrixTranspose(orthoMatrix));

	Mediator::pDevContext->Unmap(cbufReset, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_CAMERA, 1, &cbufReset);

	return true;

}

// установить/снять Alpha Blending
bool Shader::SetAlphaBlending(bool enable) {

	if (!Const::alpha) return true;

	static bool prevState = !enable;
	if (prevState == enable) return true; // ничего не изменилось

	// установить новое состояние
	const float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (enable) Mediator::pDevContext->OMSetBlendState(pBlendState, blendFactor, 0xffffff);
	else Mediator::pDevContext->OMSetBlendState(NULL, blendFactor, 0xffffff);
	prevState = enable;

	return true;

}

// установить/снять wireframe
bool Shader::SetWireframe(bool wireframe) {

	static bool prevWireframe = Const::wireFrame;
	if (wireframe == prevWireframe) return true; // ничего не изменилось

	// установить новое состояние
	if (wireframe) Mediator::pDevContext->RSSetState(pRastWireframe);
	else Mediator::pDevContext->RSSetState(pRastStandard);
	prevWireframe = wireframe;

	return true;

}

// деструктор
Shader::~Shader() {

	// константные буферы
	ReleaseCOM(cbufLightVector);
	ReleaseCOM(cbufAnimModel);
	ReleaseCOM(cbufReset);
	ReleaseCOM(cbufObject);
	ReleaseCOM(cbufFont);
	ReleaseCOM(cbufFrame);
	ReleaseCOM(cbufLight);
	ReleaseCOM(cbufOnce);

	// шейдеры
	ReleaseCOM(pVSPos);
	ReleaseCOM(pVSPosTex);
	ReleaseCOM(pVSSpriteInstance);
	ReleaseCOM(pVSPosNorTex);
	ReleaseCOM(pVSPosNor);
	ReleaseCOM(pVSTerrain);
	ReleaseCOM(pVSPosNorTexWBone);
	ReleaseCOM(pVSShadow);
	ReleaseCOM(pVSInstance);
	ReleaseCOM(pVSShadowInstance);
	ReleaseCOM(pVSPosCol);
	ReleaseCOM(pPSPos);
	ReleaseCOM(pPSPosTex);
	ReleaseCOM(pPSFont);
	ReleaseCOM(pPSSolidModel);
	ReleaseCOM(pPSPosNor);
	ReleaseCOM(pPSPosNorTex);
	ReleaseCOM(pPSTerrain);
	ReleaseCOM(pPSTerrainNoShadow);
	ReleaseCOM(pPSClipping);
	ReleaseCOM(pPSEmpty);
	ReleaseCOM(pPSPosCol);

	// раскладки вершин
	ReleaseCOM(pInputLayoutPos3Tex2);
	ReleaseCOM(pInputLayoutPos3Nor3);
	ReleaseCOM(pInputLayoutPos3Nor3Tex2);
	ReleaseCOM(pInputLayoutTerrain);
	ReleaseCOM(pInputLayoutPos3Nor3Tex2W1Bone2);
	ReleaseCOM(pInputLayoutSpriteInstance);
	ReleaseCOM(pInputLayoutInstance);
	ReleaseCOM(pInputLayoutPosCol);

	// прочее
	ReleaseCOM(pBlendState);
	ReleaseCOM(pRastStandard);
	ReleaseCOM(pRastWireframe);
	ReleaseCOM(pShadowResourceView);
	ReleaseCOM(pShadowMapDepthView);

}
