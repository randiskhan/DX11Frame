// CApp.cpp
// Implementation file for CApp.

#include "CApp.h"

CApp::CApp(void)
= default;

CApp::~CApp(void)
{
	CApp::cleanup();
}

bool CApp::pre_init(void)
{
	auto good = true;

	srand(static_cast<unsigned int>(time(nullptr)));  // NOLINT(cert-msc51-cpp)

#if defined(_DEBUG)
	win32_data_.width = 1600;
	win32_data_.height = 1600;
	directx_data_.start_windowed = true;
#else
	win32_data_.width = 1920;
	win32_data_.height = 1080;
	_CDirectXData.startWindowed = true;
#endif
	directx_data_.back_color = Vector4(0.1f, 0.1f, 0.1f, 1.0f);

	// Set the path to CAppBase spritefont file.
	debug_spritefont_path_ = L"assets\\debug.spritefont";

	return good;
}

bool CApp::post_init(void)
{
	auto good = true;

	_pDebugText.reset(new DebugText(this));
	if (!(_pDebugText && _pDebugText->is_init()))
		good &= false;
	_pCycloid.reset(new Cycloid(this));
	if (!(_pCycloid && _pCycloid->is_init()))
		good &= false;

	_pDebugText->set_do_update(true);
	_pCycloid->set_do_update(true);
	_pCycloid->set_do_render(true);

	return good;
}

bool CApp::update(void)
{
	auto good = true;

	// Quit application.
	if (get_input()->is_key_down(VK_ESCAPE))
		post_quit();
	// Take a screenshot. Overwrites previous screenshot.
	if (get_input()->is_key_down_since_last_frame(VK_SNAPSHOT))
		TakeScreenshot();
	// Hold spacebar to show debug text.
	if (get_input()->is_key_down(VK_SPACE))
		_pDebugText->set_do_render(true);
	else
		_pDebugText->set_do_render(false);

	if (good && _pDebugText->get_do_update()) good &= _pDebugText->update();
	if (good && _pCycloid->get_do_update()) good &= _pCycloid->update();

	return good;
}

bool CApp::render(void)
{
	auto good = true;

	get_sprite_batch()->Begin();
	if (good && _pDebugText->get_do_render()) good &= _pDebugText->render();
	if (good && _pCycloid->get_do_render()) good &= _pCycloid->render();
	get_sprite_batch()->End();

	return good;
}

void CApp::cleanup(void)
{
}

// Save the backbuffer to a .bmp file.
bool CApp::TakeScreenshot(void)
{
	auto good = true;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	auto hr = get_directx()->get_swap_chain()->GetBuffer(0, __uuidof(ID3D11Texture2D),
	                                                   reinterpret_cast<LPVOID*>(backBuffer.GetAddressOf()));
	if (SUCCEEDED(hr))
	{
		hr = SaveWICTextureToFile(get_directx()->get_context(), backBuffer.Get(),
			GUID_ContainerFormatBmp, L"screenshot.bmp");
	}
	if (FAILED(hr)) good = false;

	return good;
}