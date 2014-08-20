#ifndef NUMS_H
#define NUMS_H

#include "Utilities.h"

// ������ �����
template <typename TYPE>
struct NumXY {

	NumXY() :x(0), y(0) {};
	NumXY(TYPE x, TYPE y) :x(x), y(y) {};
	TYPE x, y;

	// ���������� �������� ����������
	template <typename TYPE> // ��������
	friend NumXY<TYPE> operator+ (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXY<TYPE> operator- (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXY<TYPE> operator* (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // �������
	friend NumXY<TYPE> operator* (const NumXY<TYPE>& first, const NumXY<TYPE>& second);

	// ���������� ��������� ����������
	template <typename TYPE1, typename TYPE2> // ��������
	friend NumXY<TYPE1> operator+ (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXY<TYPE1> operator- (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXY<TYPE1> operator* (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // �������
	friend NumXY<TYPE1> operator/ (const NumXY<TYPE1>& other, const TYPE2& scalar);

	// ���������� ������� ���������� � ��������
	template <typename TYPE> // ��������
	friend NumXY<TYPE>& operator+= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXY<TYPE>& operator-= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXY<TYPE>& operator*= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // �������
	friend NumXY<TYPE>& operator/= (NumXY<TYPE>& first, const NumXY<TYPE>& second);

	// ���������� ������� ���������� �� ��������
	template <typename TYPE1, typename TYPE2> // ��������
	friend NumXY<TYPE1>& operator+= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXY<TYPE1>& operator-= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXY<TYPE1>& operator*= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // �������
	friend NumXY<TYPE1>& operator/= (NumXY<TYPE1>& other, const TYPE2& scalar);

	// ����� � �������
	template <typename TYPE>
	friend std::wostringstream& operator<< (std::wostringstream& out, const NumXY<TYPE>& other);

	// ���������
	template <typename TYPE>
	friend bool operator== (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE>
	friend bool operator!= (const NumXY<TYPE>& first, const NumXY<TYPE>& second);

};

// ��������
template <typename TYPE>
NumXY<TYPE> operator+ (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return NumXY<TYPE>(first.x + second.x, first.y + second.y);

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1> operator+ (const NumXY<TYPE1>& other, const TYPE2& scalar) {

	return NumXY<TYPE1>(other.x + scalar, other.y + scalar);

}
template <typename TYPE>
NumXY<TYPE>& operator+= (NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	first.x += second.x;
	first.y += second.y;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1>& operator+= (NumXY<TYPE1>& other, const TYPE2& scalar) {

	other.x += scalar;
	other.y += scalar;
	return other;

}

// ���������
template <typename TYPE>
NumXY<TYPE> operator- (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return NumXY<TYPE>(first.x - second.x, first.y - second.y);

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1> operator- (const NumXY<TYPE1>& other, const TYPE2& scalar) {

	return NumXY<TYPE1>(other.x - scalar, other.y - scalar);

}
template <typename TYPE>
NumXY<TYPE>& operator-= (NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	first.x -= second.x;
	first.y -= second.y;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1>& operator-= (NumXY<TYPE1>& other, const TYPE2& scalar) {

	other.x -= scalar;
	other.y -= scalar;
	return other;

}

// ���������
template <typename TYPE>
NumXY<TYPE> operator* (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return NumXY<TYPE>(first.x * second.x, first.y * second.y);

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1> operator* (const NumXY<TYPE1>& other, const TYPE2& scalar) {

	return NumXY<TYPE1>(other.x * scalar, other.y * scalar);

}
template <typename TYPE>
NumXY<TYPE>& operator*= (NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	first.x *= second.x;
	first.y *= second.y;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1>& operator*= (NumXY<TYPE1>& other, const TYPE2& scalar) {

	other.x *= scalar;
	other.y *= scalar;
	return other;

}

// �������
template <typename TYPE>
NumXY<TYPE> operator/ (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return NumXY<TYPE>(first.x / second.x, first.y / second.y);

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1> operator/ (const NumXY<TYPE1>& other, const TYPE2& scalar) {

	return NumXY<TYPE1>(other.x / scalar, other.y / scalar);

}
template <typename TYPE>
NumXY<TYPE>& operator/= (NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	first.x /= second.x;
	first.y /= second.y;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXY<TYPE1>& operator/= (NumXY<TYPE1>& other, const TYPE2& scalar) {

	other.x /= scalar;
	other.y /= scalar;
	return other;

}

// ����� � �������
template <typename TYPE>
std::wostringstream& operator<< (std::wostringstream& out, const NumXY<TYPE>& other) {

	out << other.x << ' ' << other.y;

	return out;

}

// ���������
template <typename TYPE>
bool operator== (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return (first.x == second.x && first.y == second.y);

};
template <typename TYPE>
bool operator!= (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return (first.x != second.x || first.y != second.y);

};


// ������� �����
template <typename TYPE>
struct NumXYZ {

	NumXYZ() :x(0), y(0), z(0) {};
	NumXYZ(TYPE x, TYPE y, TYPE z) :x(x), y(y), z(z) {};
	NumXYZ(TYPE x) :x(x), y(x), z(x) {};
	TYPE x, y, z;

	// ���������� �������� ����������
	template <typename TYPE> // ��������
	friend NumXYZ<TYPE> operator+ (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXYZ<TYPE> operator- (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXYZ<TYPE> operator* (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // �������
	friend NumXYZ<TYPE> operator* (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

	// ���������� �������� ���������� �� ��������
	template <typename TYPE1, typename TYPE2> // ��������
	friend NumXYZ<TYPE1> operator+ (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXYZ<TYPE1> operator- (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXYZ<TYPE1> operator* (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // �������
	friend NumXYZ<TYPE1> operator/ (const NumXYZ<TYPE1>& other, const TYPE2& scalar);

	// ���������� ������� ���������� � ��������
	template <typename TYPE> // ��������
	friend NumXYZ<TYPE>& operator+= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXYZ<TYPE>& operator-= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // ���������
	friend NumXYZ<TYPE>& operator*= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // �������
	friend NumXYZ<TYPE>& operator/= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

	// ���������� ������� ���������� �� ��������
	template <typename TYPE1, typename TYPE2> // ��������
	friend NumXYZ<TYPE1>& operator+= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXYZ<TYPE1>& operator-= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // ���������
	friend NumXYZ<TYPE1>& operator*= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // �������
	friend NumXYZ<TYPE1>& operator/= (NumXYZ<TYPE1>& other, const TYPE2& scalar);

	// ����� � �������
	template <typename TYPE>
	friend std::wostringstream& operator<< (std::wostringstream& out, const NumXYZ<TYPE>& other);

	// ���������
	template <typename TYPE>
	friend bool operator== (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE>
	friend bool operator!= (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

};

// ��������
template <typename TYPE>
NumXYZ<TYPE> operator+ (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return NumXYZ<TYPE>(first.x + second.x, first.y + second.y, first.z + second.z);

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1> operator+ (const NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	return NumXYZ<TYPE1>(other.x + scalar, other.y + scalar, other.z + scalar);

}
template <typename TYPE>
NumXYZ<TYPE>& operator+= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1>& operator+= (NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	other.x += scalar;
	other.y += scalar;
	other.z += scalar;
	return other;

}

// ���������
template <typename TYPE>
NumXYZ<TYPE> operator- (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return NumXYZ<TYPE>(first.x - second.x, first.y - second.y, first.z - second.z);

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1> operator- (const NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	return NumXYZ<TYPE1>(other.x - scalar, other.y - scalar, other.z - scalar);

}
template <typename TYPE>
NumXYZ<TYPE>& operator-= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1>& operator-= (NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	other.x -= scalar;
	other.y -= scalar;
	other.z -= scalar;
	return other;

}

// ���������
template <typename TYPE>
NumXYZ<TYPE> operator* (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return NumXYZ<TYPE>(first.x * second.x, first.y * second.y, first.z * second.z);

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1> operator* (const NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	return NumXYZ<TYPE1>(other.x * scalar, other.y * scalar, other.z * scalar);

}
template <typename TYPE>
NumXYZ<TYPE>& operator*= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1>& operator*= (NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	other.x *= scalar;
	other.y *= scalar;
	other.z *= scalar;
	return other;

}

// �������
template <typename TYPE>
NumXYZ<TYPE> operator/ (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return NumXYZ<TYPE>(first.x / second.x, first.y / second.y, first.z / second.z);

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1> operator/ (const NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	return NumXYZ<TYPE1>(other.x / scalar, other.y / scalar, other.z / scalar);

}
template <typename TYPE>
NumXYZ<TYPE>& operator/= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	return first;

}
template <typename TYPE1, typename TYPE2>
NumXYZ<TYPE1>& operator/= (NumXYZ<TYPE1>& other, const TYPE2& scalar) {

	other.x /= scalar;
	other.y /= scalar;
	other.z /= scalar;
	return other;

}

// ����� � �������
template <typename TYPE>
std::wostringstream& operator<< (std::wostringstream& out, const NumXYZ<TYPE>& other) {

	out << other.x << ' ' << other.y << ' ' << other.z;

	return out;

}

// ���������
template <typename TYPE>
bool operator== (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return (first.x == second.x && first.y == second.y && first.z == second.z);

};
template <typename TYPE>
bool operator!= (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return (first.x != second.x || first.y != second.y || first.z != second.z);

};

// ������� ���������
struct myXMFLOAT3 : public XMFLOAT3 {

	myXMFLOAT3() : XMFLOAT3(0, 0, 0) {};
	myXMFLOAT3(float value) : XMFLOAT3(value, value, value) {};
	myXMFLOAT3(float x, float y, float z) : XMFLOAT3(x, y, z) {};

	// ���������� �������� ����������
	friend myXMFLOAT3 operator+ (const myXMFLOAT3& first, const myXMFLOAT3& second); // ��������
	friend myXMFLOAT3 operator- (const myXMFLOAT3& first, const myXMFLOAT3& second); // ���������
	friend myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second); // ���������
	friend myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second); // �������

	// ���������� ������� ���������� � ��������
	friend myXMFLOAT3& operator+= (myXMFLOAT3& first, const myXMFLOAT3& second); // ��������
	friend myXMFLOAT3& operator-= (myXMFLOAT3& first, const myXMFLOAT3& second); // ���������
	friend myXMFLOAT3& operator*= (myXMFLOAT3& first, const myXMFLOAT3& second); // ���������
	friend myXMFLOAT3& operator/= (myXMFLOAT3& first, const myXMFLOAT3& second); // �������

};

#endif // NUMS_H