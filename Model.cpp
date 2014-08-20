#include "Model.h"
#include "Mediator.h"

// ����������� �������
Model::Model(wchar_t* textureFilePath, wchar_t* binFilePath, myXMFLOAT3 position, bool* result) {

	// ������������� ����������
	if (binFilePath) {
		loaded = true;
	}
	if (textureFilePath) {
		textured = true;
		if (FAILED(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, textureFilePath, NULL, NULL, &pSRtexture, NULL))) {
			BRR(Mediator::errors->Push(textureFilePath));
			*result = false;
			return;
		}
	}
	objPosition = position;

	// ��������(��������) ������� ������ � ��������
	if (loaded) {

		// ��������� �� �����
		BRR(LoadModelFromFile(binFilePath));

	}
	else {

		// �������� �� ���������
		BRR(CreateVertexBuffer()); // ������� ����� ������ �� ���������
		BRR(CreateIndexBuffer()); // ������� ����� �������� �� ���������

	}

	*result = true;

}

// ������� ����� ������ �� ���������
bool Model::CreateVertexBuffer() {

	UINT vertexBufferByteWidth = 0;
	void* vertexBufferPointer = NULL;
	const float boxSize = 0.5f;

	XMFLOAT3 normalUp = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 normalDown = { 0.0f, -1.0f, 0.0f };
	XMFLOAT3 normalRight = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 normalLeft = { -1.0f, 0.0f, 0.0f };
	XMFLOAT3 normalForward = { 0.0f, 0.0f, -1.0f };
	XMFLOAT3 normalBack = { 0.0f, 0.0f, 1.0f };

	// ������ � ����������
	VertPos3Nor3Tex2 vertexBufferTextured[] = {

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalForward, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalForward, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalForward, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalForward, XMFLOAT2{ 1.0f, 0.0f } },

		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalBack, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalBack, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalBack, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalBack, XMFLOAT2{ 1.0f, 0.0f } },

		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalRight, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalRight, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalRight, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalRight, XMFLOAT2{ 1.0f, 0.0f } },

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalLeft, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalLeft, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalLeft, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalLeft, XMFLOAT2{ 1.0f, 0.0f } },

		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalUp, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalUp, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalUp, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalUp, XMFLOAT2{ 1.0f, 0.0f } },

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalDown, XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalDown, XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalDown, XMFLOAT2{ 1.0f, 1.0f } },
		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalDown, XMFLOAT2{ 1.0f, 0.0f } },

	};

	// ������ ��� �������
	VertPos3Nor3 vertexBufferNotTextured[] = {

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalForward },
		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalForward },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalForward },
		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalForward },

		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalBack },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalBack },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalBack },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalBack },

		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalRight },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalRight },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalRight },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalRight },

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalLeft },
		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalLeft },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalLeft },
		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalLeft },

		{ XMFLOAT3(-boxSize, boxSize, -boxSize), normalUp },
		{ XMFLOAT3(-boxSize, boxSize, boxSize), normalUp },
		{ XMFLOAT3(boxSize, boxSize, boxSize), normalUp },
		{ XMFLOAT3(boxSize, boxSize, -boxSize), normalUp },

		{ XMFLOAT3(-boxSize, -boxSize, -boxSize), normalDown },
		{ XMFLOAT3(-boxSize, -boxSize, boxSize), normalDown },
		{ XMFLOAT3(boxSize, -boxSize, boxSize), normalDown },
		{ XMFLOAT3(boxSize, -boxSize, -boxSize), normalDown },

	};

	if (textured) {
		vertexBufferByteWidth = sizeof(vertexBufferTextured);
		vertexBufferPointer = vertexBufferTextured;
	}
	else {
		vertexBufferByteWidth = sizeof(vertexBufferNotTextured);
		vertexBufferPointer = vertexBufferNotTextured;
	}

	// ��������� ������ ������
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = vertexBufferByteWidth;
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ������ ������
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = vertexBufferPointer;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	return true;

}

// ������� ����� �������� �� ���������
bool Model::CreateIndexBuffer() {

	// ���������� ������ ��������
	UINT indices[] = {
		0, 1, 2, 3, 0, 2,
		4, 6, 5, 7, 6, 4,
		8, 10, 9, 11, 10, 8,
		12, 13, 14, 15, 12, 14,
		16, 17, 18, 19, 16, 18,
		20, 22, 21, 23, 22, 20,
	};
	indicesAmount = ARRAYSIZE(indices);

	// ��������� ������ ��������
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT) * indicesAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ����� ��������
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indices;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	return true;

}

