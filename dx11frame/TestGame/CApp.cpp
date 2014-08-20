// CApp.cpp
// Impementation file for CApp.

#include "CApp.h"

CApp::CApp(void)
{
}

CApp::~CApp(void)
{
}

bool CApp::PreInit(void)
{
	static bool good; good = true;

	_CWin32Data.width = 400;
	_CWin32Data.height = 300;

	return good;
}

bool CApp::PostInit(void)
{
	static bool good; good = true;

	if (good) _pSpriteFontDebug.reset(new SpriteFont( GetCDirectX()->GetDevice(), L"debug.spritefont" ));
	if(!_pSpriteFontDebug) good &= false;

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
	GetSpriteDebugFont()->DrawString( GetSpriteBatch(), L"randiskhan", XMFLOAT2(10,10), Colors::White );
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
	_pSpriteFontDebug.release();
}

SpriteFont*		CApp::GetSpriteDebugFont(void)
{
	return _pSpriteFontDebug.get();
}