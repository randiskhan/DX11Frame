// dx11_frame.h
// Declaration file for dx11_frame.

#pragma once

#pragma region preprocessor directives

// Common header.
#include "common.h"
// DX11Frame object headers.
#include "win32.h"
#include "directx.h"
#include "input.h"
#include "timer.h"

#pragma endregion

// Base class for "root" class/object in executable.
#ifdef MEMBER_MSGPROC
 // ReSharper disable once CommentTypo
class dx11_frame : public i_win32_app  // NOLINT(cppcoreguidelines-special-member-functions)
#else
// ReSharper disable once CommentTypo
class dx11_frame  // NOLINT(cppcoreguidelines-special-member-functions)
#endif
{

	// DX11Frame primary objects.
	unique_ptr<win32>		win32_{};
	unique_ptr<directx>		directx_{};
	unique_ptr<input>		input_{};
	unique_ptr<timer>		timer_{};
	// DirectXTK objects.
	unique_ptr<SpriteBatch>	sprite_batch_{};
	unique_ptr<SpriteFont>	debug_font_{};

public:
	dx11_frame();
	virtual ~dx11_frame();

	WPARAM			run();

	// Get DX11Frame object references.
	win32*			get_win32() const;
	directx*		get_directx() const;
	input*			get_input() const;
	timer*			get_timer() const;
	// Get DirectXTK object references.
	SpriteBatch*	get_sprite_batch() const;

	void			draw_debug_string(
		const wstring& msg, 
		XMFLOAT2 loc, 
		FXMVECTOR color) const;

protected:
	win32_data		win32_data_;
	directx_data	directx_data_;
	input_data		input_data_;
	wstring			debug_spritefont_path_;

	static void		post_quit();

	// Pure virtual methods.
	virtual bool	pre_init();
	virtual bool	post_init();
	virtual bool	update();
	virtual bool	render();
	virtual void	cleanup();

private:
	bool			main_loop_iteration();

	// Win32 message handler basic implementation. Available for overriding in CAppBase child classes.
	virtual LRESULT CALLBACK	msg_proc(HWND, UINT, WPARAM, LPARAM);

	bool			init_base();
	bool			update_base();
	bool			render_base();
	static void		cleanup_base();

};
