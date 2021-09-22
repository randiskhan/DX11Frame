// DebugText.h
// Declaration file for DebugText.

#pragma once

#include "dx11_frame.h"
#include "IEntity.h"

class DebugText : public IEntity
{
private:
	wstring		_msg;

public:
	DebugText(dx11_frame* pCDX11Frame);
	virtual ~DebugText(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);
};
