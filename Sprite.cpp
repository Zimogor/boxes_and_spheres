#include "Sprite.h"
#include "Mediator.h"

// ����������� �������
Sprite::Sprite(wchar_t* filePath, XMFLOAT3 position, NumXY<float> dimentions, SPRITE_ALIGN spriteAlign, bool* result) : position(position), dimentions(dimentions), spriteAlign(spriteAlign) {

	if (filePath) {
		textured = true;
		HRR(D3DX11CreateShaderResourceViewFromFile(Mediator::pDev, filePath, NULL, NULL, &pSRtexture, NULL));
	}

	// ������� ����� ������
	BRR(CreateVertexBuffer());

	// �������� ������� �����������
	BRR(UpdateMatrixTranslation(*Mediator::winDimentions));

	*result = true;

}

// ������� ����� ������
bool Sprite::CreateVertexBuffer() {

	// ������ � ���������
	VertPos3Tex2 vertexBufferTex[] = {

		{ XMFLOAT3(0.0f, 0.0f, position.z), XMFLOAT2{ 0.0f, 0.0f } },
		{ XMFLOAT3(dimentions.x, 0.0f, position.z), XMFLOAT2{ 1.0f, 0.0f } },
		{ XMFLOAT3(0.0f, dimentions.y, position.z), XMFLOAT2{ 0.0f, 1.0f } },
		{ XMFLOAT3(dimentions.x, dimentions.y, position.z), XMFLOAT2{ 1.0f, 1.0f } },

	};

	// ��������� ������ ������
	D3D11_BUFFER_DESC bufferVertDesc = { 0 };
	bufferVertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferVertDesc.ByteWidth = sizeof(vertexBufferTex);
	bufferVertDesc.CPUAccessFlags = 0;
	bufferVertDesc.MiscFlags = 0;
	bufferVertDesc.StructureByteStride = 0;
	bufferVertDesc.Usage = D3D11_USAGE_IMMUTABLE;

	// �������� ������ ������
	D3D11_SUBRESOURCE_DATA subVertData = { 0 };
	subVertData.pSysMem = (void*)vertexBufferTex;
	HR(Mediator::pDev->CreateBuffer(&bufferVertDesc, &subVertData, &pVertexBuffer));

	return true;

}

// �������� ������� �����������
bool Sprite::UpdateMatrixTranslation(NumXY<UINT> winDimentions) {

	switch (spriteAlign) {
	case SA_TOPRIGHT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(winDimentions.x + position.x - dimentions.x, position.y, 0.0f));
		break;
	}
	case SA_BOTTOMLEFT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(position.x, winDimentions.y + position.y - dimentions.y, 0.0f));
		break;
	}
	case SA_BOTTOMRIGHT: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(winDimentions.x + position.x - dimentions.x, winDimentions.y + position.y - dimentions.y, 0.0f));
		break;
	}
	case SA_CENTER: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation((winDimentions.x - (UINT)dimentions.x) / 2 + position.x, (winDimentions.y - (UINT)dimentions.y) / 2 + position.y, 0.0f));
		break;
	}
	default: {
		XMStoreFloat4x4(&translationF, XMMatrixTranslation(position.x, position.y, 0.0f));
		break;
	}
	}

	return true;

}

// ���������
bool Sprite::Draw() {

	// ���������� ���������
	UINT vertexStride = 0;
	UINT vertexOffset = 0;
	vertexStride = sizeof(VertPos3Tex2);

	// ���������� ���������
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Tex2);
	if (textured) {
		Mediator::pDevContext->PSSetShaderResources(TEXTURE_BUFFER, 1, &pSRtexture);
		Mediator::pDevContext->VSSetShader(Mediator::pVSPosTex, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPosTex, NULL, 0);
	}
	else {
		Mediator::pDevContext->VSSetShader(Mediator::pVSPos, NULL, 0);
		Mediator::pDevContext->PSSetShader(Mediator::pPSPos, NULL, 0);
	}

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(&translationF);

	// ���������
	Mediator::pDevContext->Draw(vertexAmount, 0);

	return true;

}

// ����� �������
bool Sprite::Reset() {

	// ����� ������� �����������
	BR(UpdateMatrixTranslation(*Mediator::winDimentions));

	return true;

}

// ���������� �������
Sprite::~Sprite() {

	ReleaseCOM(pSRtexture);
	ReleaseCOM(pVertexBuffer);

};