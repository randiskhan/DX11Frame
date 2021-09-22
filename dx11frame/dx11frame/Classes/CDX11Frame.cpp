// dx11frame.cpp
// Implementation file for CDX11Frame.

#include "CDX11Frame.h"

#pragma region Construction/destruction
CDX11Frame::CDX11Frame(void) :
_pCWin32(nullptr),
_pCDirectX(nullptr),
_pCInput(nullptr),
_pSpriteBatch(nullptr),
_pDebugFont(nullptr),
_DebugSpritefontPath(L"")
{
}
CDX11Frame::~CDX11Frame(void)
{
	CleanupBase();
}
#pragma endregion

#pragma region Base methods
WPARAM CDX11Frame::Run(void)
{
	WPARAM ret = 0;
	if (InitBase())
	{
		// Main loop in one statement - Woohoo!
		while (MainLoopIteration());

		// If a WM_QUIT message was received through message queue,
		//	return the wParam. Otherwise, return zero. Advised by MSDN.
		if (_pCWin32->GetLastMsg()->message == WM_QUIT)
			ret = _pCWin32->GetLastMsg()->wParam;
	}

	CleanupBase();

	return ret;
}
bool CDX11Frame::MainLoopIteration(void)
{
	auto good = true;

	// Clear the message queue.
	if (good = _pCWin32->MsgQueueProc())
	{
		good = UpdateBase();
		good &= RenderBase();
	}

	return good;
}
bool CDX11Frame::InitBase(void)
{
	auto good = true;

	if (good) good = PreInit();

	// DX11Frame object creation.
	_pCTimer.reset(new CTimer());
	if (!(_pCTimer && _pCTimer->IsInit())) good = false;

	_pCWin32.reset(new CWin32(_CWin32Data));
	if (!_pCWin32) good = false;
	if (good) good = GetCWin32()->Init();

	if (good)
	{
		// Make sure everyone is working with the same window.
		_CInputData.hwnd = GetCWin32()->GetWindow();
		_CDirectXData.hwnd = GetCWin32()->GetWindow();
		// Synchronize window and backbuffer dimentions if desired.
		if (_CDirectXData.use_hwnd_dimensions)
		{
			_CDirectXData.width = _CWin32Data.width;
			_CDirectXData.height = _CWin32Data.height;
		}
	}

	if (good) _pCDirectX.reset(new directx(_CDirectXData));
	if (!_pCDirectX) good = false;
	if (good) good = GetCDirectX()->Init();

	if (good) _pCInput.reset(new CInput(_CInputData));
	if (!_pCInput) good = false;
	if (good) good = GetCInput()->Init();

	// DirectXTK object creation
	if (good) _pSpriteBatch.reset(new SpriteBatch(GetCDirectX()->get_context()));
	if (!_pSpriteBatch) good = false;

	// Create SpriteFont for debugging, but don't exit if failed.
	if (good && (_DebugSpritefontPath.length() > 0))
		_pDebugFont.reset(new SpriteFont(GetCDirectX()->get_device(), _DebugSpritefontPath.c_str()));

	if (good) good = PostInit();

	return good;
}
bool CDX11Frame::UpdateBase(void)
{
	auto good = true;

	good &= GetCTimer()->Update();
	if (good) good = GetCInput()->Update();
	if (good) good = Update();

	return good;
}
bool CDX11Frame::RenderBase(void)
{
	auto good = true;

	good = GetCDirectX()->begin_render();
	if (good) good = Render();
	if (good) good = GetCDirectX()->end_render();

	return good;
}
void CDX11Frame::CleanupBase(void)
{
}
#pragma endregion

#pragma region Object reference getters
CWin32*			CDX11Frame::GetCWin32(void)
{
	return _pCWin32.get();
}
directx*		CDX11Frame::GetCDirectX(void)
{
	return _pCDirectX.get();
}
CInput*			CDX11Frame::GetCInput(void)
{
	return _pCInput.get();
}
CTimer*			CDX11Frame::GetCTimer(void)
{
	return _pCTimer.get();
}
SpriteBatch*	CDX11Frame::GetSpriteBatch(void)
{
	return _pSpriteBatch.get();
}
#pragma endregion

#pragma region Win32 message processing
LRESULT CALLBACK	CDX11Frame::MsgProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;
	auto handled = false;

	if (!handled)
		result = DefWindowProc(hwnd, msg, wParam, lParam);

	return result;
}
#pragma endregion

#pragma region Instance methods
void	CDX11Frame::PostQuit(void)
{
	PostQuitMessage(0);
}
void	CDX11Frame::DrawDebugString(wstring msg, XMFLOAT2 loc, FXMVECTOR color)
{
	if (_pDebugFont) _pDebugFont->DrawString(GetSpriteBatch(), msg.c_str(), loc, color);
}
#pragma endregion