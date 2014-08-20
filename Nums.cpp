#include "Nums.h"

// перегрузка бинарных операторов
myXMFLOAT3 operator+ (const myXMFLOAT3& first, const myXMFLOAT3& second) { // сложение
	return myXMFLOAT3(first.x + second.x, first.y + second.y, first.z + second.z);
}
myXMFLOAT3 operator- (const myXMFLOAT3& first, const myXMFLOAT3& second) { // вычитание
	return myXMFLOAT3(first.x - second.x, first.y - second.y, first.z - second.z);
}
myXMFLOAT3 operator* (const myXMFLOAT3& first, const myXMFLOAT3& second) { // умножение
	return myXMFLOAT3(first.x * second.x, first.y * second.y, first.z * second.z);
}
myXMFLOAT3 operator/ (const myXMFLOAT3& first, const myXMFLOAT3& second) { // деление
	return myXMFLOAT3(first.x / second.x, first.y / second.y, first.z / second.z);
}

// перегрузка унарных операторов с аналогом
myXMFLOAT3& operator+= (myXMFLOAT3& first, const myXMFLOAT3& second) { // сложение
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	return first;
}
myXMFLOAT3& operator-= (myXMFLOAT3& first, const myXMFLOAT3& second) { // вычитание
	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	return first;
}
myXMFLOAT3& operator*= (myXMFLOAT3& first, const myXMFLOAT3& second) { // умножение
	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	return first;
}
myXMFLOAT3& operator/= (myXMFLOAT3& first, const myXMFLOAT3& second) { // деление
	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	return first;
}