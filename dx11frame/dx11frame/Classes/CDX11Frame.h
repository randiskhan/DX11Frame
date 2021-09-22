// dx11frame.h
// Declaration file for CDX11Frame.

#pragma region preprocessor directives

#pragma once

// Common header.
#include "common.h"
// DX11Frame object headers.
#include "CWin32.h"
#include "CDirectX.h"
#include "CInput.h"
#include "CTimer.h"

#pragma endregion

// Base class for "root" class/object in executable.
#ifdef MEMBER_MSGPROC
class CDX11Frame : public ICWin32App
#else
class CDX11Frame
#endif
{
private:
	// DX11Frame primary objects.
	unique_ptr<CWin32>		_pCWin32;
	unique_ptr<directx>	_pCDirectX;
	unique_ptr<CInput>		_pCInput;
	unique_ptr<CTimer>		_pCTimer;
	// DirectXTK objects.
	unique_ptr<SpriteBatch>	_pSpriteBatch;
	unique_ptr<SpriteFont>	_pDebugFont;

public:
	CDX11Frame(void);
	virtual ~CDX11Frame(void);

	WPARAM			Run();

	// Get DX11Frame object references.
	CWin32*			GetCWin32(void);
	directx*		GetCDirectX(void);
	CInput*			GetCInput(void);
	CTimer*			GetCTimer(void);
	// Get DirectXTK object references.
	SpriteBatch*	GetSpriteBatch(void);

	void			DrawDebugString(wstring msg, XMFLOAT2 loc, FXMVECTOR color);

protected:
	CWin32Data		_CWin32Data;
	directx_data	_CDirectXData;
	CInputData		_CInputData;
	wstring			_DebugSpritefontPath;

	void			PostQuit(void);

	// Pure virtual methods.
	virtual bool	PreInit(void) = 0;
	virtual bool	PostInit(void) = 0;
	virtual bool	Update(void) = 0;
	virtual bool	Render(void) = 0;
	virtual void	Cleanup(void) = 0;

private:
	bool			MainLoopIteration(void);

	// Win32 message handler basic implementation. Available for overriding in CAppBase child classes.
	virtual LRESULT CALLBACK	MsgProc(HWND, UINT, WPARAM, LPARAM);

	bool			InitBase(void);
	bool			UpdateBase(void);
	bool			RenderBase(void);
	void			CleanupBase(void);
};
