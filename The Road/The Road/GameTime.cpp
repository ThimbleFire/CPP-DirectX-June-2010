#include "GameTime.h"

GameTime::GameTime()
{
	frequency_=0;
	ticksPerMs_=0;
	startTime_=0;
	frameTime_=0;
}

GameTime::~GameTime()
{

}

void GameTime::Start()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency_);

	ticksPerMs_ = (float)(frequency_ / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime_);
}

float GameTime::GetTime()
{
	return frameTime_;
}

void GameTime::GetFrameTime()
{
	INT64 currentTime;

	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - startTime_);

	frameTime_ = timeDifference / ticksPerMs_;

	startTime_ = currentTime;
}