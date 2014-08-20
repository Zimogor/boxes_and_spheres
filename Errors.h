#ifndef ERRORS_H
#define ERRORS_H

#include "Utilities.h"

// класс обработки ошибок
class Errors {

public:

	// обработка ошибок
	Errors(); // конструктор
	~Errors(); // деструктор

	// общие функции
	bool Push(wchar_t* message); // новое сообщение
	bool ShowTotalMessage(); // отобразить полное сообщение

private:

	// вектор указателей на сообщения
	std::vector<wchar_t*> messages;

};

#endif // ERRORS_H