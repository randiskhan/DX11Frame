// input.cpp
// Implementation file for class input.

#include "input.h"

#pragma region Construction/destruction

input::input(const input_data id)
{
	input_data_ = id;
	mouse_position_.x = 0;
	mouse_position_.y = 0;
	for (auto i = 0; i < 256; ++i)
		keys_current_update_[i] = keys_previous_update_[i] = 0;
}

input::~input()
{
	cleanup();
}

#pragma endregion

#pragma region Instance methods

bool input::init()
{
	return true;
}

bool input::update()
{

	for (auto i = 0; i < 256; ++i)
		keys_previous_update_[i] = keys_current_update_[i];

	const bool good = (0 < GetKeyboardState(keys_current_update_));

	// Removed fail check on getting mouse coordinates.
	// This was done to not exit upon a ctrl+alt+del, which causes
	// GetCursorPos() to fail.
	if (0 < GetCursorPos(&mouse_position_))
		ScreenToClient(input_data_.hwnd, &mouse_position_);

	return good;

}

void input::cleanup()
{
}

void input::set_mouse_pos(const int x, const int y)
{
	mouse_position_.x = x;
	mouse_position_.y = y;
}

#pragma endregion

#pragma region Input querying

PPOINT	input::get_mouse_screen_pos()
{
	return &mouse_position_;
}

bool	input::is_key_down(const int vk)
{
	return (keys_current_update_[vk] & 0x80) != 0;
}

bool	input::is_key_up(const int vk)
{
	return (keys_current_update_[vk] & 0x80) == 0;
}

bool	input::is_key_down_since_last_frame(const int vk)
{
	return (((keys_current_update_[vk] & 0x80) != 0) && ((keys_previous_update_[vk] & 0x80) == 0));
}

bool	input::is_key_up_since_last_frame(const int vk)
{
	return (((keys_current_update_[vk] & 0x80) == 0) && ((keys_previous_update_[vk] & 0x80) != 0));
}

bool	input::is_mouse_button_left_down()
{
	return is_key_down(VK_LBUTTON);
}

bool	input::is_mouse_button_left_up()
{
	return is_key_up(VK_LBUTTON);
}

bool	input::is_mouse_button_right_down()
{
	return is_key_down(VK_RBUTTON);
}

bool	input::is_mouse_button_right_up()
{
	return is_key_up(VK_RBUTTON);
}

bool	input::is_mouse_button_left_down_since_last_frame()
{
	return is_key_down_since_last_frame(VK_LBUTTON);
}

bool	input::is_mouse_button_left_up_since_last_frame()
{
	return is_key_up_since_last_frame(VK_LBUTTON);
}

bool	input::is_mouse_button_right_down_since_last_frame()
{
	return is_key_down_since_last_frame(VK_RBUTTON);
}

bool	input::is_mouse_button_right_up_since_last_frame()
{
	return is_key_up_since_last_frame(VK_RBUTTON);
}

#pragma endregion
