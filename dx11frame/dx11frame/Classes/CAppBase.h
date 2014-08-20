// CAppBase.h
// Declaration file for CAppBase.

#pragma once

// Common header.
#include "common.h"
// DX11Frame object headers.
#include "CWin32.h"
#include "CDirectX.h"
#include "CInput.h"
// DirectXTK headers.
#include "Audio.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#if defined(_DEBUG)
#pragma comment(lib, "DirectXTK_d.lib")
#pragma comment(lib, "DirectXTKAudioDX_d.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioDX.lib")
#endif

// Base class for "root" class/object in executable.
class CAppBase : public ICWin32App
{
private:
	unique_ptr<CWin32>		_pCWin32;
	unique_ptr<CDirectX>	_pCDirectX;
	unique_ptr<CInput>		_pCInput;

public:
	CAppBase(void);
	virtual ~CAppBase(void);

	WPARAM			Run();

protected:
	CWin32Data				_CWin32Data;
	CDirectXData			_CDirectXData;

	bool			MainLoopIteration(void);

	// Get DX11Frame object references.
	CWin32*			GetCWin32(void);
	CDirectX*		GetCDirectX(void);
	CInput*			GetCInput(void);

	// Win32 message handler override from implementing interface ICWin32App.
	LRESULT CALLBACK	ICWin32App_MsgProc(HWND, UINT, WPARAM, LPARAM);

	bool			InitBase(void);
	bool			UpdateBase(void);
	bool			RenderBase(void);
	void			CleanupBase(void);

	void			PostQuit(void);

	// Pure virtual methods.
	virtual bool	PreInit(void) = 0;
	virtual bool	PostInit(void) = 0;
	virtual bool	Update(void) = 0;
	virtual bool	Render(void) = 0;
	virtual void	Cleanup(void) = 0;
};
