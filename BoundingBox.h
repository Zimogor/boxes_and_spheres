#pragma once

#include "Utilities.h"
#include "Structures.h"

// bounding box
class BoundingBox {

public:

	// ������������
	BoundingBox() : position(NumXY<float>()), radius(0), active(true) {};
	BoundingBox(NumXY<float> position, float radius) : position(position), radius(radius), active(true) {};

	// �������
	bool Draw(myXMFLOAT3 position); // ���������
	void SetPosition(NumXY<float> position) { this->position = position; }; // �������� boundingbox

	// ����������
	NumXY<float> position; // ���������� ������
	float radius; // ������ ����������
	bool active; // boundingbox �������

};