#pragma once
class ICleanupable
{
public:

	ICleanupable(void) {}
	virtual ~ICleanupable(void) {};

	virtual void Cleanup(void) = 0;

};

