// CInput.cpp
// Implementation file for CInput.

#include "CInput.h"

#pragma region Construction/destruction

CInput::CInput(void)
{
	_mousePos.x = 0;
	_mousePos.y = 0;
	for (int i=0;i<256;++i)
	{
		_keysCurr[i] = false;
		_keysPrev[i] = false;
		_keysThisFrameDown[i] = false;
		_keysThisFrameUp[i] = false;
	}
}

CInput::~CInput(void)
{
	Shutdown();
}

#pragma endregion

#pragma region Instance methods

bool CInput::Init(void)
{
	bool good = true;
	return good;
}

bool CInput::Update(void)
{
	static bool good; good = true;
	// Move current keyboard state to previous state, and
	//	clear the key event tracking arrays.
	for (int i=0;i<256;++i)
	{
		// Key up events override key down events in a frame to
		//	prevent a simulated "stuck" key.
		_keysPrev[i] = _keysCurr[i];
		_keysCurr[i] = _keysThisFrameDown[i];
		_keysCurr[i] = _keysThisFrameUp[i];
		_keysThisFrameDown[i] = false;
		_keysThisFrameUp[i] = false;
	}
	return good;
}

void CInput::Shutdown(void)
{
}

void CInput::SetMousePos(int x, int y)
{
	_mousePos.x = x;
	_mousePos.y = y;
}

void CInput::KeyDown(int vk)
{
	_keysThisFrameDown[vk] = true;
}

void CInput::KeyUp(int vk)
{
	_keysThisFrameUp[vk] = true;
}

#pragma endregion

#pragma region Input querying

const Point*	CInput::GetMouseScreenPos(void)
{
	return &_mousePos;
}
bool	CInput::IsKeyDown(int vk)
{
	return _keysCurr[vk];
}
bool	CInput::IsKeyUp(int vk)
{
	return !_keysCurr[vk];
}
bool	CInput::IsKeyDownSinceLastFrame(int vk)
{
	return (_keysCurr[vk] && !_keysPrev[vk]);
}
bool	CInput::IsKeyUpSinceLastFrame(int vk)
{
	return (!_keysCurr[vk] && _keysPrev[vk]);
}
bool	CInput::IsMouseButtonDown(MouseButton mb)
{
	return false;
}
bool	CInput::IsMouseButtonUp(MouseButton mb)
{
	return false;
}
bool	CInput::IsMouseButtonDownSinceLastFrame(MouseButton mb)
{
	return false;
}
bool	CInput::IsMouseButtonUpSinceLastFrame(MouseButton mb)
{
	return false;
}

#pragma endregion