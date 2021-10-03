//	common.h
//	Header file with code common to all files.

#pragma once

#pragma region Includes & defines

// C/C++ headers
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
// Windows headers
#include <wincodec.h> // Needed for DirectXTK ScreenGrab.
// DX11Frame interface headers.
#include "i_initializable.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "i_cleanupable.h"
// DirectXTK headers.
#include "Audio.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "DirectXTK.lib")
//#pragma comment(lib, "DirectXTKAudioWin8.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#pragma endregion

// Uncomment this line to enable MsgProc in CAppBase.
//#define MEMBER_MSGPROC

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
	return (sin(angle) + 1.0f) * 0.5f;
}
__forceinline double NormSin(double angle)
{
	return (sin(angle) + 1.0) * 0.5;
}
__forceinline long double NormSin(long double angle)
{
	return (sin(angle) + 1.0l) * 0.5l;
}
__forceinline float NormCos(float angle)
{
	return (cos(angle) + 1.0f) * 0.5f;
}
__forceinline double NormCos(double angle)
{
	return (cos(angle) + 1.0) * 0.5;
}
__forceinline long double NormCos(long double angle)
{
	return (cos(angle) + 1.0l) * 0.5l;
}
__forceinline float InterpolateLinear(float a, float b, float f)
{
	if (f < 0) f = 0;
	if (f > 1.0f) f = 1.0f;
	return (a * (1.0f - f)) + (b * f);
}
__forceinline double InterpolateLinear(double a, double b, double f)
{
	if (f < 0) f = 0;
	if (f > 1.0) f = 1.0;
	return (a * (1.0 - f)) + (b * f);
}
__forceinline long double InterpolateLinear(long double a, long double b, long double f)
{
	if (f < 0) f = 0;
	if (f > 1.0l) f = 1.0l;
	return (a * (1.0l - f)) + (b * f);
}
__forceinline float InterpolateCos(float a, float b, float f)
{
	if (f < 0) f = 0;
	if (f > 1.0f) f = 1.0f;
	auto fcs = 1.0f - NormCos(f * XM_PI);
	return  (a * (1.0f - fcs)) + (b * fcs);
}
__forceinline double InterpolateCos(double a, double b, double f)
{
	if (f < 0) f = 0;
	if (f > 1.0) f = 1.0;
	auto fcs = 1.0 - NormCos(f * XM_PI);
	return  (a * (1.0 - fcs)) + (b * fcs);
}
__forceinline long double InterpolateCos(long double a, long double b, long double f)
{
	if (f < 0) f = 0;
	if (f > 1.0l) f = 1.0l;
	auto fcs = 1.0l - NormCos(f * XM_PI);
	return  (a * (1.0l - fcs)) + (b * fcs);
}
#pragma endregion
