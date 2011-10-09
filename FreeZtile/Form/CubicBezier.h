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
#include "FreeZtile/Types.h"

namespace FreeZtile {

    class CubicBezier : public FreeZtile::Form
    {
    public:

        /**
         * Constructor
         *
         */
        CubicBezier();

    protected:

        /**
         *
         * @param FreeZtile::SampleInstant[] inInstants
         * @param FreeZtile::SampleValue[] outValues
         * @param unsigned int size
         */
        virtual void _apply(
                const FreeZtile::SampleInstant inInstants[],
                FreeZtile::SampleValue outValues[],
                unsigned int size);

    public:

        /**
         *
         * @return FreeZtile::SampleValue
         */
        float startValue();

        /**
         *
         * @param FreeZtile::SampleValue value
         */
        void setStartValue(FreeZtile::SampleValue value);

        /**
         *
         * @return FreeZtile::SampleValue
         */
        float endValue();

        /**
         *
         * @param FreeZtile::SampleValue value
         */
        void setEndValue(FreeZtile::SampleValue value);

        /**
         *
         * @return FreeZtile::Point
         */
        FreeZtile::FormPoint a();

        /**
         *
         * @param FreeZtile::SampleInstant instant
         * @param FreeZtile::SampleValue value
         */
        void setA(FreeZtile::SampleInstant instant, FreeZtile::SampleValue value);

        /**
         *
         * @param FreeZtile::FormPoint a
         */
        void setA(FreeZtile::FormPoint a);

        /**
         *
         * @return FreeZtile::Point
         */
        FreeZtile::FormPoint b();

        /**
         *
         * @param FreeZtile::SampleInstant instant
         * @param FreeZtile::SampleValue value
         */
        void setB(FreeZtile::SampleInstant instant, FreeZtile::SampleValue value);

        /**
         *
         * @param FreeZtile::FormPoint b
         */
        void setB(FreeZtile::FormPoint b);

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

        /**
         * Start point
         *
         * @var FormPoint
         */
        FormPoint _start;

        /**
         * End point
         *
         * @var FormPoint
         */
        FormPoint _end;

        /**
         * First control point
         *
         * @var FormPoint
         */
        FormPoint _a;

        /**
         * Second control point
         *
         * @var FormPoint
         */
        FormPoint _b;

        /**
         *
         * @var float _xTolerance
         */
        float _tolerance;

    };

}

#endif // FREEZTILE_CUBICBEZIER_H
