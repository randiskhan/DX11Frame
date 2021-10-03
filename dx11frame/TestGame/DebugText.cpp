// DebugText.cpp
// Implementation file for DebugText.

#include "DebugText.h"

DebugText::DebugText(dx11_frame* pCDX11Frame) : i_entity(pCDX11Frame)
{
	Init();
}

DebugText::~DebugText(void)
{
	cleanup();
}

bool		DebugText::Init(void)
{
	auto good = true;

	_msg = L"";

	return _IsInit = good;
}

bool		DebugText::Update(void)
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

	if (GetDoUpdate())
	{
		_msg =
			L"Press escape to exit.\n" +
			ToString(get_cdx11_frame()->get_input()->get_mouse_screen_pos()->x) +
			L"," +
			ToString(get_cdx11_frame()->get_input()->get_mouse_screen_pos()->y) +
			L" - FPS: " +
			ToString(fps);
	}

	return good;
}

bool		DebugText::Render(void)
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