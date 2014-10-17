//	common.h
//	Header file with code common to all files.

#pragma once

#pragma region Includes & defines

// C++ headers
#include <memory>
#include <iostream>
#include <sstream>
// DX11Frame interface headers.
#include "IInitializable.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "ICleanupable.h"
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

#if defined(_DEBUG)
#pragma comment(lib, "DirectXTK_d.lib")
#pragma comment(lib, "DirectXTKAudioDX_d.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioDX.lib")
#endif

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
	return (sin(angle) + 1.0f) / 2.0f;
}
__forceinline double NormSin(double angle)
{
	return (sin(angle) + 1.0f) / 2.0f;
}
__forceinline long double NormSin(long double angle)
{
	return (sin(angle) + 1.0f) / 2.0f;
}
#pragma endregion
