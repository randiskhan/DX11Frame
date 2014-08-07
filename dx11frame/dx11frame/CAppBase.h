// CAppBase.h
// Declaration file for CAppBase.

#pragma once

#include "common.h"
#include "CWin32.h"

class CAppBase : public ICWin32App
{
private:
	unique_ptr<CWin32>	_pCWin32;

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
	HWND			GetWindow(void);

	// Win32 message handler override from implementing interface ICWin32App.
	LRESULT CALLBACK	ICWin32App_MsgProc(HWND, UINT, WPARAM, LPARAM);

protected:
	bool			InitBase(void);
	bool			UpdateBase(void);
	bool			RenderBase(void);
	void			ShutdownBase(void);
};
