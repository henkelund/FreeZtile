/**
* FreeZtile
*
* Copyright (C) 2011 Henrik Hedelund (henke.hedelund@gmail.com)
*
* This file is part of FreeZtile.
*
* FreeZtile is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* FreeZtile is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with FreeZtile. If not, see <http://www.gnu.org/licenses/>.
*
* @category FreeZtile
* @package FreeZtile
* @author Henrik Hedelund <henke.hedelund@gmail.com>
* @copyright 2011 Henrik Hedelund (henke.hedelund@gmail.com)
* @license http://www.gnu.org/licenses/gpl.html GNU GPL
* @link freeztile.org
*/

#include "CubicBezier.h"
#include <algorithm>
#include <cmath>
#include <float.h>

namespace FreeZtile {

    CubicBezier::CubicBezier() :
        Form(),
        _start(0.f, 0.f),
        _end(1.f, 0.f),
        _a(0.25f, 0.f),
        _b(0.75f, 0.f),
        _tolerance(1E-4)
    {
    }

    float CubicBezier::startValue()
    {
        return _start.val;
    }

    FreeZtile::CubicBezier* CubicBezier::setStartValue(FreeZtile::SampleValue value)
    {
        FZ_FORM_EDIT_START
        _start.val = value;
        FZ_FORM_EDIT_END
        return this;
    }

    float CubicBezier::endValue()
    {
        return _end.val;
    }

    FreeZtile::CubicBezier* CubicBezier::setEndValue(FreeZtile::SampleValue value)
    {
        FZ_FORM_EDIT_START
        _end.val = value;
        FZ_FORM_EDIT_END
        return this;
    }

    FreeZtile::FormPoint CubicBezier::a()
    {
        return _a;
    }

    FreeZtile::CubicBezier* CubicBezier::setA(
            FreeZtile::SampleInstant instant, FreeZtile::SampleValue value)
    {
        FZ_FORM_EDIT_START
        _a.val = value;
        // assert between startX and bX
        _a.ins = std::min(_b.ins, std::max(_start.ins, instant));
        FZ_FORM_EDIT_END
        return this;
    }

    FreeZtile::CubicBezier* CubicBezier::setA(FreeZtile::FormPoint a)
    {
        return setA(a.ins, a.val);
    }

    FreeZtile::FormPoint CubicBezier::b()
    {
        return _b;
    }

    FreeZtile::CubicBezier* CubicBezier::setB(
            FreeZtile::SampleInstant instant, FreeZtile::SampleValue value)
    {
        FZ_FORM_EDIT_START
        _b.val = value;
        // assert between aX and endX
        _b.ins = std::min(_end.ins, std::max(_a.ins, instant));
        FZ_FORM_EDIT_END
        return this;
    }

    FreeZtile::CubicBezier* CubicBezier::setB(FreeZtile::FormPoint b)
    {
        return setB(b.ins, b.val);
    }

    float CubicBezier::tolerance()
    {
        return _tolerance;
    }

    FreeZtile::CubicBezier* CubicBezier::setTolerance(float tolerance)
    {
        FZ_FORM_EDIT_START
        _tolerance = std::min(0.5f, std::max(FLT_MIN*2, tolerance));
        FZ_FORM_EDIT_END
        return this;
    }

    void CubicBezier::_apply(
            const FreeZtile::SampleInstant inInstants[],
            FreeZtile::SampleValue outValues[],
            unsigned int size)
    {
        float   t,
                t2,     // pow(t, 2)
                ti,     // inverted t
                ti2,    // pow(ti, 2)
                f,
                d,      // f deriv
                c0y = _start.val,
                c1y = (3*_a.val)-(3*_start.val),
                c2y = (3*_start.val)-(2*(3*_a.val))+(3*_b.val),
                c3y = _end.val-_start.val+(3*_a.val)-(3*_b.val);

        unsigned int i;
        for (i = 0; i < size; ++i) {
            t = inInstants[i]; // first approximation of t = x
            d = f = 1;
            while (std::abs(f/d) > _tolerance) {
                ti = 1 - t;
                t2 = t*t;
                ti2 = ti*ti;

                //f = (std::pow(1-t, 3)*_start.ins) +
                //    (3*std::pow(1-t, 2)*t*_a.ins) +
                //    (3*(1-t)*std::pow(t, 2)*_b.ins) +
                //    (std::pow(t, 3)*_end.ins) -
                //    inPoints[i];

                // same as above but no context switches
                f = (ti2*ti*_start.ins) +
                    (3*ti2*t*_a.ins) +
                    (3*ti*t2*_b.ins) +
                    (t2*t*_end.ins) -
                    inInstants[i];

                //d = -
                //    (3*std::pow(1-t, 2)*_start.ins) +
                //    (3*_a.ins*(1 - 4*t + 3*std::pow(t, 2))) +
                //    (3*_b.ins*(2*t - 3*std::pow(t, 2))) +
                //    (3*std::pow(t, 2)*_end.ins);

                // same as above
                d = -
                    (3*ti2*_start.ins) +
                    (3*_a.ins*(1 - 4*t + 3*t2)) +
                    (3*_b.ins*(2*t - 3*t2)) +
                    (3*t2*_end.ins);

                t -= f/d;
            }
            outValues[i] = c0y+t*(c1y+t*(c2y+t*c3y));
        }
    }

}
