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
	bool good = true;

	// Set the path to CAppBase spritefont file.
	_DebugStritefontPath = L"assets\\debug.spritefont";

	return good;
}

bool CApp::PostInit(void)
{
	bool good = true;

	_pTestEntity.reset(new TestEntity(this));
	if(!(_pTestEntity && _pTestEntity->IsInit()))
		good &= false;

	return good;
}

bool CApp::Update(void)
{
	bool good = true;
	if(GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();

	_pTestEntity->Update();

	return good;
}

bool CApp::Render(void)
{
	bool good = true;

	GetSpriteBatch()->Begin();
	_pTestEntity->Render();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}
