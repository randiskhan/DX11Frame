// DebugText.h
// Declaration file for DebugText.

#pragma once

#include "dx11_frame.h"
#include "i_entity.h"

class DebugText : public i_entity
{

	wstring		_msg;

public:
	DebugText(dx11_frame* pCDX11Frame);
	virtual ~DebugText(void);

	bool	init(void);
	bool	update(void);
	bool	render(void);
	void	cleanup(void);

};
