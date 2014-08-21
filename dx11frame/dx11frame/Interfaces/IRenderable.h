#pragma once
class IRenderable
{
public:

	IRenderable(void):render(true) {}
	virtual ~IRenderable(void) {};

	virtual bool Render(void) = 0;

protected:
	bool	render;
};

