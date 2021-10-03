//	main.cpp
//	Entry point for executable.

#include "app.h"

int WINAPI WinMain(
	HINSTANCE	/*hInstance*/,
	HINSTANCE	/*hPrevInstance*/,
	LPSTR		/*lpCmdLine*/,
	int			/*nCmdShow*/)
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	WPARAM exitmsg = NULL;
	unique_ptr<app> pCApp(new app());
	if (pCApp) exitmsg = pCApp->run();
	return exitmsg;
}