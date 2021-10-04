// app.cpp
// Implementation file for app.

#include "app.h"

app::app() = default;

app::~app()
{
	app::cleanup();
}

bool app::pre_init()
{

	// ReSharper disable once CppLocalVariableMayBeConst
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

bool app::post_init()
{

	auto good = true;

	debug_text_.reset(new DebugText(this));
	if (!(debug_text_ && debug_text_->is_init()))
		good &= false;
	cycloid_.reset(new cycloid(this));
	if (!(cycloid_ && cycloid_->is_init()))
		good &= false;

	debug_text_->set_do_update(true);
	cycloid_->set_do_update(true);
	cycloid_->set_do_render(true);

	return good;

}

bool app::update()
{

	auto good = true;

	// Quit application.
	if (get_input()->is_key_down(VK_ESCAPE))
		post_quit();
	// Take a screenshot. Overwrites previous screenshot.
	if (get_input()->is_key_down_since_last_frame(VK_SNAPSHOT))
		// ReSharper disable once CppExpressionWithoutSideEffects
		take_screenshot();
	// Hold spacebar to show debug text.
	if (get_input()->is_key_down(VK_SPACE))
		debug_text_->set_do_render(true);
	else
		debug_text_->set_do_render(false);

	if (good && debug_text_->get_do_update()) good &= debug_text_->update();
	if (good && cycloid_->get_do_update()) good &= cycloid_->update();

	return good;

}

bool app::render()
{

	auto good = true;

	get_sprite_batch()->Begin();
	if (good && debug_text_->get_do_render()) good &= debug_text_->render();
	if (good && cycloid_->get_do_render()) good &= cycloid_->render();
	get_sprite_batch()->End();

	return good;

}

void app::cleanup()
{
}

// Save the back buffer to a .bmp file.
bool app::take_screenshot() const
{

	auto good = true;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	auto hr = 
		get_directx()->get_swap_chain()->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
	        reinterpret_cast<LPVOID*>(back_buffer.GetAddressOf()));
	if (SUCCEEDED(hr))
	{
		hr = SaveWICTextureToFile(
			get_directx()->get_context(), 
			back_buffer.Get(),
			GUID_ContainerFormatBmp, 
			L"screenshot.bmp");
	}
	if (FAILED(hr)) good = false;

	return good;

}
