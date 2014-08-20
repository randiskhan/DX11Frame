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
private:
	unique_ptr<SpriteFont>	_pSpriteFontDebug;

public:
	CApp(void);
	virtual ~CApp(void);

	bool		PreInit(void);
	bool		PostInit(void);
	bool		Update(void);
	bool		Render(void);
	void		Cleanup(void);

protected:
	SpriteFont*		GetSpriteDebugFont(void);
};
