#pragma once

#include "IInitializable.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "ICleanupable.h"

class IEntity : public IInitializable, public IUpdateable, public IRenderable, public ICleanupable
{
public:

	IEntity() {}
	virtual ~IEntity(void) {};
};

