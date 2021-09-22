// directx.h
// Declaration file for class directx.

#pragma once

#include <d3d11.h>
#include "common.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

struct directx_data
{
	int		width;
	int		height;
	bool	start_windowed;
	bool	use_hwnd_dimensions;
	HWND	hwnd;
	Vector4	back_color;

	directx_data()
	{
		width = 800;
		height = 600;
		start_windowed = true;
		use_hwnd_dimensions = true;
		hwnd = nullptr;
		back_color = Vector4(0.0f);
	}
};

// Class for all DirectX rendering objects.
class directx final : public IInitializable  // NOLINT(cppcoreguidelines-special-member-functions)
{

	// Direct3D object fields
	ID3D11Device*				d3d11_device_;
	ID3D10Device*				d3d10_device_;
	ID3D11DeviceContext*		d3d11_device_context_;
	IDXGISwapChain*				dxgi_swap_chain_;
	ID3D11Texture2D*			d3d11_texture_2d_;
	ID3D11RenderTargetView*		d3d11_render_target_view_;
	ID3D11DepthStencilView*		d3d11_depth_stencil_view_;
	IDXGIDevice*				dxgi_device_;
	IDXGIAdapter*				dxgi_adapter_;
	IDXGIFactory*				dxgi_factory_;
	D3D11_VIEWPORT				screen_viewport_{};

	directx_data				directx_data_;

public:
	explicit directx(directx_data dd);
	~directx() override;

	bool	Init() override;
	bool	reset(int x, int y);
	void	cleanup();

	bool	begin_render() const;
	bool	end_render() const;

	ID3D11Device*			get_device() const;
	ID3D11DeviceContext*	get_context() const;
	IDXGISwapChain*			get_swap_chain() const;

};
