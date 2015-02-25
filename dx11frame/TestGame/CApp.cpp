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
	_CWin32Data.width = 800;
	_CWin32Data.height = 800;
	_CDirectXData.startWindowed = true;
#else
	_CWin32Data.width = 1280;
	_CWin32Data.height = 1024;
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
	_pHypotrochoid.reset(new Hypotrochoid(this));
	if (!(_pHypotrochoid && _pHypotrochoid->IsInit()))
		good &= false;

#if defined(_DEBUG)
	_pDebugText->SetDoUpdate(true);
	_pDebugText->SetDoRender(true);
#else
	_pDebugText->SetDoUpdate(false);
	_pDebugText->SetDoRender(false);
#endif

	return good;
}

bool CApp::Update(void)
{
	bool good = true;

	if (GetCInput()->IsKeyDown(VK_ESCAPE)) PostQuit();
	if (GetCInput()->IsKeyDownSinceLastFrame(VK_SPACE)) TakeScreenshot();

	if (good && _pDebugText->GetDoUpdate()) good &= _pDebugText->Update();
	if (good && _pHypotrochoid->GetDoUpdate()) good &= _pHypotrochoid->Update();

	return good;
}

bool CApp::Render(void)
{
	bool good = true;

	GetSpriteBatch()->Begin();
	if (good && _pDebugText->GetDoRender()) good &= _pDebugText->Render();
	if (good && _pHypotrochoid->GetDoRender()) good &= _pHypotrochoid->Render();
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