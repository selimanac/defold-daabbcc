// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

#define DLIB_LOG_DOMAIN "DAABBCC"

#include <stdio.h>

namespace daabbcc
{
    // clang-format off
#define B2_NULL_INDEX ( -1 )

// for performance comparisons
#define B2_RESTRICT restrict


// Define platform
#if defined(_WIN32) || defined(_WIN64)
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
	#define B2_PLATFORM_UNKNOWN
#endif
// clang-format on

// Used to prevent the compiler from warning about unused variables
#define B2_MAYBE_UNUSED(x) ((void)(x))

    void* b2Alloc(int size);
    void  b2Free(void* mem, int size);

} // namespace daabbcc
