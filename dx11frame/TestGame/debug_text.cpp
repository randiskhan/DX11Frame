// debug_text.cpp
// Implementation file for debug_text.

#include "debug_text.h"

using namespace dx11_frame_helpers;

debug_text::debug_text(dx11_frame* dx11_frame) : i_entity(dx11_frame)
{
	init();
}

debug_text::~debug_text()
{
	cleanup();
}

bool		debug_text::init()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	msg_ = L"";

	return is_init_ = good;

}

bool		debug_text::update()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	static auto count = 0; ++count;
	static float last = 0;
	static float fps = 0;
	static auto interval = 0.5f;

	if (get_cdx11_frame()->get_timer()->get_total_elapsed() > last + interval)
	{
		fps = count / interval;  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
		count = 0;
		last += interval;
	}

	if (get_do_update())
	{
		msg_ =
			L"Press escape to exit.\n" +
			dx11_frame_helpers::to_string(
				get_cdx11_frame()->get_input()->get_mouse_screen_pos()->x) +
			L"," +
			dx11_frame_helpers::to_string(
				get_cdx11_frame()->get_input()->get_mouse_screen_pos()->y) +
			L" - FPS: " +
			dx11_frame_helpers::to_string(fps);
	}

	return good;

}

bool		debug_text::render()
{

	// ReSharper disable once CppLocalVariableMayBeConst
	auto good = true;

	get_cdx11_frame()->draw_debug_string(
		msg_,
		XMFLOAT2(5, 5),
		Colors::Yellow);

	return good;

}

void		debug_text::cleanup()
{
}