// CApp.h
// Declaration file for CApp.
// "Root" class of executable.
// This class must extend CAppBase.

#pragma once

#include "CDX11Frame.h"
#include "DebugText.h"
#include "Cycloid.h"

#pragma comment(lib, "dx11frame.lib")

class CApp : public dx11_frame
{
private:
	unique_ptr<DebugText>		_pDebugText;
	unique_ptr<Cycloid>			_pCycloid;

	bool		TakeScreenshot(void);

public:
	CApp(void);
	virtual ~CApp(void);

	bool		pre_init(void);
	bool		post_init(void);
	bool		update(void);
	bool		render(void);
	void		cleanup(void);
};
