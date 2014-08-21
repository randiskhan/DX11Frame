#pragma once

//#include "IInitializable.h"
//#include "IUpdateable.h"
//#include "IRenderable.h"
//#include "ICleanupable.h"
#include "CAppBase.h"

class IEntity : public IInitializable, public IUpdateable, public IRenderable, public ICleanupable
{
public:
	IEntity(CAppBase* pCAppBase) : _pCAppBase(pCAppBase) {}
	virtual ~IEntity(void) {};

private:
	CAppBase*	_pCAppBase;

protected:
	CAppBase*	GetFrame(void) { return _pCAppBase;	}

};

