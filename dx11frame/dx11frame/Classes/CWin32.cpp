// CWin32.cpp
// Implementation file for CWin32.

#include "CWin32.h"

#pragma region Construction/Destruction
CWin32::CWin32(void) :
	_HWnd(nullptr),
	_pICWin32App(nullptr),
	_IsInit(false)
{
	ZeroMemory(&_msg, sizeof(MSG));
}
CWin32::CWin32(ICWin32App *pApp)
{
	CWin32();
	_pICWin32App = pApp;
}
CWin32::~CWin32(void)
{
	Shutdown();
}
#pragma endregion

#pragma region Initialization
HWND CWin32::Init(void)
{
	_IsInit = false;
	_HWnd = nullptr;

	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = StaticMsgProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = HINST_THISCOMPONENT;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = L"WndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	RECT rect = { 0, 0, 800, 600 };
	DWORD windowStyle =
		(WS_OVERLAPPED/* |
					  WS_CAPTION |
					  WS_SYSMENU |
					  WS_MINIMIZEBOX |
					  WS_THICKFRAME*/);
	AdjustWindowRect(
		&rect,
		windowStyle,
		false);
	int width  = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	_HWnd = CreateWindow(
		L"WndClassName",
		L"DirectX 11 Framework",
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		0,
		0,
		HINST_THISCOMPONENT,
		0);
	if( !_HWnd )
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// Store a reference to this CWin32 object in the window's GWL_USERDATA
	//	so we can call a member method to handle Win32 messages.
	SetLastError(0);
	LONG lresult = SetWindowLong(_HWnd, GWL_USERDATA, (LONG)this);
	if(lresult == 0 && GetLastError() != 0) return nullptr;

	ShowWindow(_HWnd, SW_SHOW);
	UpdateWindow(_HWnd);

	_IsInit = true;
	return _HWnd;
}

void CWin32::Shutdown(void)
{
	_pICWin32App = nullptr;
}
#pragma endregion

#pragma region Win32 message processing
bool CWin32::MsgQueueProc(void)
{
	while(PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
		if(_msg.message == WM_QUIT) return false;
	}
	return true;
}
/*static*/ LRESULT CALLBACK CWin32::StaticMsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO *>(lParam);
			pMinMaxInfo->ptMinTrackSize.x = 200;
			pMinMaxInfo->ptMinTrackSize.y = 200;
		}
	}
	// Only dispatch a message to the CWin32 object for this window
	//	if it has a reference to one in its GWL_USERDATA.
	if(GetWindowLong(hwnd, GWL_USERDATA))
		return ((CWin32*)GetWindowLong(hwnd, GWL_USERDATA))->
		MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT CALLBACK CWin32::MsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	// Only dispatch a message to the ICWin32App object if we have a reference.
	if(_pICWin32App)
		return _pICWin32App->ICWin32App_MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
#pragma endregion

#pragma region Set/Get methods
HWND	CWin32::GetWindow(void)
{
	return _HWnd;
}
MSG*	CWin32::GetLastMsg(void)
{
	return &_msg;
}
bool	CWin32::IsInit(void)
{
	return _IsInit;
}
RECT	CWin32::GetScreenRect(void)
{
	RECT	ScreenRect;
	GetClientRect(_HWnd, &ScreenRect);
	return ScreenRect;
}
#pragma endregion