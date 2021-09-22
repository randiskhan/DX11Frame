// CDirectX.h
// Declaration file for CDirectX.

#pragma once

#include <d3d11.h>
#include "common.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

struct CDirectXData
{
	int		width;
	int		height;
	bool	startWindowed;
	bool	useHWndDimentions;
	HWND	hwnd;
	Vector4	backcolor;

	CDirectXData()
	{
		width = 800;
		height = 600;
		startWindowed = true;
		useHWndDimentions = true;
		hwnd = nullptr;
		backcolor = Vector4(0.0f);
	}
};

// Class for all DirectX rendering objects.
class CDirectX : public IInitializable
{
private:
	// Direct3D object fields
	ID3D11Device*				_pD3D11Device;
	ID3D10Device*				_pD3D10Device;
	ID3D11DeviceContext*		_pD3D11DeviceContext;
	IDXGISwapChain*				_pDXGISwapChain;
	ID3D11Texture2D*			_pD3D11Texture2D;
	ID3D11RenderTargetView*		_pD3D11RenderTargetView;
	ID3D11DepthStencilView*		_pD3D11DepthStencilView;
	IDXGIDevice*				_pDXGIDevice;
	IDXGIAdapter*				_pDXGIAdapter;
	IDXGIFactory*				_pDXGIFactory;
	D3D11_VIEWPORT				_ScreenViewport{};

	CDirectXData				_CDirectXData;

public:
	CDirectX(CDirectXData dd);
	virtual ~CDirectX(void);

	bool	Init(void);
	bool	reset(int x, int y);
	void	cleanup(void);

	bool	begin_render(void) const;
	bool	end_render(void) const;

	ID3D11Device*			get_device(void) const;
	ID3D11DeviceContext*	get_context(void) const;
	IDXGISwapChain*			get_swap_chain(void) const;
};
