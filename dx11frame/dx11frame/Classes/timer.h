// timer.h
// Declaration file for class timer.

#pragma once

#include "common.h"
#include <chrono>

using namespace std::chrono;

// ReSharper disable once CommentTypo
class timer final :  // NOLINT(cppcoreguidelines-special-member-functions)
	public i_initializable, public IUpdateable, public i_cleanupable
{

	high_resolution_clock::time_point	init_time_point_{};
	high_resolution_clock::time_point	time_point_current_{};
	high_resolution_clock::time_point	time_point_previous_{};

public:
	timer();
	~timer() override;

	bool	init() override;
	bool	Update() override;
	void	cleanup() override;

	double	get_total_elapsed() const;
	double	get_frame_elapsed() const;

};
