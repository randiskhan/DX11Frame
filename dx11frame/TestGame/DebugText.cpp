// DebugText.cpp
// Implementation file for DebugText.

#include "DebugText.h"

DebugText::DebugText(dx11_frame* pCDX11Frame) : IEntity(pCDX11Frame)
{
	Init();
}

DebugText::~DebugText(void)
{
	Cleanup();
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

	if (GetCDX11Frame()->get_timer()->get_total_elapsed() > last + interval)
	{
		fps = count / interval;
		count = 0;
		last += interval;
	}

	if (GetDoUpdate())
	{
		_msg =
			L"Press escape to exit.\n" +
			ToString(GetCDX11Frame()->get_input()->get_mouse_screen_pos()->x) +
			L"," +
			ToString(GetCDX11Frame()->get_input()->get_mouse_screen_pos()->y) +
			L" - FPS: " +
			ToString(fps);
	}

	return good;
}

bool		DebugText::Render(void)
{
	auto good = true;

	GetCDX11Frame()->draw_debug_string(
		_msg,
		XMFLOAT2(5, 5),
		Colors::Yellow);

	return good;
}

void		DebugText::Cleanup(void)
{
}