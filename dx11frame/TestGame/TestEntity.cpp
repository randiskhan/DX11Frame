// TestEntity.cpp
// Implementation file for TestEntity.

#include "TestEntity.h"

TestEntity::TestEntity(CDX11Frame* pCDX11Frame) : IEntity(pCDX11Frame)
{
	Init();
}

TestEntity::~TestEntity(void)
{
	Cleanup();
}

bool		TestEntity::Init(void)
{
	bool good = true;

	_msg = L"";

	return _IsInit = good;
}

bool		TestEntity::Update(void)
{
	bool good = true;
	static int count = 0; ++count;
	static float last = 0;
	static float fps = 0;
	static float interval = 0.5f;

	if (GetCDX11Frame()->GetCTimer()->GetTotalElapsed() > last + interval)
	{
		fps = count / interval;
		count = 0;
		last += interval;
	}

	if(DoUpdate())
	{
		_msg =
			L"Press escape to exit.\n"
#if defined(DEBUG) || defined(_DEBUG)
			+
			ToString(GetCDX11Frame()->GetCInput()->GetMouseScreenPos()->x) +
			L"," +
			ToString(GetCDX11Frame()->GetCInput()->GetMouseScreenPos()->y) +
			L" - FPS: " +
			ToString(fps)
#endif
			;
	}

	return good;
}

bool		TestEntity::Render(void)
{
	bool good = true;

	if(DoRender())
	{
		GetCDX11Frame()->DrawDebugString(
			_msg,
			XMFLOAT2(5,5),
			Colors::Yellow);
	}

	return good;
}

void		TestEntity::Cleanup(void)
{
}