#include "BoundingBox.h"
#include "Mediator.h"

// ���������
bool BoundingBox::Draw(myXMFLOAT3 position) {

	// ���������� ����������
	myXMFLOAT3 positionJP = position - Mediator::camera->GetPosition();
	XMMATRIX W_JP = XMMatrixTranslation(positionJP.x, positionJP.y, positionJP.z);

	// �������
	XMMATRIX S = XMMatrixScaling(radius, 1.0f, radius);

	// ���������� ������������ ������ �������
	Mediator::shader->SetObjectBuffer(S * W_JP);

	// ���������
	Mediator::pDevContext->DrawIndexed(Mediator::helper->cylinderIndicesAmount, 0, 0);

	return true;

}
