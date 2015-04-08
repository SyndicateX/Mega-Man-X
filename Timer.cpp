#include "timer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Timer::Timer()
{
	QueryPerformanceFrequency(&frequency_);
	QueryPerformanceCounter(&referenceTime_);
}

double Timer::check() const
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	return (double(currentTime.QuadPart) - referenceTime_.QuadPart) / frequency_.QuadPart;
}

void Timer::reset()
{
	QueryPerformanceCounter(&referenceTime_);
}

void Timer::sleep(DWORD sleepTime) const
{
	timeBeginPeriod(1);
	Sleep(sleepTime);
	timeEndPeriod(1);
}