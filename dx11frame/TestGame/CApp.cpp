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

	_CWin32Data.width = 1000;
	_CWin32Data.height = 750;
	_CDirectXData.startWindowed = true;
	_CDirectXData.backcolor = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	// Set the path to CAppBase spritefont file.
	_DebugSpritefontPath = L"assets\\debug.spritefont";

	return good;
}

bool CApp::PostInit(void)
{
	bool good = true;

	_pDebugText.reset(new DebugText(this));
	if (!(_pDebugText && _pDebugText->IsInit()))
		good &= false;
	_pHypotrochoid.reset(new Hypotrochoid(this));
	if (!(_pHypotrochoid && _pHypotrochoid->IsInit()))
		good &= false;

	return good;
}

bool CApp::Update(void)
{
	bool good = true;
	if (GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();

	good &= _pDebugText->Update();
	good &= _pHypotrochoid->Update();

	return good;
}

bool CApp::Render(void)
{
	bool good = true;

	GetSpriteBatch()->Begin();
	good &= _pDebugText->Render();
	good &= _pHypotrochoid->Render();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}