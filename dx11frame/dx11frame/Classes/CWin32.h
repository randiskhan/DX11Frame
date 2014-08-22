// CWin32.h
// Declaration file for CWin32.

#pragma once

#include <windows.h>
#include <windowsx.h>
#include "common.h"

// This macro eliminates the need to pass an HINSTANCE reference
//	for window creation. Will always use current HINSTANCE.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

// Interface for handling Win32 messages in an object
//	that creates this object.
class ICWin32App
{
public:
	virtual	LRESULT CALLBACK MsgProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM) = 0;
};

struct CWin32Data
{
	ICWin32App*	pICWin32App;
	wstring		wndClassName;
	wstring		wndTitle;
	int			height;
	int			width;
	DWORD		windowStyle;
	BOOL		isWindowMenu;

	CWin32Data()
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
class CWin32
{
private:
	MSG			_msg;
	HWND		_HWnd;
	bool		_IsInit;
	CWin32Data	_CWin32Data;

public:
	CWin32(CWin32Data wd);
	virtual ~CWin32(void);

	RECT		GetScreenRect(void);
	HWND		GetWindow(void);
	MSG*		GetLastMsg(void);
	bool		IsInit(void);

	// Initialization of all Win32 objects & logic.
	bool		Init(void);
	void		Cleanup(void);

	// Method for clearing the Win32 message queue.
	bool		MsgQueueProc(void);

	// Default satic WNDPROC callback
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
