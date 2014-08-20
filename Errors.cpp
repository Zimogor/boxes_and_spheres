#include "Errors.h"
#include "Mediator.h"

// �����������
Errors::Errors() {

	Mediator::errors = this;

}

// ���������� ������ ���������
bool Errors::ShowTotalMessage() {

	if (messages.size()) {

		std::wostringstream out;

		// ���������� ��� ���������
		for (UINT i(0); i < messages.size(); i++) {

			out << messages[i] << '\n';

		}

		MyMessage(out.str().c_str());

	}

	return true;

}

// ����� ���������
bool Errors::Push(wchar_t* message) {

	UINT messageSize = wcslen(message) + 1;
	wchar_t* newMessage = new wchar_t[messageSize];
	wcscpy_s(newMessage, messageSize, message);

	messages.push_back(newMessage);

	return true;

}

// ���������� ������
Errors::~Errors() {

	for (UINT i(0); i < messages.size(); i++) {
		delete[] messages[i];
	}

}