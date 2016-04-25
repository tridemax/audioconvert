#pragma once

//-------------------------------------------------------------------------------------------------
/// windows api
//-------------------------------------------------------------------------------------------------
#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT		_WIN32_WINNT_WINBLUE // _WIN32_WINNT_WIN10
#define WIN32_LEAN_AND_MEAN
#include <sdkddkver.h>
#include <windows.h>
#include <psapi.h>
#include <mmsystem.h>

//-------------------------------------------------------------------------------------------------
/// crt/stl/boost
//-------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <assert.h>

#include <memory>
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <functional>

#include <boost/config.hpp>
#include <boost/variant.hpp>

//-------------------------------------------------------------------------------------------------
/// adaptation for Win32 & MSVC
//-------------------------------------------------------------------------------------------------
#undef min
#undef max

#pragma warning (disable: 4100) // unreferenced formal parameter
#pragma warning (disable: 4127) // conditional expression is constant
#pragma warning (disable: 4201) // nonstandard extension used : nameless struct / union

//-------------------------------------------------------------------------------------------------
/// third party libraries
//-------------------------------------------------------------------------------------------------
#include "allxiph.h"
#include "mp3codec.h"

//-------------------------------------------------------------------------------------------------
/// byte type declaration
//-------------------------------------------------------------------------------------------------
#ifdef byte
#undef byte
#endif

__if_not_exists(byte)
{
	typedef unsigned __int8 byte;
}

static_assert(sizeof(byte) == 1u, "Size of \"byte\" typedef not equal to 8 bit.");

//-------------------------------------------------------------------------------------------------
///
//-------------------------------------------------------------------------------------------------
constexpr uint32_t makefourcc(char ch0, char ch1, char ch2, char ch3)
{
	return (uint32_t(uint8_t(ch0)) | (uint32_t(uint8_t(ch1)) << 8) | (uint32_t(uint8_t(ch2)) << 16) | (uint32_t(uint8_t(ch3)) << 24));
}
