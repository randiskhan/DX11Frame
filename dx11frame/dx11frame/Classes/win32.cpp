// win32.cpp
// Implementation file for class win32.

#include "win32.h"

#pragma region Construction/Destruction
win32::win32(const win32_data wd) :
_HWnd(nullptr),
_IsInit(false)
{
	ZeroMemory(&_msg, sizeof(MSG));
	_CWin32Data = wd;
}
win32::~win32(void)
{
	Cleanup();
}
#pragma endregion

#pragma region Initialization/Cleanup
bool win32::Init(void)
{
	auto good = true;
	_HWnd = nullptr;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StaticMsgProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _CWin32Data.wndClassName.c_str();

	auto atom = RegisterClass(&wc);
	if (!atom) good &= false;

	RECT rect = { 0, 0, _CWin32Data.width, _CWin32Data.height };

	// Added the extra evaluation operator to clear warning for mixing 'bool' and 'BOOL' types.
	if (good)
		good &= (0 < AdjustWindowRect(
		&rect,
		_CWin32Data.windowStyle,
		_CWin32Data.isWindowMenu));

	if (good)
		_HWnd = CreateWindow(
		_CWin32Data.wndClassName.c_str(),
		_CWin32Data.wndTitle.c_str(),
		_CWin32Data.windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		HINST_THISCOMPONENT,
		0);
	if (!_HWnd) good &= false;

	// Store a reference to this win32 object in the window's GWL_USERDATA
	//	so we can call a member method to handle Win32 messages.
	if (good)
	{
		SetLastError(0);
		// Store a pointer to this object to use a memmber message handler.
		auto lresult = SetWindowLong(_HWnd, GWL_USERDATA, (LONG)this);
		if (lresult == 0 && GetLastError() != 0) good = false;
	}

	ShowWindow(_HWnd, SW_SHOW);
	// Removing this UpdateWindow call for now.
	//if (good) good &= UpdateWindow(_HWnd);

	return _IsInit = good;
}
void win32::Cleanup(void)
{
}
#pragma endregion

#pragma region Win32 message processing
bool win32::MsgQueueProc(void)
{
	while (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
		if (_msg.message == WM_QUIT) return false;
	}
	return true;
}
/*static*/ LRESULT CALLBACK win32::StaticMsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
		case WM_SETCURSOR:
		{
			SetCursor(nullptr);
			return 0;
		}
		// This WM_DESTROY case left here to handle messages from window destruction
		// just to be safe
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
#ifdef MEMBER_MSGPROC
	// Only dispatch a message to the win32 object for this window
	//	if it has a reference to one in its GWL_USERDATA.
	if (GetWindowLong(hwnd, GWL_USERDATA))
		return ((win32*)GetWindowLong(hwnd, GWL_USERDATA))->
		MsgProc(hwnd, msg, wParam, lParam);
	else
#endif
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT CALLBACK win32::MsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	// Only dispatch a message to the i_win32_app object if we have a reference.
	if (_CWin32Data.pICWin32App)
		return _CWin32Data.pICWin32App->msg_proc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
#pragma endregion

#pragma region Set/Get methods
HWND	win32::GetWindow(void)
{
	return _HWnd;
}
MSG*	win32::GetLastMsg(void)
{
	return &_msg;
}
bool	win32::IsInit(void)
{
	return _IsInit;
}
RECT	win32::GetScreenRect(void)
{
	RECT	ScreenRect;
	GetClientRect(_HWnd, &ScreenRect);
	return ScreenRect;
}
#pragma endregion