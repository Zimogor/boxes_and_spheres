#include "SimpleVectorModel.h"
#include "Mediator.h"

// конструктор
SimpleVectorModel::SimpleVectorModel(bool* result) : Model(NULL, NULL, myXMFLOAT3(), result) {

};

// рисование экземпляра
bool PerSimpleVectorModel::Draw(UINT indicesAmount) {

	// координаты экземпляра
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);
	XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);

	// обновление константного буфера шейдера
	Mediator::shader->SetObjectBuffer(S * W_JP);

	// рисование
	Mediator::pDevContext->DrawIndexed(indicesAmount, 0, 0);

	return true;

}

// рисование
bool SimpleVectorModel::Draw() {

	// общая настройка
	UINT vertexStride = sizeof(VertPos3Nor3);
	UINT vertexOffset = 0;
	Mediator::pDevContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &vertexStride, &vertexOffset);
	Mediator::pDevContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Mediator::pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Mediator::pDevContext->IASetInputLayout(Mediator::pInputLayoutPos3Nor3Tex2);
	Mediator::pDevContext->VSSetShader(Mediator::pVSPosNor, NULL, 0);
	Mediator::pDevContext->PSSetShader(Mediator::pPSSolid, NULL, 0);

	// для каждого экземпляра
	for (auto iter = instances.begin(); iter != instances.end(); iter++) {
		BR(iter->Draw(indicesAmount));
	}

	// для каждого экземпляра-подёнка
	for (auto iter = flyInstances.begin(); iter != flyInstances.end(); iter++) {
		BR(iter->Draw(indicesAmount));
	}

	flyInstances.clear();

	return true;

}

// создать экземпляр
bool SimpleVectorModel::CreateInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	instances.push_back(PerSimpleVectorModel(position, scale, rotation));

	return true;

}

// показать экземпляр-подёнок
bool SimpleVectorModel::ShowFlyInstance(myXMFLOAT3 position, myXMFLOAT3 scale, float rotation) {

	flyInstances.push_back(PerSimpleVectorModel(position, scale, rotation));

	return true;

}