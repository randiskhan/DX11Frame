#pragma once

#include "dx11_frame.h"

class IEntity : public IInitializable, public IUpdateable, public IRenderable, public i_cleanupable
{
public:
	IEntity(dx11_frame* pCAppBase) : _pCDX11Frame(pCAppBase) {}
	virtual ~IEntity(void) { _pCDX11Frame = nullptr; };

private:
	dx11_frame*	_pCDX11Frame;

protected:
	dx11_frame*	GetCDX11Frame(void) { return _pCDX11Frame; }
};
