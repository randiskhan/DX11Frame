// win32.h
// Declaration file for class win32.

#pragma once

#include "common.h"
#include <windows.h>
#include <windowsx.h>

// This macro eliminates the need to pass an HINSTANCE reference
//	for window creation. Will always use current HINSTANCE.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;  // NOLINT(bugprone-reserved-identifier)
// ReSharper disable once IdentifierTypo
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)  // NOLINT(cppcoreguidelines-macro-usage)
#endif

// Interface for handling Win32 messages in an object
//	that creates this object.
class i_win32_app  // NOLINT(cppcoreguidelines-special-member-functions)
{
protected:
	~i_win32_app() = default;
public:
	virtual	LRESULT CALLBACK msg_proc(
		HWND,
		UINT,
		WPARAM,
		LPARAM) = 0;
};

struct win32_data
{
	i_win32_app*	i_win32_app;
	wstring			wnd_class_name;
	wstring			wnd_title;
	int				height;
	int				width;
	DWORD			window_style;
	BOOL			is_window_menu;

	win32_data()
	{
		i_win32_app = nullptr;
		wnd_class_name = L"WindowClass";
		wnd_title = L"DirectX11";
		width = 800;
		height = 600;
		window_style = (WS_CAPTION);
		is_window_menu = 0;
	}
};

// Class to encapsulate the code for generating a window in a Win32
// application.
class win32 final  // NOLINT(cppcoreguidelines-special-member-functions)
{

	MSG			msg_{};
	HWND		h_wnd_;
	bool		is_init_;
	win32_data	win32_data_;

public:
	explicit win32(const win32_data& wd);
	~win32();

	RECT			get_screen_rect() const;
	HWND			get_window() const;
	MSG*			get_last_msg();
	bool			is_init() const;

	// Initialization of all Win32 objects & logic.
	bool			init();
	static void		cleanup();

	// Method for clearing the Win32 message queue.
	bool		msg_queue_proc();

	// Default static WNDPROC callback
	static	LRESULT CALLBACK static_msg_proc(
		HWND,
		UINT,
		WPARAM,
		LPARAM);
	// Object member WNDPROC callback
	LRESULT CALLBACK msg_proc(
		HWND,
		UINT,
		WPARAM,
		LPARAM) const;

};
