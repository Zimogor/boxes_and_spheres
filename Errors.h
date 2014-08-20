#ifndef ERRORS_H
#define ERRORS_H

#include "Utilities.h"

// ����� ��������� ������
class Errors {

public:

	// ��������� ������
	Errors(); // �����������
	~Errors(); // ����������

	// ����� �������
	bool Push(wchar_t* message); // ����� ���������
	bool ShowTotalMessage(); // ���������� ������ ���������

private:

	// ������ ���������� �� ���������
	std::vector<wchar_t*> messages;

};

#endif // ERRORS_H