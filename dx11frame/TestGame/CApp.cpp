// CApp.cpp
// Impementation file for CApp.

#include "CApp.h"

CApp::CApp(void)
{
}

CApp::~CApp(void)
{
	Cleanup();
}

bool CApp::PreInit(void)
{
	static bool good; good = true;

	_pTestEntity.reset(new TestEntity(this));
	if(!_pTestEntity) good &= false;

	return good;
}

bool CApp::PostInit(void)
{
	static bool good; good = true;

	if (good) good &= _pTestEntity->Init();

	return good;
}

bool CApp::Update(void)
{
	static bool good; good = true;
	if(GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();

	_pTestEntity->Update();

	return good;
}

bool CApp::Render(void)
{
	static bool good; good = true;

	GetSpriteBatch()->Begin();
	_pTestEntity->Render();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}
