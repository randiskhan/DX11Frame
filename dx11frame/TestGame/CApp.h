// CApp.h
// Declaration file for CApp.
// "Root" class of executable.
// This class must extend CAppBase.

#pragma once

#include "CAppBase.h"
#include "TestEntity.h"

#pragma comment(lib, "dx11frame.lib")

class CApp : public CAppBase
{
private:
	unique_ptr<TestEntity>	_pTestEntity;

public:
	CApp(void);
	virtual ~CApp(void);

	bool		PreInit(void);
	bool		PostInit(void);
	bool		Update(void);
	bool		Render(void);
	void		Cleanup(void);
};
