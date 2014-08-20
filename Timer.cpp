#include "Timer.h"
#include "Mediator.h"

// ����������� �������
Timer::Timer() {

	// ���������� ����� ������ ������������� ��������
	__int64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	perfomancePeriod = 1.0f / frequency;

	// ���������� ��������� ������������
	QueryPerformanceCounter((LARGE_INTEGER*)&constructCounters);

	Mediator::timer = this;

}

// ������������� �������
bool Timer::Init() {

	// ��������� ��������
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&startCounters));
	tickStartCounters = startCounters;

	return true;

}

// �������� ��� �������, true - ���� ������ �������
bool Timer::Tick(float* dT) {

	// ���������� ��� ���������� ��������
	static float fullTime = 0.0f; // ������� ����� ���� �����
	static UINT counter = 0; // ���������� ����� �� �������

	// ������� ��������
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&tickStartCounters));

	// ������� ������� ������ � ��������
	float tickTime;
	tickTime = max(0.0f, (tickStartCounters - startCounters) * perfomancePeriod);
	startCounters = tickStartCounters;

	// ���������� ���������� ����������
	fullTime += tickTime;
	counter++;
	if (fullTime > 1.0f) {
		// ������ �������
		FPS = counter;
		frameTime = 1.0f / FPS;
		fullTime -= 1.0f;
		counter = 0;
		return true;
	}

	*dT = tickTime;

	return false;

}

// ���������� ������� ����� �����
float Timer::GetTickTime() {

	__int64 curCounters;
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&curCounters));
	return (curCounters - tickStartCounters) * perfomancePeriod;

}

// ��������� ������
bool Timer::StartTimer() {

	BR(QueryPerformanceCounter((LARGE_INTEGER*)&startTimerCounter));

	if (timerStarted) return false;
	timerStarted = true;
	return true;

}

// �������� ����� �������
float Timer::GetTimer() {

	timerStarted = false;

	__int64 curTimerCounter;
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&curTimerCounter));

	return (curTimerCounter - startTimerCounter) * perfomancePeriod;

}