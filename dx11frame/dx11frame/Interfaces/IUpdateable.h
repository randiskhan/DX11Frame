#pragma once
class IUpdateable
{
protected:
	bool			_DoUpdate;

public:

	// Construction/Destruction
	IUpdateable(void) : _DoUpdate(false) {}
	virtual			~IUpdateable(void) {};

	// Pure virtual members
	virtual bool	Update(void) = 0;

	// Instance members
	void			SetDoUpdate(bool update) { _DoUpdate = update; }
	bool			GetDoUpdate(void) { return _DoUpdate; }
};
