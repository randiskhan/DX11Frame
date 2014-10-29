#pragma once
class IRenderable
{
protected:
	bool			_DoRender;

public:

	// Construction/Destruction
	IRenderable(void):_DoRender(true) {}
	virtual			~IRenderable(void) {};

	// Pure virtual members
	virtual bool	Render(void) = 0;

	// Instance members
	bool			DoRender(void) { return _DoRender; }
};
