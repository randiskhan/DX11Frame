// TestEntity.h
// Declaration file for TestEntity.

#pragma once

#include "CAppBase.h"
#include "IEntity.h"

class TestEntity : public IEntity
{
private:
	unique_ptr<SpriteFont>	_pDebugFont;
	wstring					_msg;

public:
	TestEntity(CAppBase* pCAppBase);
	virtual ~TestEntity(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);

protected:

	void	DrawDebugString(wstring msg, XMFLOAT2 loc, FXMVECTOR color);
};

