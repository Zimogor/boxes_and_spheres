#ifndef NUMS_H
#define NUMS_H

#include "Utilities.h"

// парное число
template <typename TYPE>
struct NumXY {

	NumXY() :x(0), y(0) {};
	NumXY(TYPE x, TYPE y) :x(x), y(y) {};
	TYPE x, y;

	// перегрузка бинарных операторов
	template <typename TYPE> // сложение
	friend NumXY<TYPE> operator+ (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // вычитание
	friend NumXY<TYPE> operator- (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // умножение
	friend NumXY<TYPE> operator* (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // деление
	friend NumXY<TYPE> operator* (const NumXY<TYPE>& first, const NumXY<TYPE>& second);

	// перегрузка скалярных операторов
	template <typename TYPE1, typename TYPE2> // сложение
	friend NumXY<TYPE1> operator+ (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // вычитание
	friend NumXY<TYPE1> operator- (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // умножение
	friend NumXY<TYPE1> operator* (const NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // деление
	friend NumXY<TYPE1> operator/ (const NumXY<TYPE1>& other, const TYPE2& scalar);

	// перегрузка унарных операторов с аналогом
	template <typename TYPE> // сложение
	friend NumXY<TYPE>& operator+= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // вычитание
	friend NumXY<TYPE>& operator-= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // умножение
	friend NumXY<TYPE>& operator*= (NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE> // деление
	friend NumXY<TYPE>& operator/= (NumXY<TYPE>& first, const NumXY<TYPE>& second);

	// перегрузка унарных операторов со скаляром
	template <typename TYPE1, typename TYPE2> // сложение
	friend NumXY<TYPE1>& operator+= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // вычитание
	friend NumXY<TYPE1>& operator-= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // умножение
	friend NumXY<TYPE1>& operator*= (NumXY<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // деление
	friend NumXY<TYPE1>& operator/= (NumXY<TYPE1>& other, const TYPE2& scalar);

	// вывод в консоль
	template <typename TYPE>
	friend std::wostringstream& operator<< (std::wostringstream& out, const NumXY<TYPE>& other);

	// сравнения
	template <typename TYPE>
	friend bool operator== (const NumXY<TYPE>& first, const NumXY<TYPE>& second);
	template <typename TYPE>
	friend bool operator!= (const NumXY<TYPE>& first, const NumXY<TYPE>& second);

};

// сложение
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

// вычитание
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

// умножение
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

// деление
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

// вывод в консоль
template <typename TYPE>
std::wostringstream& operator<< (std::wostringstream& out, const NumXY<TYPE>& other) {

	out << other.x << ' ' << other.y;

	return out;

}

// сравнение
template <typename TYPE>
bool operator== (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return (first.x == second.x && first.y == second.y);

};
template <typename TYPE>
bool operator!= (const NumXY<TYPE>& first, const NumXY<TYPE>& second) {

	return (first.x != second.x || first.y != second.y);

};


// тройное число
template <typename TYPE>
struct NumXYZ {

	NumXYZ() :x(0), y(0), z(0) {};
	NumXYZ(TYPE x, TYPE y, TYPE z) :x(x), y(y), z(z) {};
	NumXYZ(TYPE x) :x(x), y(x), z(x) {};
	TYPE x, y, z;

	// перегрузка бинарных операторов
	template <typename TYPE> // сложение
	friend NumXYZ<TYPE> operator+ (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // вычитание
	friend NumXYZ<TYPE> operator- (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // умножение
	friend NumXYZ<TYPE> operator* (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // деление
	friend NumXYZ<TYPE> operator* (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

	// перегрузка бинарных операторов со скаляром
	template <typename TYPE1, typename TYPE2> // сложение
	friend NumXYZ<TYPE1> operator+ (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // вычитание
	friend NumXYZ<TYPE1> operator- (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // умножение
	friend NumXYZ<TYPE1> operator* (const NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // деление
	friend NumXYZ<TYPE1> operator/ (const NumXYZ<TYPE1>& other, const TYPE2& scalar);

	// перегрузка унарных операторов с аналогом
	template <typename TYPE> // сложение
	friend NumXYZ<TYPE>& operator+= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // вычитание
	friend NumXYZ<TYPE>& operator-= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // умножение
	friend NumXYZ<TYPE>& operator*= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE> // деление
	friend NumXYZ<TYPE>& operator/= (NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

	// перегрузка унарных операторов со скаляром
	template <typename TYPE1, typename TYPE2> // сложение
	friend NumXYZ<TYPE1>& operator+= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // вычитание
	friend NumXYZ<TYPE1>& operator-= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // умножение
	friend NumXYZ<TYPE1>& operator*= (NumXYZ<TYPE1>& other, const TYPE2& scalar);
	template <typename TYPE1, typename TYPE2> // деление
	friend NumXYZ<TYPE1>& operator/= (NumXYZ<TYPE1>& other, const TYPE2& scalar);

	// вывод в консоль
	template <typename TYPE>
	friend std::wostringstream& operator<< (std::wostringstream& out, const NumXYZ<TYPE>& other);

	// сравнения
	template <typename TYPE>
	friend bool operator== (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);
	template <typename TYPE>
	friend bool operator!= (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second);

};

// сложение
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

// вычитание
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

// умножение
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

// деление
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

// вывод в консоль
template <typename TYPE>
std::wostringstream& operator<< (std::wostringstream& out, const NumXYZ<TYPE>& other) {

	out << other.x << ' ' << other.y << ' ' << other.z;

	return out;

}

// сравнение
template <typename TYPE>
bool operator== (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return (first.x == second.x && first.y == second.y && first.z == second.z);

};
template <typename TYPE>
bool operator!= (const NumXYZ<TYPE>& first, const NumXYZ<TYPE>& second) {

	return (first.x != second.x || first.y != second.y || first.z != second.z);

};

// тройной плавающий
struct myXMFLOAT3 : public XMFLOAT3 {

	myXMFLOAT3() : XMFLOAT3(0, 0, 0) {};
	myXMFLOAT3(float value) : XMFLOAT3(value, value, value) {};
	myXMFLOAT3(float x, float y, float z) : XMFLOAT3(x, y, z) {};

	// перегрузка бинарных операторов
	friend myXMFLOAT3 operator+ (const myXMFLOAT3& first, const myXMFLOAT3& second); // сложение
	friend myXMFLOAT3 operator- (const myXMFLOAT3& first, const myXMFLOAT3& second); // вычитание
	friend myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second); // умножение
	friend myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second); // деление

	// перегрузка унарных операторов с аналогом
	friend myXMFLOAT3& operator+= (myXMFLOAT3& first, const myXMFLOAT3& second); // сложение
	friend myXMFLOAT3& operator-= (myXMFLOAT3& first, const myXMFLOAT3& second); // вычитание
	friend myXMFLOAT3& operator*= (myXMFLOAT3& first, const myXMFLOAT3& second); // умножение
	friend myXMFLOAT3& operator/= (myXMFLOAT3& first, const myXMFLOAT3& second); // деление

};

#endif // NUMS_H