// timer.cpp
// Implementation file for class timer.

#include "timer.h"

timer::timer()
{
	Init();
}

timer::~timer()
{
	cleanup();
}

bool	timer::Init()
{

	_IsInit = false;

	init_time_point_ = 
		time_point_previous_ = 
		time_point_current_ = 
		high_resolution_clock::now();

	_IsInit = true;

	return _IsInit;

}

bool	timer::Update()
{

	time_point_previous_ = time_point_current_;
	time_point_current_ = high_resolution_clock::now();

	return true;

}

void	timer::cleanup()
{
}

double	timer::get_total_elapsed() const
{
	return duration_cast<duration<double>>(
		time_point_current_ - init_time_point_).count();
}

double	timer::get_frame_elapsed() const
{
	return duration_cast<duration<double>>(
		time_point_current_ - time_point_previous_).count();
}
