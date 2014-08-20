#ifndef UTILITIES_H
#define UTILITIES_H

#include <crtdbg.h>
#include <Windows.h>
#include <windowsx.h>
#include <DirectXMath.h>
#include <vector>
#include <sstream>
#include <d3d11.h>
#include <fstream>
#include <D3DX11.h>
#include <xnamath.h>

#define DRAW_ANGLE_DEFAULT 0.0f // угол поворота по умолчанию

// сообщение
#ifndef MyMessage
#define MyMessage(x)\
	MessageBox(NULL, x, L"error", MB_OK);
#endif // MyMessage

// удаление простого объекта
#ifndef ReleaseNULL
#define ReleaseNULL(x)\
	if (x) {\
		delete (x);\
		x = NULL;\
	}
#endif // ReleaseNULL

// удаление нескольких объектов
#ifndef ReleaseNULLS
#define ReleaseNULLS(x)\
	if (x) {\
	delete[] (x);\
	x = NULL;\
	}
#endif // ReleaseNULLS

// удаление COM объекта
#ifndef ReleaseCOM
#define ReleaseCOM(x)\
	if (x) {\
		x->Release();\
		x = NULL;\
	}
#endif // ReleaseCOM

// простра€ проверка на истинность bool
#ifndef BR
#define BR(x)\
	if (!(x)) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // BR

// простра€ проверка на HRESULT
#ifndef HR
#define HR(x)\
	if (FAILED(x)) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // HR

// проверка на bool в конструкторах объектов
#ifndef BRR
#define BRR(x)\
	if (!(x)) {\
	Mediator::errors->Push(L#x);\
	*result = false;\
	return;\
	}
#endif // BRR

// проверка на HRESULT в конструкторах объектов
#ifndef HRR
#define HRR(x)\
	if (FAILED(x)) {\
	Mediator::errors->Push(L#x);\
	*result = false;\
	return;\
	}
#endif // HRR

// используетс€ в функции main (проверка переменной result)
#ifndef BRR0
#define BRR0(x)\
	x;\
	if (!result) {\
		errors.Push(L#x);\
		errors.ShowTotalMessage();\
		return 0;\
	}
#endif // BRR0

// используетс€ дл€ создани€ других конструкторов (проверка переменной result)
#ifndef BRRC1
#define BRRC1(x)\
	x;\
	if (!result) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // BRRC1

// используетс€ дл€ создани€ конструкторов в других конструкторах (проверка переменной result)
#ifndef BRRC2
#define BRRC2(x)\
	x;\
	if (!*result) {\
	Mediator::errors->Push(L#x);\
	return;\
	}
#endif // BRRC2

// используетс€ с WinProc
#ifndef BRQ
#define BRQ(x)\
	if (!(x)) {\
		Mediator::errors->Push(L#x);\
		PostQuitMessage(0);\
	}
#endif // BRQ

// единичный вывод в консоль
#ifndef OnlyChange
#define OnlyChange(message, value, valInit)\
static auto valPrev = valInit;\
if (value != valPrev) {\
	BR(Mediator::console->Push(L#message, value));\
	valPrev = value;\
}
#endif // OnlyChange

// ошибка компил€ции шейдеров
#ifndef CompileError
#define CompileError(x)\
if (x) {\
MessageBoxA(NULL, (char*)x->GetBufferPointer(), NULL, MB_OK);\
return false;\
}\
ReleaseCOM(x);
#endif // CompileError

#endif // UTILITIES_H