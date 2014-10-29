#pragma once
class ICleanupable
{
public:

	// Construction/Destruction
	ICleanupable(void) {}
	virtual ~ICleanupable(void) {};

	// Pure virtual members
	virtual void Cleanup(void) = 0;
};
