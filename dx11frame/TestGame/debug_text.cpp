// DebugText.cpp
// Implementation file for DebugText.

#include "debug_text.h"

using namespace dx11_frame_helpers;

DebugText::DebugText(dx11_frame* pCDX11Frame) : i_entity(pCDX11Frame)
{
	init();
}

DebugText::~DebugText(void)
{
	cleanup();
}

bool		DebugText::init(void)
{
	auto good = true;

	_msg = L"";

	return is_init_ = good;
}

bool		DebugText::update(void)
{
	auto good = true;
	static auto count = 0; ++count;
	static float last = 0;
	static float fps = 0;
	static auto interval = 0.5f;

	if (get_cdx11_frame()->get_timer()->get_total_elapsed() > last + interval)
	{
		fps = count / interval;
		count = 0;
		last += interval;
	}

	if (get_do_update())
	{
		_msg =
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

bool		DebugText::render(void)
{
	auto good = true;

	get_cdx11_frame()->draw_debug_string(
		_msg,
		XMFLOAT2(5, 5),
		Colors::Yellow);

	return good;
}

void		DebugText::cleanup(void)
{
}