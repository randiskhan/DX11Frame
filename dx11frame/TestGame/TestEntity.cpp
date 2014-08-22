// TestEntity.cpp
// Impementation file for TestEntity.

#include "TestEntity.h"

TestEntity::TestEntity(CAppBase* pCAppBase) : IEntity(pCAppBase),
	_msg(L"")
{
}

TestEntity::~TestEntity(void)
{
	Cleanup();
}

bool		TestEntity::Init(void)
{
	static bool good; good = true;

	return good;
}

bool		TestEntity::Update(void)
{
	static bool good; good = true;

	_msg = 
		ToString(GetFrame()->GetCInput()->GetMouseScreenPos()->x) + 
		L"," + 
		ToString(GetFrame()->GetCInput()->GetMouseScreenPos()->y);

	return good;
}

bool		TestEntity::Render(void)
{
	static bool good; good = true;

	GetFrame()->DrawDebugString(
		_msg, 
		XMFLOAT2(50,50), 
		Colors::Yellow);
	
	return good;
}

void		TestEntity::Cleanup(void)
{
}
