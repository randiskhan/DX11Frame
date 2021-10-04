// input.h
// Declaration file for class input.

#pragma once

#include "common.h"
#include <windows.h>

struct input_data
{
	HWND	hwnd;

	input_data()
	{
		hwnd = nullptr;
	}
};

// Class for all input logic and objects
class input final  // NOLINT(cppcoreguidelines-special-member-functions)
{

	POINT			mouse_position_{};
	BYTE			keys_current_update_[256]{};
	BYTE			keys_previous_update_[256]{};

	input_data		input_data_;

public:
	explicit		input(input_data id);
					~input();

	static bool		init();
	bool			update();
	static void		cleanup();

	void			set_mouse_pos(int x, int y);

	PPOINT			get_mouse_screen_pos();
	bool			is_mouse_button_left_down();
	bool			is_mouse_button_left_up();
	bool			is_mouse_button_right_down();
	bool			is_mouse_button_right_up();
	bool			is_mouse_button_left_down_since_last_frame();
	bool			is_mouse_button_left_up_since_last_frame();
	bool			is_mouse_button_right_down_since_last_frame();
	bool			is_mouse_button_right_up_since_last_frame();

	bool			is_key_down(int vk);
	bool			is_key_up(int vk);
	bool			is_key_down_since_last_frame(int vk);
	bool			is_key_up_since_last_frame(int vk);

};
