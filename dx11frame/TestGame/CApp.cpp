// CApp.cpp
// Implementation file for CApp.

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

#if defined(DEBUG) || defined(_DEBUG)
	_CWin32Data.width = 1000;
	_CWin32Data.height = 750;
	_CDirectXData.startWindowed = true;
#else
	_CWin32Data.width = GetSystemMetrics(SM_CXFULLSCREEN);
	_CWin32Data.height = GetSystemMetrics(SM_CYFULLSCREEN);
	_CDirectXData.startWindowed = false;
#endif
	_CDirectXData.backcolor = Vector4(0.1f,0.1f,0.1f,1.0f);


	// Set the path to CAppBase spritefont file.
	_DebugSpritefontPath = L"assets\\debug.spritefont";

	return good;
}

bool CApp::PostInit(void)
{
	bool good = true;

	_pTestEntity.reset(new TestEntity(this));
	if(!(_pTestEntity && _pTestEntity->IsInit()))
		good &= false;
	_pTestEntity2.reset(new TestEntity2(this));
	if(!(_pTestEntity2 && _pTestEntity2->IsInit()))
		good &= false;

	return good;
}

bool CApp::Update(void)
{
	bool good = true;
	if(GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();

	good &= _pTestEntity->Update();
	good &= _pTestEntity2->Update();

	return good;
}

bool CApp::Render(void)
{
	bool good = true;

	GetSpriteBatch()->Begin();
	good &= _pTestEntity->Render();
	good &= _pTestEntity2->Render();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}
