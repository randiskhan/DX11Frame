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

	srand((unsigned int)time(NULL));

#if defined(_DEBUG)
	_CWin32Data.width = 900;
	_CWin32Data.height = 900;
	_CDirectXData.startWindowed = true;
#else
	_CWin32Data.width = 1920;
	_CWin32Data.height = 1080;
	_CDirectXData.startWindowed = false;
#endif
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
	_pCycloid.reset(new Cycloid(this));
	if (!(_pCycloid && _pCycloid->IsInit()))
		good &= false;

	_pDebugText->SetDoUpdate(true);
	_pCycloid->SetDoUpdate(true);
	_pCycloid->SetDoRender(true);

	return good;
}

bool CApp::Update(void)
{
	bool good = true;

	// Quit application.
	if (GetCInput()->IsKeyDown(VK_ESCAPE))
		PostQuit();
	// Take a screenshot. Overwrites previous screenshot.
	if (GetCInput()->IsKeyDownSinceLastFrame(VK_SNAPSHOT))
		TakeScreenshot();
	// Hold spacebar to show debug text.
	if (GetCInput()->IsKeyDown(VK_SPACE))
		_pDebugText->SetDoRender(true);
	else
		_pDebugText->SetDoRender(false);

	if (good && _pDebugText->GetDoUpdate()) good &= _pDebugText->Update();
	if (good && _pCycloid->GetDoUpdate()) good &= _pCycloid->Update();

	return good;
}

bool CApp::Render(void)
{
	bool good = true;

	GetSpriteBatch()->Begin();
	if (good && _pDebugText->GetDoRender()) good &= _pDebugText->Render();
	if (good && _pCycloid->GetDoRender()) good &= _pCycloid->Render();
	GetSpriteBatch()->End();

	return good;
}

void CApp::Cleanup(void)
{
}

// Save the backbuffer to a .bmp file.
bool CApp::TakeScreenshot(void)
{
	bool good = true;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = GetCDirectX()->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
	if (SUCCEEDED(hr))
	{
		hr = SaveWICTextureToFile(GetCDirectX()->GetContext(), backBuffer.Get(),
			GUID_ContainerFormatBmp, L"screenshot.bmp");
	}
	if (FAILED(hr)) good = false;

	return good;
}