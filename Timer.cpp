#include "Timer.h"
#include "Mediator.h"

// конструктор таймера
Timer::Timer() {

	// вычисление время одного процессорного каунтера
	__int64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	perfomancePeriod = 1.0f / frequency;

	// вычисление каунтеров конструктора
	QueryPerformanceCounter((LARGE_INTEGER*)&constructCounters);

	Mediator::timer = this;

}

// инициализация таймера
bool Timer::Init() {

	// стартовые каунтеры
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&startCounters));
	tickStartCounters = startCounters;

	return true;

}

// основной тик таймера, true - если прошла секунда
bool Timer::Tick(float* dT) {

	// переменные для секундного подсчёта
	static float fullTime = 0.0f; // считает время всех тиков
	static UINT counter = 0; // количество тиков за секунду

	// текущие каунтеры
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&tickStartCounters));

	// сколько времени прошло в секундах
	float tickTime;
	tickTime = max(0.0f, (tickStartCounters - startCounters) * perfomancePeriod);
	startCounters = tickStartCounters;

	// вычисление секундного обновления
	fullTime += tickTime;
	counter++;
	if (fullTime > 1.0f) {
		// прошла секунда
		FPS = counter;
		frameTime = 1.0f / FPS;
		fullTime -= 1.0f;
		counter = 0;
		return true;
	}

	*dT = tickTime;

	return false;

}

// возвращает текущее время кадра
float Timer::GetTickTime() {

	__int64 curCounters;
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&curCounters));
	return (curCounters - tickStartCounters) * perfomancePeriod;

}

// запустить таймер
bool Timer::StartTimer() {

	BR(QueryPerformanceCounter((LARGE_INTEGER*)&startTimerCounter));

	if (timerStarted) return false;
	timerStarted = true;
	return true;

}

// получить время таймера
float Timer::GetTimer() {

	timerStarted = false;

	__int64 curTimerCounter;
	BR(QueryPerformanceCounter((LARGE_INTEGER*)&curTimerCounter));

	return (curTimerCounter - startTimerCounter) * perfomancePeriod;

}