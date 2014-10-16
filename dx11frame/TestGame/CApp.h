// CApp.h
// Declaration file for CApp.
// "Root" class of executable.
// This class must extend CAppBase.

#pragma once

#include "CAppBase.h"
#include "TestEntity.h"
#include "TestEntity2.h"

#pragma comment(lib, "dx11frame.lib")

class CApp : public CAppBase
{
private:
	unique_ptr<TestEntity>	_pTestEntity;
	unique_ptr<TestEntity2>	_pTestEntity2;

public:
	CApp(void);
	virtual ~CApp(void);

	bool		PreInit(void);
	bool		PostInit(void);
	bool		Update(void);
	bool		Render(void);
	void		Cleanup(void);
};
