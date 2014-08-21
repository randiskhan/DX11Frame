#pragma once
class IInitializable
{
public:

	IInitializable(void) {}
	virtual ~IInitializable(void) {};

	virtual bool Init(void) = 0;

};

