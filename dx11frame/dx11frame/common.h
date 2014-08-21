//	common.h
//	Header file with code common to all files.

#pragma once

#pragma region Includes & defines

#include <memory>
#include <iostream>
#include <sstream>

using namespace std;

#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#pragma endregion

#pragma region ToString functions
template<typename T>
__forceinline std::wstring ToString(const T& s)
{
	std::wostringstream oss;
	oss << s;

	return oss.str();
}

template<typename T>
__forceinline T FromString(const std::wstring& s)
{
	T x;
	std::wistringstream iss(s);
	iss >> x;

	return x;
}
#pragma endregion

#pragma region Safe release & delete
template <class T>
__forceinline void SafeRelease(T& IUnk)
{
	if (IUnk)
	{
		IUnk->Release();
		IUnk = nullptr;
	}
}

template <class T>
__forceinline void SafeDelete(T& Obj)
{
	delete Obj;
	Obj = nullptr;
}
#pragma endregion

#pragma region Math helpers
__forceinline float NormSin(float angle)
{
	return (sin(angle) + 1.0f) / 2.0f;
}
#pragma endregion
