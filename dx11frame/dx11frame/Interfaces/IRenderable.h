#pragma once
class IRenderable
{
protected:
	bool			_DoRender;

public:

	// Construction/Destruction
	IRenderable(void) :_DoRender(false) {}
	virtual			~IRenderable(void) {};

	// Pure virtual members
	virtual bool	Render(void) = 0;

	// Instance members
	void			SetDoRender(bool render) { _DoRender = render; }
	bool			GetDoRender(void) { return _DoRender; }
};
