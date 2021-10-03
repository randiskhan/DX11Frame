// timer.cpp
// Implementation file for class timer.

#include "timer.h"

timer::timer()
{
	init();
}

timer::~timer()
{
	cleanup();
}

bool	timer::init()
{

	is_init_ = false;

	init_time_point_ = 
		time_point_previous_ = 
		time_point_current_ = 
		high_resolution_clock::now();

	is_init_ = true;

	return is_init_;

}

bool	timer::update()
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
