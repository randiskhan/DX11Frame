//	main.cpp
//	Home of WinMain.

#include "common.h"
#include "CAppBase.h"
#include "CApp.h"

int WINAPI WinMain(
	HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int /*nCmdShow*/)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	WPARAM exitmsg = NULL;
	unique_ptr<CAppBase> pCApp(new CApp());
	if(pCApp) exitmsg = pCApp->Run();
	return exitmsg;
}