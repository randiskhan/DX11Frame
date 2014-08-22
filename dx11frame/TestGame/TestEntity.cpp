// TestEntity.cpp
// Impementation file for TestEntity.

#include "TestEntity.h"

TestEntity::TestEntity(CAppBase* pCAppBase) : IEntity(pCAppBase)
{
	Init();
}

TestEntity::~TestEntity(void)
{
	Cleanup();
}

bool		TestEntity::Init(void)
{
	static bool good; good = true;

	_msg = L"";

	return _IsInit = good;
}

bool		TestEntity::Update(void)
{
	static bool good; good = true;

	if(DoUpdate())
	{
		_msg = 
			ToString(GetFrame()->GetCInput()->GetMouseScreenPos()->x) + 
			L"," + 
			ToString(GetFrame()->GetCInput()->GetMouseScreenPos()->y);
	}

	return good;
}

bool		TestEntity::Render(void)
{
	static bool good; good = true;

	if(DoRender())
	{
		GetFrame()->DrawDebugString(
			_msg, 
			XMFLOAT2(20,20), 
			Colors::Yellow);
	}
	
	return good;
}

void		TestEntity::Cleanup(void)
{
}
