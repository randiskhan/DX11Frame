// app.h
// Declaration file for app.
// "Root" class of executable.
// This class must extend dx11_frame.

#pragma once

#include "dx11_frame.h"
#include "DebugText.h"
#include "centered_trochoid.h"

#pragma comment(lib, "dx11frame.lib")

// ReSharper disable once CommentTypo
class app final : public dx11_frame  // NOLINT(cppcoreguidelines-special-member-functions)
{

	unique_ptr<DebugText>			debug_text_{};
	unique_ptr<centered_trochoid>	centered_trochoid_{};

	bool		take_screenshot() const;

public:
	app();
	~app() override;

	bool		pre_init() override;
	bool		post_init() override;
	bool		update() override;
	bool		render() override;
	void		cleanup() override;

};
