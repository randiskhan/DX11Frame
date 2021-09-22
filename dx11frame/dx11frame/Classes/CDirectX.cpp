// CDirectX.cpp
// Implementation file for CDirectX.

#include "CDirectX.h"

#pragma region Construction/Destruction

CDirectX::CDirectX(const CDirectXData dd) :
	_pD3D11Device(nullptr),
	_pD3D10Device(nullptr),
	_pD3D11DeviceContext(nullptr),
	_pDXGISwapChain(nullptr),
	_pD3D11Texture2D(nullptr),
	_pD3D11RenderTargetView(nullptr),
	_pD3D11DepthStencilView(nullptr),
	_pDXGIDevice(nullptr),
	_pDXGIAdapter(nullptr),
	_pDXGIFactory(nullptr),
	_CDirectXData(dd)
{
	ZeroMemory(&_ScreenViewport, sizeof(D3D11_VIEWPORT));
}

CDirectX::~CDirectX(void)
{
	cleanup();
}

void CDirectX::cleanup(void)
{
	SafeRelease(_pD3D11Device);
	if (_pD3D11DeviceContext)
		_pD3D11DeviceContext->ClearState();
	SafeRelease(_pD3D11DeviceContext);
	SafeRelease(_pDXGISwapChain);
	SafeRelease(_pD3D11Texture2D);
	SafeRelease(_pD3D11RenderTargetView);
	SafeRelease(_pD3D11DepthStencilView);
	SafeRelease(_pDXGIDevice);
	SafeRelease(_pDXGIAdapter);
	SafeRelease(_pDXGIFactory);
}

#pragma endregion

#pragma region Initialization

bool	CDirectX::Init(void)
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
		&_pD3D11Device,
		&feature_level,
		&_pD3D11DeviceContext);

	if (feature_level != D3D_FEATURE_LEVEL_11_0)
		good = false;

	DXGI_SWAP_CHAIN_DESC sd;

	if (SUCCEEDED(hr) && good)
	{
		sd.BufferDesc.Width = _CDirectXData.width;
		sd.BufferDesc.Height = _CDirectXData.height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = _CDirectXData.hwnd;
		sd.Windowed = _CDirectXData.startWindowed;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		hr = _pD3D11Device->QueryInterface(
			__uuidof(IDXGIDevice), 
			reinterpret_cast<void**>(&_pDXGIDevice));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = _pDXGIDevice->GetParent(
			__uuidof(IDXGIAdapter), 
			reinterpret_cast<void**>(&_pDXGIAdapter));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = _pDXGIAdapter->GetParent(
			__uuidof(IDXGIFactory), 
			reinterpret_cast<void**>(&_pDXGIFactory));
	}
	if (SUCCEEDED(hr) && good)
	{
		hr = _pDXGIFactory->CreateSwapChain(
			_pD3D11Device, 
			&sd, 
			&_pDXGISwapChain);
	}
	if (SUCCEEDED(hr) && good)
	{
		// Block alt-enter for fullscreen toggle, for now.
		_pDXGIFactory->MakeWindowAssociation(
			_CDirectXData.hwnd, 
			DXGI_MWA_NO_ALT_ENTER);
	}
	if (SUCCEEDED(hr) && good)
		good &= reset(_CDirectXData.width, _CDirectXData.height);

	if (FAILED(hr)) good = false;

	return _IsInit = good;

}

bool	CDirectX::reset(const int x, const int y)
{

	auto good = true;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	SafeRelease(_pD3D11RenderTargetView);
	SafeRelease(_pD3D11DepthStencilView);
	SafeRelease(_pD3D11Texture2D);

	HRESULT hr = _pDXGISwapChain->ResizeBuffers(
		1,
		x,
		y,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);
	ID3D11Texture2D* back_buffer = nullptr;
	if (SUCCEEDED(hr))
	{
		hr = _pDXGISwapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&back_buffer));
	}
	if (SUCCEEDED(hr))
	{
		hr = _pD3D11Device->CreateRenderTargetView(
			back_buffer,
			nullptr,
			&_pD3D11RenderTargetView);
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

		hr = _pD3D11Device->CreateTexture2D(
			&depth_stencil_desc,
			nullptr,
			&_pD3D11Texture2D);
	}
	if (SUCCEEDED(hr))
	{
		hr = _pD3D11Device->CreateDepthStencilView(
			_pD3D11Texture2D,
			nullptr,
			&_pD3D11DepthStencilView);
	}
	if (SUCCEEDED(hr))
	{
		_pD3D11DeviceContext->OMSetRenderTargets(
			1,
			&_pD3D11RenderTargetView,
			_pD3D11DepthStencilView);

		_ScreenViewport.TopLeftX = 0;
		_ScreenViewport.TopLeftY = 0;
		_ScreenViewport.Width = static_cast<float>(x);
		_ScreenViewport.Height = static_cast<float>(y);
		_ScreenViewport.MinDepth = 0.0f;
		_ScreenViewport.MaxDepth = 1.0f;

		_pD3D11DeviceContext->RSSetViewports(1, &_ScreenViewport);
	}
	SafeRelease(back_buffer);

	if (FAILED(hr)) good &= false;

	return _IsInit = good;

}

#pragma endregion

#pragma region Instance methods

auto CDirectX::get_device(void) const -> ID3D11Device*
{
	return _pD3D11Device;
}

ID3D11DeviceContext*	CDirectX::get_context(void) const
{
	return _pD3D11DeviceContext;
}

IDXGISwapChain*			CDirectX::get_swap_chain(void) const
{
	return _pDXGISwapChain;
}

#pragma endregion

#pragma region Begin/End rendering

bool	CDirectX::begin_render() const
{

	constexpr auto good = true;

	const float bc[4] = {
		_CDirectXData.backcolor.x,
		_CDirectXData.backcolor.y,
		_CDirectXData.backcolor.z,
		_CDirectXData.backcolor.w
	};
	_pD3D11DeviceContext->ClearRenderTargetView(
		_pD3D11RenderTargetView,
		reinterpret_cast<const float*>(bc));
	_pD3D11DeviceContext->ClearDepthStencilView(
		_pD3D11DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	return good;

}

bool	CDirectX::end_render(void) const
{

	auto good = true;

	const HRESULT hr = _pDXGISwapChain->Present(0, 0);
	if (FAILED(hr)) good = false;

	return good;

}

#pragma endregion