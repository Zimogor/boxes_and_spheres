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

#define DRAW_ANGLE_DEFAULT 0.0f // ���� �������� �� ���������

// ���������
#ifndef MyMessage
#define MyMessage(x)\
	MessageBox(NULL, x, L"error", MB_OK);
#endif // MyMessage

// �������� �������� �������
#ifndef ReleaseNULL
#define ReleaseNULL(x)\
	if (x) {\
		delete (x);\
		x = NULL;\
	}
#endif // ReleaseNULL

// �������� ���������� ��������
#ifndef ReleaseNULLS
#define ReleaseNULLS(x)\
	if (x) {\
	delete[] (x);\
	x = NULL;\
	}
#endif // ReleaseNULLS

// �������� COM �������
#ifndef ReleaseCOM
#define ReleaseCOM(x)\
	if (x) {\
		x->Release();\
		x = NULL;\
	}
#endif // ReleaseCOM

// �������� �������� �� ���������� bool
#ifndef BR
#define BR(x)\
	if (!(x)) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // BR

// �������� �������� �� HRESULT
#ifndef HR
#define HR(x)\
	if (FAILED(x)) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // HR

// �������� �� bool � ������������� ��������
#ifndef BRR
#define BRR(x)\
	if (!(x)) {\
	Mediator::errors->Push(L#x);\
	*result = false;\
	return;\
	}
#endif // BRR

// �������� �� HRESULT � ������������� ��������
#ifndef HRR
#define HRR(x)\
	if (FAILED(x)) {\
	Mediator::errors->Push(L#x);\
	*result = false;\
	return;\
	}
#endif // HRR

// ������������ � ������� main (�������� ���������� result)
#ifndef BRR0
#define BRR0(x)\
	x;\
	if (!result) {\
		errors.Push(L#x);\
		errors.ShowTotalMessage();\
		return 0;\
	}
#endif // BRR0

// ������������ ��� �������� ������ ������������� (�������� ���������� result)
#ifndef BRRC1
#define BRRC1(x)\
	x;\
	if (!result) {\
	Mediator::errors->Push(L#x);\
	return false;\
	}
#endif // BRRC1

// ������������ ��� �������� ������������� � ������ ������������� (�������� ���������� result)
#ifndef BRRC2
#define BRRC2(x)\
	x;\
	if (!*result) {\
	Mediator::errors->Push(L#x);\
	return;\
	}
#endif // BRRC2

// ������������ � WinProc
#ifndef BRQ
#define BRQ(x)\
	if (!(x)) {\
		Mediator::errors->Push(L#x);\
		PostQuitMessage(0);\
	}
#endif // BRQ

// ��������� ����� � �������
#ifndef OnlyChange
#define OnlyChange(message, value, valInit)\
static auto valPrev = valInit;\
if (value != valPrev) {\
	BR(Mediator::console->Push(L#message, value));\
	valPrev = value;\
}
#endif // OnlyChange

// ������ ���������� ��������
#ifndef CompileError
#define CompileError(x)\
if (x) {\
MessageBoxA(NULL, (char*)x->GetBufferPointer(), NULL, MB_OK);\
return false;\
}\
ReleaseCOM(x);
#endif // CompileError

#endif // UTILITIES_H