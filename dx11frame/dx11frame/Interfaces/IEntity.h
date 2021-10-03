#pragma once

#include "dx11_frame.h"

// ReSharper disable once CommentTypo
class i_entity :  // NOLINT(cppcoreguidelines-special-member-functions)
	public IInitializable,
	public IUpdateable,
	public IRenderable,
	public i_cleanupable
{

public:
	explicit i_entity(dx11_frame* app_base) : dx11_frame_(app_base) {}
	~i_entity() override { dx11_frame_ = nullptr; }

private:
	dx11_frame*	dx11_frame_;

protected:
	dx11_frame*	get_cdx11_frame() const { return dx11_frame_; }

};
