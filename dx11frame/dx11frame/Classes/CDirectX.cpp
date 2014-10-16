// CDirectX.cpp
// Implementation file for CDirectX.

#include "CDirectX.h"

#pragma region Construction/Destruction

CDirectX::CDirectX(CDirectXData dd) :
	_pD3D11Device(nullptr),
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
	Cleanup();
}

void CDirectX::Cleanup(void)
{
	SafeRelease(_pD3D11Device);
	if( _pD3D11DeviceContext )
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
	bool good = true;

	HRESULT hr;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&_pD3D11Device,
		&featureLevel,
		&_pD3D11DeviceContext);

	if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
		good = false;

	DXGI_SWAP_CHAIN_DESC sd;

	if(SUCCEEDED(hr) && good)
	{
		sd.BufferDesc.Width  = _CDirectXData.width;
		sd.BufferDesc.Height = _CDirectXData.height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount  = 1;
		sd.OutputWindow = _CDirectXData.hwnd;
		sd.Windowed     = true;
		sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags        = 0;
		hr = _pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&_pDXGIDevice);
	}
	if(SUCCEEDED(hr) && good)
	{
		hr = _pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&_pDXGIAdapter);
	}
	if(SUCCEEDED(hr) && good)
	{
		hr = _pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&_pDXGIFactory);
	}
	if(SUCCEEDED(hr) && good)
	{
		hr = _pDXGIFactory->CreateSwapChain(_pD3D11Device, &sd, &_pDXGISwapChain);
	}
	if(SUCCEEDED(hr) && good)
	{
		// Block alt-enter for fullscreen toggle, for now.
		_pDXGIFactory->MakeWindowAssociation(_CDirectXData.hwnd, DXGI_MWA_NO_ALT_ENTER);
	}
	if(SUCCEEDED(hr) && good)
		good &= Reset(_CDirectXData.width, _CDirectXData.height);

	if(FAILED(hr)) good = false;

	return _IsInit = good;
}

bool	CDirectX::Reset(int x, int y)
{
	bool good = true;
	HRESULT hr;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	SafeRelease(_pD3D11RenderTargetView);
	SafeRelease(_pD3D11DepthStencilView);
	SafeRelease(_pD3D11Texture2D);

	hr = _pDXGISwapChain->ResizeBuffers(
		1,
		x,
		y,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);
	ID3D11Texture2D* backBuffer = NULL;
	if(SUCCEEDED(hr))
	{
		hr = _pDXGISwapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer));
	}
	if(SUCCEEDED(hr))
	{
		hr = _pD3D11Device->CreateRenderTargetView(
			backBuffer,
			0,
			&_pD3D11RenderTargetView);
	}
	if(SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width				= x;
		depthStencilDesc.Height				= y;
		depthStencilDesc.MipLevels			= 1;
		depthStencilDesc.ArraySize			= 1;
		depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags		= 0;
		depthStencilDesc.MiscFlags			= 0;

		hr = _pD3D11Device->CreateTexture2D(
			&depthStencilDesc,
			0,
			&_pD3D11Texture2D);
	}
	if(SUCCEEDED(hr))
	{
		hr = _pD3D11Device->CreateDepthStencilView(
			_pD3D11Texture2D,
			0,
			&_pD3D11DepthStencilView);
	}
	if(SUCCEEDED(hr))
	{
		_pD3D11DeviceContext->OMSetRenderTargets(
			1,
			&_pD3D11RenderTargetView,
			_pD3D11DepthStencilView);

		_ScreenViewport.TopLeftX = 0;
		_ScreenViewport.TopLeftY = 0;
		_ScreenViewport.Width    = (float)x;
		_ScreenViewport.Height   = (float)y;
		_ScreenViewport.MinDepth = 0.0f;
		_ScreenViewport.MaxDepth = 1.0f;

		_pD3D11DeviceContext->RSSetViewports(1, &_ScreenViewport);
	}
	SafeRelease(backBuffer);

	if(FAILED(hr)) good &= false;

	return _IsInit = good;
}

#pragma endregion

#pragma region Instance methods
ID3D11Device*			CDirectX::GetDevice(void)
{
	return _pD3D11Device;
}
ID3D11DeviceContext*	CDirectX::GetContext(void)
{
	return _pD3D11DeviceContext;
}

#pragma endregion

#pragma region Begin/End rendering
bool	CDirectX::BeginRender(void)
{
	bool good = true;

	static float backcolor[4] = { 0 };
	_pD3D11DeviceContext->ClearRenderTargetView(
		_pD3D11RenderTargetView,
		reinterpret_cast<const float*>(backcolor));
	_pD3D11DeviceContext->ClearDepthStencilView(
		_pD3D11DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	return good;
}

bool	CDirectX::EndRender(void)
{
	bool good = true;
	HRESULT hr;

	hr = _pDXGISwapChain->Present(0, 0);
	if(FAILED(hr)) good = false;

	return good;
}
#pragma endregion