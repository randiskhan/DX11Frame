#pragma once
class IUpdateable
{
public:

	IUpdateable(void) : update(true) {}
	virtual ~IUpdateable(void) {};

	virtual bool Update(void) = 0;

protected:
	bool	update;
};

