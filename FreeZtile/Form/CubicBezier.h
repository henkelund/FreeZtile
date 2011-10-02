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

#ifndef FREEZTILE_CUBICBEZIER_H
#define FREEZTILE_CUBICBEZIER_H

#include "FreeZtile/Form.h"

namespace FreeZtile {

    class CubicBezier : public FreeZtile::Form
    {
    public:
        CubicBezier();

        /**
         *
         * @param FreeZtile::SamplePoint []xs
         * @param FreeZtile::SampleValue []ys
         * @param unsigned int size
         */
        virtual void xsToYs(
                const FreeZtile::SamplePoint xs[],
                FreeZtile::SampleValue ys[],
                unsigned int size);

        /**
         *
         * @return float
         */
        float startY();

        /**
         *
         * @param float y
         */
        void setStartY(float y);

        /**
         *
         * @return float
         */
        float endY();

        /**
         *
         * @param float y
         */
        void setEndY(float y);

        /**
         *
         * @return FreeZtile::PointF
         */
        FreeZtile::PointF a();

        /**
         *
         * @param float x
         * @param float y
         */
        void setA(float x, float y);

        /**
         *
         * @return FreeZtile::PointF
         */
        FreeZtile::PointF b();

        /**
         *
         * @param float x
         * @param float y
         */
        void setB(float x, float y);

        /**
         *
         * @reurn float
         */
        float tolerance();

        /**
         *
         * @param float tolerance
         */
        void setTolerance(float tolerance);

    private:

        // X- & Y-components of start, end and control points.
        float _sx, _sy, _ex, _ey, _ax, _ay, _bx, _by;

        // constants for calculating curve.
        // should be recalculated when points change.
        float _c0y, _c1y, _c2y, _c3y;

        /**
         *
         * @var float _xTolerance
         */
        float _tolerance;

    };

}

#endif // FREEZTILE_CUBICBEZIER_H
