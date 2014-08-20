#include "BoundingBox.h"
#include "Mediator.h"

// рисование
bool BoundingBox::Draw(myXMFLOAT3 position) {

	// координаты экземпл€ра
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);

	// масштаб
	XMMATRIX S = XMMatrixScaling(radius, 1.0f, radius);

	// обновление константного буфера шейдера
	Mediator::shader->SetObjectBuffer(S * W_JP);

	// рисование
	Mediator::pDevContext->DrawIndexed(Mediator::helper->cylinderIndicesAmount, 0, 0);

	return true;

}
