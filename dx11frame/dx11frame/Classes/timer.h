// timer.h
// Declaration file for class timer.

#pragma once

#include "common.h"
#include <chrono>

using namespace std::chrono;

class timer final :  // NOLINT(cppcoreguidelines-special-member-functions)
	public IInitializable, public IUpdateable, public ICleanupable
{
private:
	high_resolution_clock::time_point	init_time_point_{};
	high_resolution_clock::time_point	time_point_current_{};
	high_resolution_clock::time_point	time_point_previous_{};

public:
	timer();
	~timer() override;

	bool	Init() override;
	bool	Update() override;
	void	Cleanup() override;

	double	get_total_elapsed() const;
	double	get_frame_elapsed() const;
};
