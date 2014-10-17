// CTimer.h
// Declaration file for CTimer.

#pragma once

#include "common.h"
//#include "IInitializable.h"
//#include "IUpdateable.h"
//#include "ICleanupable.h"
#include <chrono>

using namespace std::chrono;

class CTimer : public IInitializable, public IUpdateable, public ICleanupable
{
private:
	high_resolution_clock::time_point	_InitTimePoint;
	high_resolution_clock::time_point	_CurrTimePoint;
	high_resolution_clock::time_point	_PrevTimePoint;

public:
	CTimer(void);
	virtual ~CTimer(void);

	bool	Init(void);
	bool	Update(void);
	void	Cleanup(void);

	float	GetTotalElapsed(void);
	float	GetFrameElapsed(void);
};

