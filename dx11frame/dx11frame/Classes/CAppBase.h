// CAppBase.h
// Declaration file for CAppBase.

#pragma once

#include "common.h"
#include "CWin32.h"
#include "CDirectX.h"
#include "CInput.h"

// Base class for "root" class/object in executable.
class CAppBase : public ICWin32App
{
private:
	unique_ptr<CWin32>		_pCWin32;
	unique_ptr<CDirectX>	_pCDirectX;
	unique_ptr<CInput>		_pCInput;

protected:
	CWin32Data				_CWin32Data;
	CDirectXData			_CDirectXData;

public:
	CAppBase(void);
	virtual ~CAppBase(void);

	WPARAM			Run();
	bool			MainLoopIteration(void);

	// Pure virtual methods
	virtual bool	Init(void) = 0;
	virtual bool	Update(void) = 0;
	virtual bool	Render(void) = 0;
	virtual void	Shutdown(void) = 0;

	// Gets/Sets
	CWin32*			GetCWin32(void);
	CDirectX*		GetCDirectX(void);
	CInput*			GetCInput(void);

	// Win32 message handler override from implementing interface ICWin32App.
	LRESULT CALLBACK	ICWin32App_MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	bool			InitBase(void);
	bool			UpdateBase(void);
	bool			RenderBase(void);
	void			ShutdownBase(void);

	void			PostQuit(void);
};
