// CAppBase.h
//
// Declaration file for CAppBase.

#pragma once

#include "common.h"

class CAppBase
{
public:
	CAppBase(void);
	virtual ~CAppBase(void);

	WPARAM	Run();
};
