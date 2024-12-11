// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#include "daabbcc/core.h"

namespace daabbcc
{

    float      b2_lengthUnitsPerMeter = 1.0f;

    static int b2DefaultAssertFcn(const char* condition, const char* fileName, int lineNumber)
    {
        dmLogError("ASSERTION: %s, %s, line %d\n", condition, fileName, lineNumber);
        // return non-zero to break to debugger
        return 1;
    }

    b2AssertFcn* b2AssertHandler = b2DefaultAssertFcn;
} // namespace daabbcc
