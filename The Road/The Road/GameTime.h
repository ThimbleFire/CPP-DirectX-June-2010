#ifndef _GAMETIME_H_
#define _GAMETIME_H_

#pragma comment(lib, "pdh.lib")

#include <pdh.h>

class GameTime
{
public:
	GameTime();
	~GameTime();

	void Start();
	float GetTime();
	void  GetFrameTime();
	
private:
	INT64 frequency_;
	float ticksPerMs_;
	INT64 startTime_;
	float frameTime_;

};

#endif