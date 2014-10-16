// CDirectX.h
// Declaration file for CDirectX.

#pragma once

#include <d3d11.h>
#include "common.h"
#include "IInitializable.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

struct CDirectXData
{
	int		width;
	int		height;
	bool	useHWndDimentions;
	HWND	hwnd;

	CDirectXData()
	{
		width = 800;
		height = 600;
		useHWndDimentions = true;
		hwnd = nullptr;
	}
};

// Class for all DirectX rendering objects.
class CDirectX : public IInitializable
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

	CDirectXData				_CDirectXData;

public:
	CDirectX(CDirectXData dd);
	virtual ~CDirectX(void);

	bool	Init(void);
	bool	Reset(int x, int y);
	void	Cleanup(void);

	bool	BeginRender(void);
	bool	EndRender(void);

	ID3D11Device*			GetDevice(void);
	ID3D11DeviceContext*	GetContext(void);
};
