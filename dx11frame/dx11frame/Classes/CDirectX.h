//	CDirectX.h
//	Declaration file for CDirectX.

#pragma once

#include "common.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

// Class for all DirectX rendering objects.
class CDirectX
{
private:
	// Direct3D object fields
	ID3D11Device*				_pD3D11Device;
	ID3D11DeviceContext*		_pD3D11DeviceContext;
	IDXGISwapChain*				_pDXGISwapChain;
	ID3D11Texture2D*			_pD3D11Texture2D;
	ID3D11RenderTargetView*		_pD3D11RenderTargetView;
	ID3D11DepthStencilView*		_pD3D11DepthStencilView;
	IDXGIDevice*				_pDXGIDevice;
	IDXGIAdapter*				_pDXGIAdapter;
	IDXGIFactory*				_pDXGIFactory;
	D3D11_VIEWPORT				_ScreenViewport;

public:
	CDirectX(void);
	virtual ~CDirectX(void);

	bool	Init(HWND, int, int);
	bool	Reset(int x, int y);
	void	Shutdown(void);

	bool	BeginRender(void);
	bool	EndRender(void);

	ID3D11Device*			GetDevice(void);
	ID3D11DeviceContext*	GetContext(void);

};

