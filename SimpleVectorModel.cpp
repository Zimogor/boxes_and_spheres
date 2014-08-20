#include "SimpleVectorModel.h"
#include "Mediator.h"

// �����������
SimpleVectorModel::SimpleVectorModel(bool* result) : Model(NULL, NULL, myXMFLOAT3(), result) {

};

// ��������� ����������
bool PerSimpleVectorModel::Draw(UINT indicesAmount) {

	// ���������� ����������
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(S * W_JP);

	// ���������
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// ���������
bool SimpleVectorModel::Draw() {

	// ����� ���������
	UINT vertexStride = sizeof(VertPos3Nor3);
	UINT vertexOffset = 0;
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSSolid, NULL, 0);

	// ��� ������� ����������
	for (auto iter = instances.begin(); iter != instances.end(); iter++) {
		BR(iter->Draw(indicesAmount));
	}

	// ��� ������� ����������-������
	for (auto iter = flyInstances.begin(); iter != flyInstances.end(); iter++) {
		BR(iter->Draw(indicesAmount));
	}

	flyInstances.clear();

	return true;

}

// ������� ���������
bool SimpleVectorModel::CreateInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	instances.push_back(PerSimpleVectorModel(position, scale, rotation));

	return true;

}

// �������� ���������-������
bool SimpleVectorModel::ShowFlyInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	flyInstances.push_back(PerSimpleVectorModel(position, scale, rotation));

	return true;

}