#ifndef TIMER_H
#define TIMER_H

#include "Utilities.h"

class Timer {

public:

	// конструктор
	Timer();

	// основные функции
	bool Init(); // инициализация таймера
	bool Tick(float* dT); // тик таймера, true - если новые значения (раз в секунду)

	// основные переменные
	UINT FPS = 0;
	float frameTime = 0.0f;

	// рабочие функции
	float GetTickTime(); // возвращает текущее время кадра
	bool StartTimer(); // запустить таймер
	float GetTimer(); // получить время таймера

private:

	// переменные
	float perfomancePeriod = 0.0f; // время одного процессорного каунтера
	__int64 constructCounters = 0; // каунтеры конструктора таймера
	__int64 startCounters = 0; // стартовые каунтеры таймера
	__int64 tickStartCounters = 0; // каунтеры начала кадра
	__int64 startTimerCounter = 0; // каунтера старта таймера
	bool timerStarted = false; // таймер запущен

};

#endif // TIMER_H