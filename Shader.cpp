#include "Shader.h"
#include "Mediator.h"

XMMATRIX viewLightMatrix; // ������� ��������� �����
XMMATRIX projLightMatrix; // ������� �������� �����

// �����������
Shader::Shader(bool* result) {

	// ������� ��������� ������ � ��������� ������
	BRR(CreateVertexShadersAndLayouts());

	// ������� ���������� �������
	BRR(CreatePixelShaders());

	// ������� ����������� ������
	BRR(CreateConstBuffers());

	// ������� blend state, sampler state � render state
	BRR(CreateStates());

	// �������� ����� �����
	BRR(CreateShadowMap());

	// ������� ���������
	Mediator::pInputLayoutPos3Tex2 = pInputLayoutPos3Tex2; // ��� �������� � ���������
	Mediator::pInputLayoutSpriteInstance = pInputLayoutSpriteInstance; // ��� ������ ����
	Mediator::pInputLayoutPos3Nor3 = pInputLayoutPos3Nor3; // ��� ������� ��� ��������
	Mediator::pInputLayoutPos3Nor3Tex2 = pInputLayoutPos3Nor3Tex2; // ��� ������� c ���������
	Mediator::pInputLayoutTerrain = pInputLayoutTerrain; // ��� ����������
	Mediator::pInputLayoutPos3Nor3Tex2W1Bone2 = pInputLayoutPos3Nor3Tex2W1Bone2; // ��� ������������ �������
	Mediator::pInputLayoutInstance = pInputLayoutInstance; // ��� ������� ������� � ���������
	Mediator::pInputLayoutPosCol = pInputLayoutPosCol; // ��� �����

	// ��������� �������
	Mediator::pVSPos = pVSPos; // ��� �������� ��� ��������
	Mediator::pVSPosTex = pVSPosTex; // ��� �������� � ���������
	Mediator::pVSSpriteInstance = pVSSpriteInstance; // ��� ������ ����
	Mediator::pVSPosNor = pVSPosNor; // ��� ������� ��� ��������
	Mediator::pVSPosNorTex = pVSPosNorTex; // ��� ������� � ����������
	Mediator::pVSTerrain = pVSTerrain; // ��� �������� ����������
	Mediator::pVSPosNorTexWBone = pVSPosNorTexWBone; // ��� ������������ ������� � �������
	Mediator::pVSShadow = pVSShadow; // ���� ������������ ������� � �������
	Mediator::pVSInstance = pVSInstance; // ��� �������� �������
	Mediator::pVSShadowInstance = pVSShadowInstance; // ��� ����� �������� �������
	Mediator::pVSPosCol = pVSPosCol; // ��� �����

	// ���������� �������
	Mediator::pPSPos = pPSPos; // ��� �������� ��� ��������
	Mediator::pPSPosTex = pPSPosTex; // ��� �������� � ���������
	Mediator::pPSFont = pPSFont; // ��� �������
	Mediator::pPSSolid = pPSSolidModel; // ��� �������� �������
	Mediator::pPSPosNor = pPSPosNor; // ��� ������� ��� ��������
	Mediator::pPSPosNorTex = pPSPosNorTex; // ��� ������� � ���������
	Mediator::pPSClipping = pPSClipping; // ��� ������� � ��������� � ����������
	Mediator::pPSTerrain = pPSTerrain; // ��� �������� ����������
	Mediator::pPSTerrainNoShadow = pPSTerrainNoShadow; // ��� �������� ����������
	Mediator::pPSEmpty = pPSEmpty; // ������� ������
	Mediator::pPSPosCol = pPSPosCol; // ��� �����

	// ��������� ������
	Mediator::cbufAnimModel = cbufAnimModel; // ��� ������������ �������
	Mediator::cbufReset = cbufReset; // ��� ������
	Mediator::cbufObject = cbufObject; // ��� ��������
	Mediator::cbufFont = cbufFont; // ��� �������
	Mediator::cbufFrame = cbufFrame; // ��� ������� �����

	Mediator::shader = this;

	*result = true;

}

