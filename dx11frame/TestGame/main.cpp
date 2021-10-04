//	main.cpp
//	Entry point for executable.

#include "app.h"

int WINAPI WinMain(
	_In_		HINSTANCE	/*hInstance*/,
	_In_opt_	HINSTANCE	/*hPrevInstance*/,
	_In_		LPSTR		/*lpCmdLine*/,
	_In_		int			/*nCmdShow*/)
{

#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	WPARAM exit_msg = NULL;
	const unique_ptr<app> app_main(new app());
	if (app_main) exit_msg = app_main->run();
	return exit_msg;  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

}