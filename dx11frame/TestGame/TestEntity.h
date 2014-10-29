// TestEntity.h
// Declaration file for TestEntity.

#pragma once

#include "CDX11Frame.h"
#include "IEntity.h"

class TestEntity : public IEntity
{
private:
	wstring		_msg;

public:
	TestEntity(CDX11Frame* pCDX11Frame);
	virtual ~TestEntity(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);
};
