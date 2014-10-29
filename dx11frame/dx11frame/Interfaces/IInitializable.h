#pragma once
class IInitializable
{
protected:
	bool			_IsInit;

public:

	// Construction/Destruction
	IInitializable(void) : _IsInit(false) {}
	virtual			~IInitializable(void) {};

	// Pure virtual members
	virtual bool	Init(void) = 0;

	// Instance members
	bool			IsInit(void) { return _IsInit; }
};
