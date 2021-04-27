// CInput.cpp
// Implementation file for CInput.

#include "CInput.h"

#pragma region Construction/destruction

CInput::CInput(CInputData id)
{
	_CInputData = id;
	_mousePos.x = 0;
	_mousePos.y = 0;
	for (auto i = 0; i < 256; ++i)
		_keysCurr[i] = _keysPrev[i] = 0;
}

CInput::~CInput(void)
{
	Cleanup();
}

#pragma endregion

#pragma region Instance methods

bool CInput::Init(void)
{
	auto good = true;
	return good;
}

bool CInput::Update(void)
{
	auto good = true;

	for (auto i = 0; i < 256; ++i)
		_keysPrev[i] = _keysCurr[i];

	good = (0 < GetKeyboardState(_keysCurr));

	// Removed fail check on getting mouse coordinates.
	// This was done to not exit upon a ctrl+alt+del, which causes
	// GetCursorPos() to fail.
	if (0 < GetCursorPos(&_mousePos))
		ScreenToClient(_CInputData.hwnd, &_mousePos);

	return good;
}

void CInput::Cleanup(void)
{
}

void CInput::SetMousePos(int x, int y)
{
	_mousePos.x = x;
	_mousePos.y = y;
}

#pragma endregion

#pragma region Input querying

const PPOINT	CInput::GetMouseScreenPos(void)
{
	return &_mousePos;
}
bool	CInput::IsKeyDown(int vk)
{
	return (_keysCurr[vk] & 0x80) != 0;
}
bool	CInput::IsKeyUp(int vk)
{
	return (_keysCurr[vk] & 0x80) == 0;
}
bool	CInput::IsKeyDownSinceLastFrame(int vk)
{
	return (((_keysCurr[vk] & 0x80) != 0) && ((_keysPrev[vk] & 0x80) == 0));
}
bool	CInput::IsKeyUpSinceLastFrame(int vk)
{
	return (((_keysCurr[vk] & 0x80) == 0) && ((_keysPrev[vk] & 0x80) != 0));
}
bool	CInput::IsMouseButtonLeftDown(void)
{
	return IsKeyDown(VK_LBUTTON);
}
bool	CInput::IsMouseButtonLeftUp(void)
{
	return IsKeyUp(VK_LBUTTON);
}
bool	CInput::IsMouseButtonRightDown(void)
{
	return IsKeyDown(VK_RBUTTON);
}
bool	CInput::IsMouseButtonRightUp(void)
{
	return IsKeyUp(VK_RBUTTON);
}
bool	CInput::IsMouseButtonLeftDownSinceLastFrame(void)
{
	return IsKeyDownSinceLastFrame(VK_LBUTTON);
}
bool	CInput::IsMouseButtonLeftUpSinceLastFrame(void)
{
	return IsKeyUpSinceLastFrame(VK_LBUTTON);
}
bool	CInput::IsMouseButtonRightDownSinceLastFrame(void)
{
	return IsKeyDownSinceLastFrame(VK_RBUTTON);
}
bool	CInput::IsMouseButtonRightUpSinceLastFrame(void)
{
	return IsKeyUpSinceLastFrame(VK_RBUTTON);
}

#pragma endregion