// ��������
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

	// �������� ��������� ����� ������� (������� ���� � ��������)
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFrame3* cbufVS = reinterpret_cast<CbufFrame3*>(mappedData.pData);
	XMStoreFloat4x4(&cbufVS->gViewLightMatrix, XMMatrixTranspose(viewLightMatrix));
	XMStoreFloat4x4(&cbufVS->gViewLightMatrixAbs, XMMatrixTranspose(viewLightMatrixAbs));
	XMStoreFloat4x4(&cbufVS->gProjLightMatrix, XMMatrixTranspose(projLightMatrix));
	Mediator::pDevContext->Unmap(cbufLight, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_LIGHT, 1, &cbufLight);

	// �������� ���������� ����� ������� (����������� �������)
	HR(Mediator::pDevContext->Map(cbufLightVector, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFrame1* cbufPS = reinterpret_cast<CbufFrame1*>(mappedData.pData);
	XMStoreFloat4(&cbufPS->lightVector, lightV);
	Mediator::pDevContext->Unmap(cbufLightVector, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_LIGHTVECTOR, 1, &cbufLightVector);

	return true;

}

// ������� ������� ��������� �����
XMMATRIX Shader::GetViewLightMatrix() {
	return viewLightMatrix;
}
// ������� �������� �����
XMMATRIX Shader::GetProjLightMatrix() {
	return projLightMatrix;
}

// ������� ��������� ������� � �������� ������
bool Shader::CreateVertexShadersAndLayouts() {
	
	UINT shaderFlags = 0;
#if defined (DEBUG) | defined (_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // DEBUG

	ID3D10Blob* pErrorMsg = NULL;

	// ����� ������ ��������
	char D10profileVS[] = "vs_4_0"; char D11profileVS[] = "vs_5_0";
	char* profilePointerVS = NULL;
	profilePointerVS = Mediator::featureLevel >= D3D_FEATURE_LEVEL_11_0 ? D11profileVS : D10profileVS;

	// ��� �������� ��� ��������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPos", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPos));
	ReleaseCOM(VSBlob);

	// ��� �������� � ���������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosTex", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosTex));
	D3D11_INPUT_ELEMENT_DESC elementDescPosTex[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosTex, ARRAYSIZE(elementDescPosTex), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Tex2));
	ReleaseCOM(VSBlob);

	// ��� ������ ����
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSSpriteInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSSpriteInstance));
	// ��� ������ ����
	D3D11_INPUT_ELEMENT_DESC elementDescSpriteInstance[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // �������� ���������
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(XMFLOAT3), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // �������� ��������
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescSpriteInstance, ARRAYSIZE(elementDescSpriteInstance), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutSpriteInstance));
	ReleaseCOM(VSBlob);

	// ��� ������� ��� ��������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSPosNor", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSPosNor));
	D3D11_INPUT_ELEMENT_DESC elementDescPosNor[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescPosNor, ARRAYSIZE(elementDescPosNor), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutPos3Nor3));
	ReleaseCOM(VSBlob);

	// ��� ������� � ���������
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

	// ��� �������� ����������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSTerrain", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSTerrain));
	D3D11_INPUT_ELEMENT_DESC elementDescTerrain[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // ��� normal mapping
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3 + sizeof(XMFLOAT2), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 3 + sizeof(XMFLOAT2)* 2, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // �����
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescTerrain, ARRAYSIZE(elementDescTerrain), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutTerrain));
	ReleaseCOM(VSBlob);

	// ��� ������������ �������
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

	// ���� ��� ������������ �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSShadow", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSShadow));
	ReleaseCOM(VSBlob);

	// ��� ������� �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSInstance));
	D3D11_INPUT_ELEMENT_DESC elementDescInstance[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3)* 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // ��������
		{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32_FLOAT, 1, sizeof(XMFLOAT3), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // ��������
		{ "INSTANCECOLOR", 1, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(XMFLOAT3)+sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 }, // ���������������
	};
	HR(Mediator::pDev->CreateInputLayout(elementDescInstance, ARRAYSIZE(elementDescInstance), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &pInputLayoutInstance));
	ReleaseCOM(VSBlob);

	// ���� ��� ������� �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "VSShadowInstance", profilePointerVS, shaderFlags, 0, NULL, &VSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &pVSShadowInstance));
	ReleaseCOM(VSBlob);

	// ��� �����
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

