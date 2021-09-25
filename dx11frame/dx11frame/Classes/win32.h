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
	i_win32_app*	pICWin32App;
	wstring			wndClassName;
	wstring			wndTitle;
	int				height;
	int				width;
	DWORD			windowStyle;
	BOOL			isWindowMenu;

	win32_data()
	{
		pICWin32App = nullptr;
		wndClassName = L"WindowClass";
		wndTitle = L"DirectX11";
		width = 800;
		height = 600;
		windowStyle = (WS_CAPTION);
		isWindowMenu = 0;
	}
};

// Class to encapsulate the code for generating a window in a Win32 application.
class win32
{

	MSG			_msg;
	HWND		_HWnd;
	bool		_IsInit;
	win32_data	_CWin32Data;

public:
	win32(win32_data wd);
	virtual ~win32(void);

	RECT		GetScreenRect(void);
	HWND		GetWindow(void);
	MSG*		GetLastMsg(void);
	bool		IsInit(void);

	// Initialization of all Win32 objects & logic.
	bool		Init(void);
	void		Cleanup(void);

	// Method for clearing the Win32 message queue.
	bool		MsgQueueProc(void);

	// Default static WNDPROC callback
	static	LRESULT CALLBACK StaticMsgProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM);
	// Object member WNDPROC callback
	LRESULT CALLBACK MsgProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM);

};
