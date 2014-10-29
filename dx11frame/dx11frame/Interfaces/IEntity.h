#pragma once

#include "CDX11Frame.h"

class IEntity : public IInitializable, public IUpdateable, public IRenderable, public ICleanupable
{
public:
	IEntity(CDX11Frame* pCAppBase) : _pCDX11Frame(pCAppBase) {}
	virtual ~IEntity(void) { _pCDX11Frame = nullptr; };

private:
	CDX11Frame*	_pCDX11Frame;

protected:
	CDX11Frame*	GetCDX11Frame(void) { return _pCDX11Frame;	}
};
