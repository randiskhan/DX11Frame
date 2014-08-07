// CInput.cpp
// Implementation file for CInput.

#include "CInput.h"

#pragma region Construction/destruction
CInput::CInput(void)
{
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
	bool good = true;
	return good;
}

void CInput::Shutdown(void)
{
}
#pragma endregion
