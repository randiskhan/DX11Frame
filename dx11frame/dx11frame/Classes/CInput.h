// CInput.h
// Declaration file for CInput.

#pragma once

#include "common.h"

// Class for all input logic and objects
class CInput
{
public:
	CInput(void);
	virtual ~CInput(void);

	bool		Init(void);
	bool		Update(void);
	void		Shutdown(void);
};

