// CApp.h
// Declaration file for CApp.
// "Root" class of executable.
// This class must be implemented and inherit from CAppBase, as dx11frame.lib
// looks for this file and instantiates CApp.

#pragma once

#include "CAppBase.h"

#pragma comment(lib, "dx11frame.lib")

class CApp : public CAppBase
{
public:
	CApp(void);
	virtual ~CApp(void);

	bool		Init(void);
	bool		Update(void);
	bool		Render(void);
	void		Shutdown(void);
};
