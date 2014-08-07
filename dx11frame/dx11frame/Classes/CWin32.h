// CWin32.h
// Declaration file for CWin32.

#pragma once

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
	virtual	LRESULT CALLBACK ICWin32App_MsgProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM) = 0;
};

// Class to encapsulate the code for generating a window in a Win32 application.
class CWin32
{
private:
	MSG			_msg;
	HWND		_HWnd;
	bool		_IsInit;
	ICWin32App*	_pICWin32App;

public:
	CWin32(void);
	CWin32(ICWin32App* pApp);
	virtual ~CWin32(void);

	RECT		GetScreenRect(void);
	HWND		GetWindow(void);
	MSG*		GetLastMsg(void);
	bool		IsInit(void);

	// Initialization of all Win32 objects & logic.
	HWND		Init(void);
	void		Shutdown(void);

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
