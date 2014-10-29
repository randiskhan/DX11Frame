#pragma once
class IUpdateable
{
protected:
	bool			_DoUpdate;

public:

	// Construction/Destruction
	IUpdateable(void) : _DoUpdate(true) {}
	virtual			~IUpdateable(void) {};

	// Pure virtual members
	virtual bool	Update(void) = 0;

	// Instance members
	bool			DoUpdate(void) { return _DoUpdate; }
};
