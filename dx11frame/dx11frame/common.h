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
#include "i_updateable.h"
#include "i_renderable.h"
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
using namespace SimpleMath;

#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) | defined(_DEBUG)
// ReSharper disable once CommentTypo
// ReSharper disable once CppInconsistentNaming
#define _CRTDBG_MAP_ALLOC  // NOLINT(bugprone-reserved-identifier)
#include <crtdbg.h>
#endif
#pragma endregion

// Uncomment this line to enable msg_proc in dx11_frame.
//#define MEMBER_MSGPROC

namespace dx11_frame_helpers
{

	#pragma region ToString functions

	template<typename T>
	__forceinline std::wstring to_string(const T& s)
	{
		std::wostringstream oss;
		oss << s;

		return oss.str();
	}

	template<typename T>
	__forceinline T from_string(const std::wstring& s)
	{
		T x;
		std::wistringstream iss(s);
		iss >> x;

		return x;
	}

	#pragma endregion

	#pragma region Safe release & delete

	template <class T>
	__forceinline void safe_release(T& i_unk)
	{
		if (i_unk)
		{
			i_unk->Release();
			i_unk = nullptr;
		}
	}

	template <class T>
	__forceinline void safe_delete(T& obj)
	{
		delete obj;
		obj = nullptr;
	}

	#pragma endregion

	#pragma region Math helpers

	__forceinline float norm_sin(const float angle)
	{
		return (sin(angle) + 1.0f) * 0.5f;
	}

	__forceinline double norm_sin(const double angle)
	{
		return (sin(angle) + 1.0) * 0.5;
	}

	__forceinline long double norm_sin(const long double angle)
	{
		return (sin(angle) + 1.0l) * 0.5l;
	}

	__forceinline float norm_cos(const float angle)
	{
		return (cos(angle) + 1.0f) * 0.5f;
	}

	__forceinline double norm_cos(const double angle)
	{
		return (cos(angle) + 1.0) * 0.5;
	}

	__forceinline long double norm_cos(const long double angle)
	{
		return (cos(angle) + 1.0l) * 0.5l;
	}

	__forceinline float interpolate_linear(
		const float a, 
		const float b, 
		float f)
	{
		if (f < 0) f = 0;
		if (f > 1.0f) f = 1.0f;
		return (a * (1.0f - f)) + (b * f);
	}

	__forceinline double interpolate_linear(
		const double a, 
		const double b, 
		double f)
	{
		if (f < 0) f = 0;
		if (f > 1.0) f = 1.0;
		return (a * (1.0 - f)) + (b * f);
	}

	__forceinline long double interpolate_linear(
		const long double a, 
		const long double b, 
		long double f)
	{
		if (f < 0) f = 0;
		if (f > 1.0l) f = 1.0l;
		return (a * (1.0l - f)) + (b * f);
	}

	__forceinline float interpolate_cos(
		const float a, 
		const float b, 
		float f)
	{
		if (f < 0) f = 0;
		if (f > 1.0f) f = 1.0f;
		const auto fcs = 1.0f - norm_cos(f * XM_PI);
		return  (a * (1.0f - fcs)) + (b * fcs);
	}

	__forceinline double interpolate_cos(
		const double a, 
		const double b, 
		double f)
	{
		if (f < 0) f = 0;
		if (f > 1.0) f = 1.0;
		const auto fcs = 1.0 - norm_cos(f * XM_PI);
		return  (a * (1.0 - fcs)) + (b * fcs);
	}

	__forceinline long double interpolate_cos(
		const long double a, 
		const long double b, 
		long double f)
	{
		if (f < 0) f = 0;
		if (f > 1.0l) f = 1.0l;
		const auto fcs = 1.0l - norm_cos(f * XM_PI);
		return  (a * (1.0l - fcs)) + (b * fcs);
	}

	#pragma endregion

}
