// CAppBase.cpp
// Impementation file for CAppBase.

#include "CAppBase.h"

#pragma region Construction/destruction
CAppBase::CAppBase(void) :
	_pCWin32(nullptr),
	_pCDirectX(nullptr),
	_pCInput(nullptr)
{
}

CAppBase::~CAppBase(void)
{
}
#pragma endregion

#pragma region Base methods
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
	bool good = true;

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
	if (good) GetCWin32()->Init();

	_pCDirectX.reset(new CDirectX());
	if(!_pCDirectX) good &= false;
	if (good) good &= _pCDirectX->Init(GetCWin32()->GetWindow(),800,600);

	_pCInput.reset(new CInput());
	if(!_pCInput) good &= false;
	if (good) good &= GetCInput()->Init();

	return good;
}

bool CAppBase::UpdateBase(void)
{
	static bool good; good = true;

	good &= _pCInput->Update();
	if(good) good &= Update();

	return good;
}

bool CAppBase::RenderBase(void)
{
	static bool good; good = true;

	good &= _pCDirectX->BeginRender();
	if (good) good &= Render();
	if (good) good &= _pCDirectX->EndRender();

	return good;
}

void CAppBase::ShutdownBase(void)
{
	Shutdown();
	_pCDirectX.release();
	_pCInput.release();
	_pCWin32.release();
}
#pragma endregion

#pragma region Object reference getters
CWin32* CAppBase::GetCWin32(void)
{
	return _pCWin32.get();
}

CDirectX* CAppBase::GetCDirectX(void)
{
	return _pCDirectX.get();
}

CInput* CAppBase::GetCInput(void)
{
	return _pCInput.get();
}
#pragma endregion

#pragma region Win32 message processing
LRESULT CALLBACK	CAppBase::ICWin32App_MsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;
	static bool handled; handled = false;

	switch(msg)
	{
	case WM_MOUSEMOVE:
		{
			GetCInput()->SetMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		} break;
	case WM_KEYDOWN:
		{
			GetCInput()->KeyDown(wParam);
			handled = true;
		} break;
	case WM_KEYUP:
		{
			GetCInput()->KeyUp(wParam);
			handled = true;
		} break;
	}

	if(!handled)
		result = DefWindowProc(hwnd, msg, wParam, lParam);
	return result;
}
#pragma endregion

#pragma region Instance methods
void	CAppBase::PostQuit(void)
{
	PostQuitMessage(0);
}
#pragma endregion