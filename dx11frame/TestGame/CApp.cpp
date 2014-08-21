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

	return good;
}

bool CApp::PostInit(void)
{
	static bool good; good = true;

	return good;
}

bool CApp::Update(void)
{
	static bool good; good = true;
	if(GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();

	return good;
}

bool CApp::Render(void)
{
	static bool good; good = true;

	GetSpriteBatch()->Begin();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}
