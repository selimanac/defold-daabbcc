// SPDX-FileCopyrightText: 2023 Erin Catto
// SPDX-License-Identifier: MIT

#include "daabbcc/math_functions.h"

namespace daabbcc
{

    bool b2IsValidFloat(float a)
    {
        if (isnan(a))
        {
            return false;
        }

        if (isinf(a))
        {
            return false;
        }

        return true;
    }

    bool b2IsValidVec2(b2Vec2 v)
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
