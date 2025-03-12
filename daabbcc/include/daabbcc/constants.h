// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#pragma once

namespace daabbcc
{
    extern float b2_lengthUnitsPerMeter;

// Used to detect bad values. Positions greater than about 16km will have precision
// problems, so 100km as a limit should be fine in all cases.
#define B2_HUGE (100000.0f * b2_lengthUnitsPerMeter)

#define B2_DEFAULT_CATEGORY_BITS 0x0001ULL
#define B2_DEFAULT_MASK_BITS UINT64_MAX

    enum b2TreeNodeFlags
    {
        b2_allocatedNode = 0x0001,
        b2_enlargedNode = 0x0002,
        b2_leafNode = 0x0004,
    };

} // namespace daabbcc
