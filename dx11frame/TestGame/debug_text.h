// debug_text.h
// Declaration file for debug_text.

#pragma once

#include "dx11_frame.h"
#include "i_entity.h"

class debug_text final : public i_entity  // NOLINT(cppcoreguidelines-special-member-functions)
{

	wstring		msg_;

public:
	explicit debug_text(dx11_frame* dx11_frame);
	~debug_text() override;

	bool	init() override;
	bool	update() override;
	bool	render() override;
	void	cleanup() override;

};