// �������� ����� �����
bool Shader::CreateShadowMap() {

	// �������� 2D
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

	// ����� �����
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(Mediator::pDev->CreateDepthStencilView(pShadowTexture, &dsvDesc, &pShadowMapDepthView));

	// ������ ��� ����� �����
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	HR(Mediator::pDev->CreateShaderResourceView(pShadowTexture, &srvDesc, &pShadowResourceView));

	ReleaseCOM(pShadowTexture);

	return true;

}

// ������� ���������� �������
bool Shader::CreatePixelShaders() {

	UINT shaderFlags = 0;
#if defined (DEBUG) | defined (_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // DEBUG

	ID3D10Blob* pErrorMsg = NULL;

	// ����� ������ ��������
	char D10profilePS[] = "ps_4_0"; char D11profilePS[] = "ps_5_0";
	char* profilePointerVS = NULL; char* profilePointerPS = NULL;
	profilePointerPS = Mediator::featureLevel >= D3D_FEATURE_LEVEL_11_0 ? D11profilePS : D10profilePS;

	// ��� ������� ��� ��������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPos", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPos));
	ReleaseCOM(PSBlob);

	// ��� ������� � ���������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosTex", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosTex));
	ReleaseCOM(PSBlob);

	// ��� �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSFont", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSFont));
	ReleaseCOM(PSBlob);

	// ��� �������� �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSSolid", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSSolidModel));
	ReleaseCOM(PSBlob);

	// ��� ������ ��� ��������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosNor", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosNor));
	ReleaseCOM(PSBlob);

	// ��� ������ � ���������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosNorTex", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosNorTex));
	ReleaseCOM(PSBlob);

	// ��� ������ � ��������� � ����������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSClipping", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSClipping));
	ReleaseCOM(PSBlob);

	// ��� �������� ����������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSTerrain", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSTerrain));
	ReleaseCOM(PSBlob);

	// ��� �������� ���������� (��� ����)
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSTerrainNoShadow", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSTerrainNoShadow));
	ReleaseCOM(PSBlob);

	// ������ �������
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSEmpty", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSEmpty));
	ReleaseCOM(PSBlob);

	// ��� �����
	HR(D3DX11CompileFromFile(L"resources//sh11.fx", NULL, NULL, "PSPosCol", profilePointerPS, shaderFlags, 0, NULL, &PSBlob, &pErrorMsg, NULL));
	CompileError(pErrorMsg);
	HR(Mediator::pDev->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &pPSPosCol));
	ReleaseCOM(PSBlob);

	return true;

}

