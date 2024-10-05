// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once
#include "daabbcc/core.h"

#include <stdatomic.h>

#if defined(B2_COMPILER_MSVC)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#else
#include <stdlib.h>
#endif

#include <stdint.h>

namespace daabbcc
{
void* b2Alloc(uint32_t size);
void b2Free(void* mem, uint32_t size);
} // namespace daabbcc
