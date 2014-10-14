#include "CTimer.h"

CTimer::CTimer(void)
{
	Init();
}

CTimer::~CTimer(void)
{
	Cleanup();
}

bool	CTimer::Init(void)
{
	bool good = true;
	_IsInit = false;

	// Do initialization stuff.
	_InitTimePoint = _PrevTimePoint = _CurrTimePoint = high_resolution_clock::now();

	return  _IsInit = good;
}

bool	CTimer::Update(void)
{
	bool good = true;

	// Do update stuff.
	_PrevTimePoint = _CurrTimePoint;
	_CurrTimePoint = high_resolution_clock::now();

	return  good;
}

void	CTimer::Cleanup(void)
{
	// Do cleanup stuff.
}

float	CTimer::GetTotalElapsed(void)
{
	return duration_cast<duration<float>>(_CurrTimePoint - _InitTimePoint).count();
}

float	CTimer::GetFrameElapsed(void)
{
	return duration_cast<duration<float>>(_CurrTimePoint - _PrevTimePoint).count();
}