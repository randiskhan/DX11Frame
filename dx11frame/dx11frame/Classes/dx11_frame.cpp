// dx11_frame.cpp
// Implementation file for dx11_frame.

#include "dx11_frame.h"

#pragma region Construction/destruction

dx11_frame::dx11_frame() = default;

dx11_frame::~dx11_frame()
{
	cleanup_base();
}

#pragma endregion

#pragma region Base methods

WPARAM dx11_frame::run()
{

	WPARAM ret = 0;

	if (init_base())
	{
		// Main loop.
		// ReSharper disable once CppPossiblyErroneousEmptyStatements
		while (main_loop_iteration());

		// If a WM_QUIT message was received through message queue,
		//	return the wParam. Otherwise, return zero. Advised by MSDN.
		if (win32_->get_last_msg()->message == WM_QUIT)
			ret = win32_->get_last_msg()->wParam;
	}

	cleanup_base();

	return ret;

}

bool dx11_frame::main_loop_iteration()
{

	bool good;

	// Clear the message queue.
	if ((good = win32_->msg_queue_proc()))
	{
		good = update_base();
		good &= render_base();
	}

	return good;

}

bool dx11_frame::init_base()
{

	auto good = true;

	if (good) good = pre_init();

	// DX11Frame object creation.
	timer_.reset(new timer());
	if (!(timer_ && timer_->IsInit())) 
		good = false;

	win32_.reset(new win32(win32_data_));
	if (!win32_) 
		good = false;
	if (good) 
		good = get_win32()->init();

	if (good)
	{
		// Make sure everyone is working with the same window.
		input_data_.hwnd = get_win32()->get_window();
		directx_data_.hwnd = get_win32()->get_window();
		// Synchronize window and back-buffer dimensions if desired.
		if (directx_data_.use_hwnd_dimensions)
		{
			directx_data_.width = win32_data_.width;
			directx_data_.height = win32_data_.height;
		}
	}

	if (good) 
		directx_.reset(new directx(directx_data_));
	if (!directx_) 
		good = false;
	if (good) 
		good = get_directx()->Init();

	if (good) 
		input_.reset(new input(input_data_));
	if (!input_) 
		good = false;
	if (good) 
		good = get_input()->init();  // NOLINT(readability-static-accessed-through-instance)

	// DirectXTK object creation
	if (good) 
		sprite_batch_.reset(new SpriteBatch(get_directx()->get_context()));
	if (!sprite_batch_) 
		good = false;

	// Create SpriteFont for debugging, but don't exit if failed.
	if (good && (debug_spritefont_path_.length() > 0))
		debug_font_.reset(
			new SpriteFont(get_directx()->get_device(), 
				debug_spritefont_path_.c_str()));

	if (good) 
		good = post_init();

	return good;

}

bool dx11_frame::update_base()
{

	auto good = true;

	good &= get_timer()->Update();
	if (good) good = get_input()->update();
	if (good) good = update();

	return good;

}

bool dx11_frame::render_base()
{

	auto good = get_directx()->begin_render();
	if (good) 
		good = render();
	if (good) 
		good = get_directx()->end_render();

	return good;

}

void dx11_frame::cleanup_base()
{
}

#pragma endregion

#pragma region Object reference getters

win32*			dx11_frame::get_win32() const
{
	return win32_.get();
}

directx*		dx11_frame::get_directx() const
{
	return directx_.get();
}

input*			dx11_frame::get_input() const
{
	return input_.get();
}

timer*			dx11_frame::get_timer() const
{
	return timer_.get();
}

SpriteBatch*	dx11_frame::get_sprite_batch() const
{
	return sprite_batch_.get();
}

#pragma endregion

#pragma region Win32 message processing

auto			CALLBACK dx11_frame::msg_proc(
	const HWND hwnd, // NOLINT(misc-misplaced-const)
	const UINT msg,
	const WPARAM w_param,
	const LPARAM l_param) -> LRESULT
{
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

#pragma endregion

#pragma region Instance methods

void	dx11_frame::post_quit()
{
	PostQuitMessage(0);
}

bool	dx11_frame::pre_init()
{
	return true;
}

bool	dx11_frame::post_init()
{
	return true;
}

bool	dx11_frame::update()
{
	return true;
}

bool	dx11_frame::render()
{
	return true;
}

void	dx11_frame::cleanup()
{
}

void	dx11_frame::draw_debug_string(
	const wstring& msg,
	const XMFLOAT2 loc, 
	FXMVECTOR color) const
{
	if (debug_font_) 
		debug_font_->DrawString(
			get_sprite_batch(), 
			msg.c_str(), 
			loc, 
			color);
}

#pragma endregion
