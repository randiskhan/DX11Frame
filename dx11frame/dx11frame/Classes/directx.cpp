// directx.cpp
// Implementation file for class directx.

#include "directx.h"

using namespace dx11_frame_helpers;

#pragma region Construction/Destruction

directx::directx(const directx_data dd) :
	d3d11_device_(nullptr),
	d3d10_device_(nullptr),
	d3d11_device_context_(nullptr),
	dxgi_swap_chain_(nullptr),
	d3d11_texture_2d_(nullptr),
	d3d11_render_target_view_(nullptr),
	d3d11_depth_stencil_view_(nullptr),
	dxgi_device_(nullptr),
	dxgi_adapter_(nullptr),
	dxgi_factory_(nullptr),
	directx_data_(dd)
{
	ZeroMemory(&screen_viewport_, sizeof(D3D11_VIEWPORT));
}

directx::~directx(void)
{
	cleanup();
}

void directx::cleanup(void)
{
	safe_release(d3d11_device_);
	if (d3d11_device_context_)
		d3d11_device_context_->ClearState();
	safe_release(d3d11_device_context_);
	safe_release(dxgi_swap_chain_);
	safe_release(d3d11_texture_2d_);
	safe_release(d3d11_render_target_view_);
	safe_release(d3d11_depth_stencil_view_);
	safe_release(dxgi_device_);
	safe_release(dxgi_adapter_);
	safe_release(dxgi_factory_);
}

#pragma endregion

#pragma region Initialization

bool	directx::init(void)
{

	auto good = true;

	HRESULT hr;

	UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_level;
	hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		create_device_flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&d3d11_device_,
		&feature_level,
		&d3d11_device_context_);

	if (feature_level != D3D_FEATURE_LEVEL_11_0)
		good = false;

	DXGI_SWAP_CHAIN_DESC sd;

	if (SUCCEEDED(hr) && good)
	{
		sd.BufferDesc.Width = directx_data_.width;
		sd.BufferDesc.Height = directx_data_.height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = directx_data_.hwnd;
		sd.Windowed = directx_data_.start_windowed;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		hr = d3d11_device_->QueryInterface(
			__uuidof(IDXGIDevice), 
			reinterpret_cast<void**>(&dxgi_device_));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = dxgi_device_->GetParent(
			__uuidof(IDXGIAdapter), 
			reinterpret_cast<void**>(&dxgi_adapter_));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = dxgi_adapter_->GetParent(
			__uuidof(IDXGIFactory), 
			reinterpret_cast<void**>(&dxgi_factory_));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = dxgi_factory_->CreateSwapChain(
			d3d11_device_, 
			&sd, 
			&dxgi_swap_chain_);
	}
	if (SUCCEEDED(hr) && good)
	{
		// Block alt-enter for fullscreen toggle, for now.
		dxgi_factory_->MakeWindowAssociation(
			directx_data_.hwnd, 
			DXGI_MWA_NO_ALT_ENTER);
	}
	if (SUCCEEDED(hr) && good)
		good &= reset(directx_data_.width, directx_data_.height);

	if (FAILED(hr)) good = false;

	return is_init_ = good;

}

bool	directx::reset(const int x, const int y)
{

	auto good = true;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	safe_release(d3d11_render_target_view_);
	safe_release(d3d11_depth_stencil_view_);
	safe_release(d3d11_texture_2d_);

	HRESULT hr = dxgi_swap_chain_->ResizeBuffers(
		1,
		x,
		y,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);
	ID3D11Texture2D* back_buffer = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = dxgi_swap_chain_->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&back_buffer));
	}
	if (SUCCEEDED(hr))
	{
		hr = d3d11_device_->CreateRenderTargetView(
			back_buffer,
			nullptr,
			&d3d11_render_target_view_);
	}
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC depth_stencil_desc;

		depth_stencil_desc.Width = x;
		depth_stencil_desc.Height = y;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
		depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.CPUAccessFlags = 0;
		depth_stencil_desc.MiscFlags = 0;

		hr = d3d11_device_->CreateTexture2D(
			&depth_stencil_desc,
			nullptr,
			&d3d11_texture_2d_);
	}
	if (SUCCEEDED(hr))
	{
		hr = d3d11_device_->CreateDepthStencilView(
			d3d11_texture_2d_,
			nullptr,
			&d3d11_depth_stencil_view_);
	}
	if (SUCCEEDED(hr))
	{
		d3d11_device_context_->OMSetRenderTargets(
			1,
			&d3d11_render_target_view_,
			d3d11_depth_stencil_view_);

		screen_viewport_.TopLeftX = 0;
		screen_viewport_.TopLeftY = 0;
		screen_viewport_.Width = static_cast<float>(x);
		screen_viewport_.Height = static_cast<float>(y);
		screen_viewport_.MinDepth = 0.0f;
		screen_viewport_.MaxDepth = 1.0f;

		d3d11_device_context_->RSSetViewports(1, &screen_viewport_);
	}
	safe_release(back_buffer);

	if (FAILED(hr)) good &= false;

	return is_init_ = good;

}

#pragma endregion

#pragma region Instance methods

auto directx::get_device(void) const -> ID3D11Device*
{
	return d3d11_device_;
}

ID3D11DeviceContext*	directx::get_context(void) const
{
	return d3d11_device_context_;
}

IDXGISwapChain*			directx::get_swap_chain(void) const
{
	return dxgi_swap_chain_;
}

#pragma endregion

#pragma region Begin/End rendering

bool	directx::begin_render() const
{

	constexpr auto good = true;

	const float bc[4] = {
		directx_data_.back_color.x,
		directx_data_.back_color.y,
		directx_data_.back_color.z,
		directx_data_.back_color.w
	};
	d3d11_device_context_->ClearRenderTargetView(
		d3d11_render_target_view_,
		reinterpret_cast<const float*>(bc));
	d3d11_device_context_->ClearDepthStencilView(
		d3d11_depth_stencil_view_,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	return good;

}

bool	directx::end_render(void) const
{

	auto good = true;

	const HRESULT hr = dxgi_swap_chain_->Present(0, 0);
	if (FAILED(hr)) good = false;

	return good;

}

#pragma endregion