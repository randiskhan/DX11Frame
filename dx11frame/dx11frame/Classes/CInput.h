// CInput.h
// Declaration file for CInput.

#pragma once

#include "common.h"

// Class for all input logic and objects
class CInput
{
private:
	Point	_mousePos;
	bool	_keysCurr[256];
	bool	_keysPrev[256];
	bool	_keysThisFrameDown[256];
	bool	_keysThisFrameUp[256];

protected:

public:
	CInput(void);
	virtual ~CInput(void);

	bool		Init(void);
	bool		Update(void);
	void		Shutdown(void);

	void		SetMousePos(int x, int y);
	void		KeyDown(int vk);
	void		KeyUp(int vk);

	const Point*	GetMouseScreenPos(void);
	bool			IsMouseButtonDown(MouseButton mb);
	bool			IsMouseButtonUp(MouseButton mb);
	bool			IsMouseButtonDownSinceLastFrame(MouseButton mb);
	bool			IsMouseButtonUpSinceLastFrame(MouseButton mb);

	bool			IsKeyDown(int vk);
	bool			IsKeyUp(int vk);
	bool			IsKeyDownSinceLastFrame(int vk);
	bool			IsKeyUpSinceLastFrame(int vk);
};
