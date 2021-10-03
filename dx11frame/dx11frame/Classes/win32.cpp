// win32.cpp
// Implementation file for class win32.

#include "win32.h"

#pragma region Construction/Destruction

win32::win32(const win32_data& wd) :
h_wnd_(nullptr),
is_init_(false)
{
	ZeroMemory(&msg_, sizeof(MSG));
	win32_data_ = wd;
}

win32::~win32()
{
	cleanup();
}

#pragma endregion

#pragma region Initialization/Cleanup

bool win32::init()
{

	auto good = true;
	h_wnd_ = nullptr;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = static_msg_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HINST_THISCOMPONENT;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = win32_data_.wnd_class_name.c_str();

	const auto atom = RegisterClass(&wc);
	if (!atom) good &= false;

	RECT rect = 
	{
		0,
		0,
		win32_data_.width,
		win32_data_.height
	};

	// Added the extra evaluation operator to clear warning for mixing 'bool'
	// and 'BOOL' types.
	if (good)
		good &= (0 < AdjustWindowRect(
		&rect,
		win32_data_.window_style,
		win32_data_.is_window_menu));

	if (good)
		h_wnd_ = CreateWindow(
		win32_data_.wnd_class_name.c_str(),
		win32_data_.wnd_title.c_str(),
		win32_data_.window_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		HINST_THISCOMPONENT,
		nullptr);
	if (!h_wnd_) good &= false;

	// Store a reference to this win32 object in the window's GWL_USERDATA
	//	so we can call a member method to handle Win32 messages.
	if (good)
	{
		SetLastError(0);
		// Store a pointer to this object to use a member message handler.
		const auto l_result = 
			SetWindowLong(
				h_wnd_, 
				GWL_USERDATA, 
				reinterpret_cast<LONG>(this));
		if (l_result == 0 && GetLastError() != 0) good = false;
	}

	if (h_wnd_)
		ShowWindow(h_wnd_, SW_SHOW);

	return is_init_ = good;

}

void win32::cleanup()
{
}

#pragma endregion

#pragma region Win32 message processing

bool win32::msg_queue_proc()
{
	while (PeekMessage(
		&msg_, 
		nullptr, 
		0, 
		0, 
		PM_REMOVE))
	{
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
		if (msg_.message == WM_QUIT) 
			return false;
	}
	return true;
}

/*static*/ LRESULT CALLBACK win32::static_msg_proc(
	const HWND hwnd, // NOLINT(misc-misplaced-const)
	const UINT msg,
	const WPARAM w_param,
	const LPARAM l_param)
{

	switch (msg)
	{
		case WM_SETCURSOR:
		{
			SetCursor(nullptr);
			return 0;
		}
		// This WM_DESTROY case left here to handle messages from window
		// destruction just to be safe.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default: ;
	}
#ifdef MEMBER_MSGPROC
	// Only dispatch a message to the win32 object for this window
	//	if it has a reference to one in its GWL_USERDATA.
	if (GetWindowLong(hwnd, GWL_USERDATA))
		return ((win32*)GetWindowLong(hwnd, GWL_USERDATA))->
		MsgProc(hwnd, msg, w_param, l_param);
	else
#endif
		return DefWindowProc(hwnd, msg, w_param, l_param);

}

LRESULT CALLBACK win32::msg_proc(
	const HWND hwnd,  // NOLINT(misc-misplaced-const)
	const UINT msg,
	const WPARAM w_param,
	const LPARAM l_param) const
{
	// Only dispatch a message to the i_win32_app object if we have a
	// reference.
	if (win32_data_.i_win32_app)
		return win32_data_.i_win32_app->msg_proc(hwnd, msg, w_param, l_param);
	else
		return DefWindowProc(hwnd, msg, w_param, l_param);
}

#pragma endregion

#pragma region Set/Get methods

HWND	win32::get_window() const
{
	return h_wnd_;
}

MSG*	win32::get_last_msg()
{
	return &msg_;
}

bool	win32::is_init() const
{
	return is_init_;
}

RECT	win32::get_screen_rect() const
{
	RECT	screen_rect;
	GetClientRect(h_wnd_, &screen_rect);
	return screen_rect;
}

#pragma endregion