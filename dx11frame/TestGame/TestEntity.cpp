// TestEntity.cpp
// Impementation file for TestEntity.

#include "TestEntity.h"

TestEntity::TestEntity(CAppBase* pCAppBase) : IEntity(pCAppBase)
{
}

TestEntity::~TestEntity(void)
{
	Cleanup();
}

bool		TestEntity::Init(void)
{
	static bool good; good = true;

	_pDebugFont.reset(new SpriteFont(GetFrame()->GetCDirectX()->GetDevice(),L"debug.spritefont"));
	if(!_pDebugFont) good &= false;

	return good;
}

bool		TestEntity::Update(void)
{
	static bool good; good = true;
	return good;
}

bool		TestEntity::Render(void)
{
	static bool good; good = true;
	DrawDebugString(L"randiskhan", XMFLOAT2(10,10), Colors::Yellow);
	return good;
}

void		TestEntity::Cleanup(void)
{
}

void		TestEntity::DrawDebugString(wstring msg, XMFLOAT2 loc, FXMVECTOR color)
{
	_pDebugFont->DrawString(GetFrame()->GetSpriteBatch(), msg.c_str(), loc, color);
}