// ��������� ������ �� �����
bool Model::LoadModelFromFile(wchar_t* binFilePath) {

	// �������� �����
	std::ifstream fin;
	fin.open(binFilePath, std::ios_base::binary);
	BR(fin.is_open());

	// ������ ���������
	UINT vertexAmount = 0;
	fin.read((char*)&vertexAmount, sizeof(UINT));
	fin.read((char*)&indicesAmount, sizeof(UINT));

	// ������ ������ ������
	void* vertexBuffer = NULL;
	if (textured) {

		// ������ ������ ������ ��� ���������� ������
		VertPos3Nor3Tex2* vertexBufferTextured = new VertPos3Nor3Tex2[vertexAmount];
		fin.read((char*)vertexBufferTextured, sizeof(VertPos3Nor3Tex2)* vertexAmount);
		vertexBuffer = vertexBufferTextured;

	}
	else {

		// ������ ������ ������ ��� ������ ��� ��������
		VertPos3Nor3* vertexBufferNoTexture = new VertPos3Nor3[vertexAmount];
		VertPos3Nor3* pointer = vertexBufferNoTexture;
		for (UINT i(0); i < vertexAmount; i++) {
			VertPos3Nor3 curVertex;
			XMFLOAT2 texture;
			fin.read((char*)&curVertex.position, sizeof(XMFLOAT3));
			fin.read((char*)&curVertex.normal, sizeof(XMFLOAT3));
			fin.read((char*)&texture, sizeof(XMFLOAT2));
			memcpy(pointer, &curVertex, sizeof(VertPos3Nor3));
			pointer++;
		}
		vertexBuffer = vertexBufferNoTexture;

	}

	// ������ ������ ��������
	UINT* indexBuffer = new UINT[indicesAmount];
	fin.read((char*)indexBuffer, sizeof(UINT)* indicesAmount);

	// ��������� ������ ������
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = textured ? sizeof(VertPos3Nor3Tex2)* vertexAmount : sizeof(VertPos3Nor3) * vertexAmount;
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ������ ������
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = vertexBuffer;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	// ��������� ������ ��������
	D3D11_BUFFER_DESC bufferIndDesc = { 0 };
	bufferIndDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferIndDesc.ByteWidth = sizeof(UINT)* indicesAmount;
	bufferIndDesc.CPUAccessFlags = 0;
	bufferIndDesc.MiscFlags = 0;
	bufferIndDesc.StructureByteStride = 0;
	bufferIndDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ����� ��������
	D3D11_SUBRESOURCE_DATA subIndData = { 0 };
	subIndData.pSysMem = indexBuffer;
	HR(Mediator::pDev->CreateBuffer(&bufferIndDesc, &subIndData, &pIndexBuffer));

	// �������� ��������
	ReleaseNULLS(vertexBuffer);
	ReleaseNULLS(indexBuffer);
	fin.close();

	return true;

}

// ��������� � ��������� � �����
bool Model::Draw(CXMMATRIX boneMatrix) {

	// ���������� ���������
	UINT vertexStride = textured ? sizeof(VertPos3Nor3Tex2) : sizeof(VertPos3Nor3);
	UINT vertexOffset = 0;
	myXMFLOAT3 positionJP = objPosition - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);

	// ���������� ���������
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (textured) {
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
		Mediator::pDevContext->VSSetShader(Mediator::pVSPosNorTex, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPosNorTex, NULL, 0);
	}
	else {
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
		Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPosNor, NULL, 0);
	}

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(boneMatrix * W_JP);

	// ���������
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// ��������� �������
bool Model::Draw() {

	// ���������� ���������
	myXMFLOAT3 positionJP = objPosition - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	UINT vertexStride = textured ? sizeof(VertPos3Nor3Tex2) : sizeof(VertPos3Nor3);
	UINT vertexOffset = 0;

	// ���������� ���������
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (textured) {
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
	}
	else {
		Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
	}
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSPosNor, NULL, 0);

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(W_JP);

	// ���������
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// ���������� �������
Model::~Model() {

	ReleaseCOM(pSRtexture);
	ReleaseCOM(pIndexBuffer);
	ReleaseCOM(pVertexBuffer);

}