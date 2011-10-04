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
        _sx(0.f),   _sy(0.f),
        _ex(1.f),   _ey(0.f),
        _ax(0.25f), _ay(0.f),
        _bx(0.75f), _by(0.f),
        _tolerance(1E-08)
    {
    }

    float CubicBezier::startY()
    {
        return _sy;
    }

    void CubicBezier::setStartY(float y)
    {
        _acquire();
        _sy = y;
        _release();
    }

    float CubicBezier::endY()
    {
        return _ey;
    }

    void CubicBezier::setEndY(float y)
    {
        _acquire();
        _ey = y;
        _release();
    }

    FreeZtile::Point CubicBezier::a()
    {
        return FreeZtile::Point(_ax, _ay);
    }

    void CubicBezier::setA(float x, float y)
    {
        _acquire();
        _ay = y;
        // assert between startX and bX
        _ax = std::min(_bx, std::max(_sx, x));
        _release();
    }

    FreeZtile::Point CubicBezier::b()
    {
        return FreeZtile::Point(_bx, _by);
    }

    void CubicBezier::setB(float x, float y)
    {
        _acquire();
        _by = y;
        // assert between aX and endX
        _bx = std::min(_ex, std::max(_ax, x));
        _release();
    }

    float CubicBezier::tolerance()
    {
        return _tolerance;
    }

    void CubicBezier::setTolerance(float tolerance)
    {
        _acquire();
        _tolerance = std::min(0.5f, std::max(FLT_MIN*2, tolerance));
        _release();
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
                c0y = _sy,
                c1y = (3*_ay)-(3*_sy),
                c2y = (3*_sy)-(2*(3*_ay))+(3*_by),
                c3y = _ey-_sy+(3*_ay)-(3*_by);

        unsigned int i;
        for (i = 0; i < size; ++i) {
            t = inInstants[i]; // first approximation of t = x
            d = f = 1;
            while (std::abs(f/d) > _tolerance) {
                ti = 1 - t;
                t2 = t*t;
                ti2 = ti*ti;

                //f = (std::pow(1-t, 3)*_sx) +
                //    (3*std::pow(1-t, 2)*t*_ax) +
                //    (3*(1-t)*std::pow(t, 2)*_bx) +
                //    (std::pow(t, 3)*_ex) -
                //    inPoints[i];

                // same as above but no context switches
                f = (ti2*ti*_sx) +
                    (3*ti2*t*_ax) +
                    (3*ti*t2*_bx) +
                    (t2*t*_ex) -
                    inInstants[i];

                //d = -
                //    (3*std::pow(1-t, 2)*_sx) +
                //    (3*_ax*(1 - 4*t + 3*std::pow(t, 2))) +
                //    (3*_bx*(2*t - 3*std::pow(t, 2))) +
                //    (3*std::pow(t, 2)*_ex);

                // same as above
                d = -
                    (3*ti2*_sx) +
                    (3*_ax*(1 - 4*t + 3*t2)) +
                    (3*_bx*(2*t - 3*t2)) +
                    (3*t2*_ex);

                t -= f/d;
            }
            outValues[i] = c0y+t*(c1y+t*(c2y+t*c3y));
        }
    }

}
