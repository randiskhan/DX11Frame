// CApp.h
// Declaration file for CApp.
// "Root" class of executable.
// This class must extend CAppBase.

#pragma once

#include "CDX11Frame.h"
#include "DebugText.h"
#include "Hypotrochoid.h"

#pragma comment(lib, "dx11frame.lib")

class CApp : public CDX11Frame
{
private:
	unique_ptr<DebugText>		_pDebugText;
	unique_ptr<Hypotrochoid>	_pHypotrochoid;

public:
	CApp(void);
	virtual ~CApp(void);

	bool		PreInit(void);
	bool		PostInit(void);
	bool		Update(void);
	bool		Render(void);
	void		Cleanup(void);
};
