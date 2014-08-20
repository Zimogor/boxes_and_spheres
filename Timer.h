#ifndef TIMER_H
#define TIMER_H

#include "Utilities.h"

class Timer {

public:

	// �����������
	Timer();

	// �������� �������
	bool Init(); // ������������� �������
	bool Tick(float* dT); // ��� �������, true - ���� ����� �������� (��� � �������)

	// �������� ����������
	UINT FPS = 0;
	float frameTime = 0.0f;

	// ������� �������
	float GetTickTime(); // ���������� ������� ����� �����
	bool StartTimer(); // ��������� ������
	float GetTimer(); // �������� ����� �������

private:

	// ����������
	float perfomancePeriod = 0.0f; // ����� ������ ������������� ��������
	__int64 constructCounters = 0; // �������� ������������ �������
	__int64 startCounters = 0; // ��������� �������� �������
	__int64 tickStartCounters = 0; // �������� ������ �����
	__int64 startTimerCounter = 0; // �������� ������ �������
	bool timerStarted = false; // ������ �������

};

#endif // TIMER_H