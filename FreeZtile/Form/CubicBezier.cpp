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

namespace FreeZtile {

    CubicBezier::CubicBezier() :
        Form(),
        _sx(0.f),   _sy(0.f),
        _ex(1.f),   _ey(0.f),
        _ax(0.25f), _ay(0.f),
        _bx(0.75f), _by(0.f),
        _c0x(0.f),  _c0y(0.f),
        _c1x(0.f),  _c1y(0.f),
        _c2x(0.f),  _c2y(0.f),
        _c3x(0.f),  _c3y(0.f),
        _xTolerance(0.001)
    {
        _reCalcCs();
    }

    float CubicBezier::startY()
    {
        return _sy;
    }

    void CubicBezier::setStartY(float y)
    {
        _sy = y;
        _reCalcCs();
    }

    float CubicBezier::endY()
    {
        return _ey;
    }

    void CubicBezier::setEndY(float y)
    {
        _ey = y;
        _reCalcCs();
    }

    FreeZtile::PointF CubicBezier::a()
    {
        return FreeZtile::PointF(_ax, _ay);
    }

    void CubicBezier::setA(float x, float y)
    {
        _ay = y;
        // assert between startX and bX
        _ax = std::min(_bx, std::max(_sx, x));
        _reCalcCs();
    }

    FreeZtile::PointF CubicBezier::b()
    {
        return FreeZtile::PointF(_bx, _by);
    }

    void CubicBezier::setB(float x, float y)
    {
        _by = y;
        // assert between startX and bX
        _bx = std::min(_ex, std::max(_ax, x));
        _reCalcCs();
    }

    float CubicBezier::tolerance()
    {
        return _xTolerance;
    }

    void CubicBezier::setTolerance(float tolerance)
    {
        _xTolerance = std::min(0.5f, std::max(0.0000001f, tolerance));
    }

    void CubicBezier::_reCalcCs()
    {
        //TODO: We need to assert thread safety between this function and xsToYs()
        _c0x = _sx;
        _c0y = _sy;
        _c1x = (3*_ax)-(3*_sx);
        _c1y = (3*_ay)-(3*_sy);
        _c2x = (3*_sx)-(2*(3*_ax))+(3*_bx);
        _c2y = (3*_sy)-(2*(3*_ay))+(3*_by);
        _c3x = _ex-_sx+(3*_ax)-(3*_bx);
        _c3y = _ey-_sy+(3*_ay)-(3*_by);
    }

    void CubicBezier::xsToYs(
            const FreeZtile::SamplePoint xs[],
            FreeZtile::SampleValue ys[],
            unsigned int size)
    {
        float x, t, up, lo, ax;
        unsigned int i;
        for (i = 0; i < size; ++i) {

            // find t for x
            // this is the tricky part and i dont have a better solution
            // than binary search right now (Newton-Raphson perhaps?)
            x = std::min(1.f, std::max(0.f, xs[i]));
            lo = 0.f; // initial upper bounds
            up = 1.f; // initial lower bounds
            t = (up + lo)/2; // initial t approx 50%
            ax = _c0x+t*(_c1x+t*(_c2x+t*_c3x)); // x from approx t

            while (std::abs(x-ax) > _xTolerance) {
                if(x > ax) lo = t; else up = t;
                t = (up + lo)/2;
                ax = _c0x+t*(_c1x+t*(_c2x+t*_c3x));
            }

            // find y for t
            ys[i] = _c0y+t*(_c1y+t*(_c2y+t*_c3y));
        }
    }

}
