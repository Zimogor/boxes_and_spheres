#include "Nums.h"

// ���������� �������� ����������
myXMFLOAT3 operator+ (const myXMFLOAT3& first, const myXMFLOAT3& second) { // ��������
	return myXMFLOAT3(first.x + second.x, first.y + second.y, first.z + second.z);
}
myXMFLOAT3 operator- (const myXMFLOAT3& first, const myXMFLOAT3& second) { // ���������
	return myXMFLOAT3(first.x - second.x, first.y - second.y, first.z - second.z);
}
myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second) { // ���������
	return myXMFLOAT3(first.x * second.x, first.y * second.y, first.z * second.z);
}
myXMFLOAT3 operator/ (const myXMFLOAT3& first, const myXMFLOAT3& second) { // �������
	return myXMFLOAT3(first.x / second.x, first.y / second.y, first.z / second.z);
}

// ���������� ������� ���������� � ��������
myXMFLOAT3& operator+= (myXMFLOAT3& first, const myXMFLOAT3& second) { // ��������
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	return first;
}
myXMFLOAT3& operator-= (myXMFLOAT3& first, const myXMFLOAT3& second) { // ���������
	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	return first;
}
myXMFLOAT3& operator*= (myXMFLOAT3& first, const myXMFLOAT3& second) { // ���������
	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	return first;
}
myXMFLOAT3& operator/= (myXMFLOAT3& first, const myXMFLOAT3& second) { // �������
	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	return first;
}