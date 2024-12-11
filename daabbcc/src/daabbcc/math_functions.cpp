// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#include "daabbcc/math_functions.h"

namespace daabbcc
{
    bool b2Vec2_IsValid(b2Vec2 v)
    {
        if (isnan(v.x) || isnan(v.y))
        {
            return false;
        }

        if (isinf(v.x) || isinf(v.y))
        {
            return false;
        }

        return true;
    }
} // namespace daabbcc
