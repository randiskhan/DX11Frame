// CAppBase.cpp
// Impementation file for CAppBase.

#include "CAppBase.h"

CAppBase::CAppBase(void) :
	_pCWin32(nullptr)
{
}

CAppBase::~CAppBase(void)
{
}

WPARAM CAppBase::Run(void)
{
	WPARAM ret = 0;
	if(InitBase() && Init())
	{
		// Main loop in one statement - Woohoo!
		while(MainLoopIteration());

		// If a WM_QUIT message was received through message queue,
		//	return the wParam. Otherwise, return zero. Advised by MSDN.
		if (_pCWin32->GetLastMsg()->message == WM_QUIT)
			ret = _pCWin32->GetLastMsg()->wParam;
	}
	return ret;
}

bool CAppBase::MainLoopIteration(void)
{
	static bool good = true;

	good = true;

	// Clear the message queue.
	if (good &= _pCWin32->MsgQueueProc())
	{
		good &= UpdateBase();
		good &= RenderBase();
	}

	return good;
}

bool CAppBase::InitBase(void)
{
	bool good = true;

	_pCWin32.reset(new CWin32(this));
	if(!_pCWin32) good &= false;
	if (good) _pCWin32->Init();

	_pCDirectX.reset(new CDirectX());
	if(!_pCDirectX) good &= false;
	if (good) _pCDirectX->Init(GetWindow(),800,600);

	return good;
}

bool CAppBase::UpdateBase(void)
{
	return Update();
}

bool CAppBase::RenderBase(void)
{
	bool good = true;

	good &= _pCDirectX->BeginRender();
	if (good) good &= Render();
	if (good) good &= _pCDirectX->EndRender();

	return good;
}

void CAppBase::ShutdownBase(void)
{
	Shutdown();
	_pCDirectX.release();
	_pCWin32.release();
}

HWND CAppBase::GetWindow(void)
{
	return _pCWin32->GetWindow();
}

#pragma region Win32 message processing
LRESULT CALLBACK	CAppBase::ICWin32App_MsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;
	bool handled = false;

	if(!handled)
		result = DefWindowProc(hwnd, msg, wParam, lParam);
	return result;
}
#pragma endregion