// ������� ����������� ������
bool Shader::CreateConstBuffers() {

	D3D11_BUFFER_DESC cbbd = { 0 };
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_MAPPED_SUBRESOURCE mappedData;

	// ������������ �������������
	cbbd.ByteWidth = sizeof(CbufFrame1);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufLightVector));

	// ��� ������������ �������
	cbbd.ByteWidth = sizeof(CbufAnimModel);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufAnimModel));

	// ��� ������
	cbbd.ByteWidth = sizeof(CbufReset);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufReset));

	// ��� ��������
	cbbd.ByteWidth = sizeof(CbufObject);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufObject));

	// ��� �������
	cbbd.ByteWidth = sizeof(CbufFont);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufFont));

	// ��� ������� �����
	cbbd.ByteWidth = sizeof(CbufFrame2);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufFrame));

	// ��� ����� �����
	cbbd.ByteWidth = sizeof(CbufFrame3);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufLight));

	// ������ ���������
	cbbd.ByteWidth = sizeof(CbufOnce);
	HR(Mediator::pDev->CreateBuffer(&cbbd, NULL, &cbufOnce));
	HR(Mediator::pDevContext->Map(cbufOnce, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufOnce* cbuf = reinterpret_cast<CbufOnce*>(mappedData.pData);
	cbuf->shadowMapDimentons = XMFLOAT4((float)Const::shadowMapSize.x, (float)Const::shadowMapSize.y, 0.0f, 0.0f);
	Mediator::pDevContext->Unmap(cbufOnce, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_ONCE, 1, &cbufOnce);

	return true;

}

// ������� blend state, sampler state � render state
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

	// �������� � ��������� RendesStates
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(rastDesc));
	Const::backCulling ? rastDesc.CullMode = D3D11_CULL_BACK : rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	HR(Mediator::pDev->CreateRasterizerState(&rastDesc, &pRastStandard));
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	HR(Mediator::pDev->CreateRasterizerState(&rastDesc, &pRastWireframe));
	if (Const::wireFrame) Mediator::pDevContext->RSSetState(pRastWireframe);
	else Mediator::pDevContext->RSSetState(pRastStandard);

	{// ��������� SamplerStates

		// �����������
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

		// ��� �����
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

// ���������� ������� �������
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

// ���������� ��������� ������
bool Shader::SetCamBuffer(CXMMATRIX viewProjMatrix, XMFLOAT4 camPos, XMFLOAT4 camFocus) {

	// �������� ������ � ������
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

// ���������� ���� ������
bool Shader::SetFontBuffer(XMFLOAT4* fontColor) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufFont, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufFont* cbuf = reinterpret_cast<CbufFont*>(mappedData.pData);
	cbuf->fontColor = *fontColor;
	Mediator::pDevContext->Unmap(cbufFont, 0);
	Mediator::pDevContext->PSSetConstantBuffers(SR_FONT, 1, &cbufFont);

	return true;

}

// ���������� ��������� ������
bool Shader::SetResetBuffer(CXMMATRIX orthoMatrix) {

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(Mediator::pDevContext->Map(cbufReset, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CbufReset* cbuf = reinterpret_cast<CbufReset*>(mappedData.pData);

	XMStoreFloat4x4(&cbuf->gOrthoMatrix, XMMatrixTranspose(orthoMatrix));

	Mediator::pDevContext->Unmap(cbufReset, 0);
	Mediator::pDevContext->VSSetConstantBuffers(SR_CAMERA, 1, &cbufReset);

	return true;

}

// ����������/����� Alpha Blending
bool Shader::SetAlphaBlending(bool enable) {

	if (!Const::alpha) return true;

	static bool prevState = !enable;
	if (prevState == enable) return true; // ������ �� ����������

	// ���������� ����� ���������
	const float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (enable) Mediator::pDevContext->OMSetBlendState(pBlendState, blendFactor, 0xffffff);
	else Mediator::pDevContext->OMSetBlendState(NULL, blendFactor, 0xffffff);
	prevState = enable;

	return true;

}

// ����������/����� wireframe
bool Shader::SetWireframe(bool wireframe) {

	static bool prevWireframe = Const::wireFrame;
	if (wireframe == prevWireframe) return true; // ������ �� ����������

	// ���������� ����� ���������
	if (wireframe) Mediator::pDevContext->RSSetState(pRastWireframe);
	else Mediator::pDevContext->RSSetState(pRastStandard);
	prevWireframe = wireframe;

	return true;

}

// ����������
Shader::~Shader() {

	// ����������� ������
	ReleaseCOM(cbufLightVector);
	ReleaseCOM(cbufAnimModel);
	ReleaseCOM(cbufReset);
	ReleaseCOM(cbufObject);
	ReleaseCOM(cbufFont);
	ReleaseCOM(cbufFrame);
	ReleaseCOM(cbufLight);
	ReleaseCOM(cbufOnce);

	// �������
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

	// ��������� ������
	ReleaseCOM(pInputLayoutPos3Tex2);
	ReleaseCOM(pInputLayoutPos3Nor3);
	ReleaseCOM(pInputLayoutPos3Nor3Tex2);
	ReleaseCOM(pInputLayoutTerrain);
	ReleaseCOM(pInputLayoutPos3Nor3Tex2W1Bone2);
	ReleaseCOM(pInputLayoutSpriteInstance);
	ReleaseCOM(pInputLayoutInstance);
	ReleaseCOM(pInputLayoutPosCol);

	// ������
	ReleaseCOM(pBlendState);
	ReleaseCOM(pRastStandard);
	ReleaseCOM(pRastWireframe);
	ReleaseCOM(pShadowResourceView);
	ReleaseCOM(pShadowMapDepthView);

}
