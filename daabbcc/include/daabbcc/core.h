// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once
#define DLIB_LOG_DOMAIN "DAABBCC"
#include "daabbcc/base.h"
#include "daabbcc/math_functions.h"
#include "dmsdk/dlib/log.h"

#include <stdio.h>

namespace daabbcc
{

#define B2_NULL_INDEX (-1)

// clang-format off
// Define platform
#if defined( _WIN64 )
	#define B2_PLATFORM_WINDOWS
#elif defined( __ANDROID__ )
	#define B2_PLATFORM_ANDROID
#elif defined( __linux__ )
	#define B2_PLATFORM_LINUX
#elif defined( __APPLE__ )
	#include <TargetConditionals.h>
	#if defined( TARGET_OS_IPHONE ) && !TARGET_OS_IPHONE
		#define B2_PLATFORM_MACOS
	#else
		#define B2_PLATFORM_IOS
	#endif
#elif defined( __EMSCRIPTEN__ )
	#define B2_PLATFORM_WASM
#else
	#error Unsupported platform
#endif

// Define CPU
#if defined( __x86_64__ ) || defined( _M_X64 ) || defined( __i386__ ) || defined( _M_IX86 )
	#define B2_CPU_X86_X64
#elif defined( __aarch64__ ) || defined( _M_ARM64 ) || defined( __arm__ ) || defined( _M_ARM )
	#define B2_CPU_ARM
#elif defined( __EMSCRIPTEN__ )
	#define B2_CPU_WASM
#else
	#error Unsupported CPU
#endif

// Define compiler
#if defined( __clang__ )
	#define B2_COMPILER_CLANG
#elif defined( __GNUC__ )
	#define B2_COMPILER_GCC
#elif defined( _MSC_VER )
	#define B2_COMPILER_MSVC
#endif

#if defined( B2_COMPILER_MSVC )
	#define B2_BREAKPOINT __debugbreak()
#elif defined( B2_PLATFORM_ANDROID )
	#define B2_BREAKPOINT __builtin_trap()
#elif defined( B2_PLATFORM_WASM )
	#define B2_BREAKPOINT                                                                                                            \
		do                                                                                                                           \
		{                                                                                                                            \
		}                                                                                                                            \
		while ( 0 )
#elif defined( B2_COMPILER_GCC ) || defined( B2_COMPILER_CLANG )
	#if defined( B2_CPU_X86_X64 )
		#define B2_BREAKPOINT __asm volatile( "int $0x3" )
	#elif defined( B2_CPU_ARM )
		#define B2_BREAKPOINT __builtin_trap()
	#endif
#else
	#error Unknown platform
#endif

#if !defined( NDEBUG ) || defined( B2_ENABLE_ASSERT )
	extern b2AssertFcn* b2AssertHandler;
	#define B2_ASSERT( condition )                                                                                                   \
		do                                                                                                                           \
		{                                                                                                                            \
			if ( !( condition ) && b2AssertHandler( #condition, __FILE__, (int)__LINE__ ) )                                          \
				B2_BREAKPOINT;                                                                                                       \
		}                                                                                                                            \
		while ( 0 )
#else
	#define B2_ASSERT( ... ) ( (void)0 )
#endif
// clang-format on

extern float b2_lengthUnitsPerMeter;

// Used to detect bad values. Positions greater than about 16km will have
// precision problems, so 100km as a limit should be fine in all cases.
#define b2_huge (100000.0f * b2_lengthUnitsPerMeter)

// Used to prevent the compiler from warning about unused variables
#define B2_MAYBE_UNUSED(x) ((void)(x))

// Use to validate definitions. Do not take my cookie.
#define B2_SECRET_COOKIE 1152023

#define b2CheckDef(DEF) B2_ASSERT(DEF->internalValue == B2_SECRET_COOKIE)
} // namespace daabbcc
