// DebugText.h
// Declaration file for DebugText.

#pragma once

#include "CDX11Frame.h"
#include "IEntity.h"

class DebugText : public IEntity
{
private:
	wstring		_msg;

public:
	DebugText(CDX11Frame* pCDX11Frame);
	virtual ~DebugText(void);

	bool	Init(void);
	bool	Update(void);
	bool	Render(void);
	void	Cleanup(void);

	void	SetDoUpdate(bool);
	void	SetDoRender(bool);
};
