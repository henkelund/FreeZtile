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

namespace FZ {

    class CubicBezier : public FZ::Form
    {
    public:

        static const char       *EVENT_CUBICBEZIER_POINT_CHANGED;
        static const char       *EVENT_DATA_POINT_A;
        static const char       *EVENT_DATA_POINT_B;

        /**
         * Constructor
         *
         */
        CubicBezier();

        /**
         *
         * @return FZ::SampleValue
         */
        float               startValue();

        /**
         *
         * @param FZ::SampleValue value
         */
        FZ::CubicBezier*    setStartValue(FZ::SampleValue value);

        /**
         *
         * @return FZ::SampleValue
         */
        float               endValue();

        /**
         *
         * @param FZ::SampleValue value
         */
        FZ::CubicBezier*    setEndValue(FZ::SampleValue value);

        /**
         *
         * @return FZ::Point
         */
        FZ::FormPoint       a();

        /**
         *
         * @param FZ::SampleInstant instant
         * @param FZ::SampleValue value
         */
        FZ::CubicBezier*    setA(
                                FZ::SampleInstant   instant,
                                FZ::SampleValue     value);

        /**
         *
         * @param FZ::FormPoint a
         */
        FZ::CubicBezier*    setA(FZ::FormPoint a);

        /**
         *
         * @return FZ::Point
         */
        FZ::FormPoint       b();

        /**
         *
         * @param FZ::SampleInstant instant
         * @param FZ::SampleValue value
         */
        FZ::CubicBezier*    setB(
                                FZ::SampleInstant   instant,
                                FZ::SampleValue     value);

        /**
         *
         * @param FZ::FormPoint b
         */
        FZ::CubicBezier*    setB(FZ::FormPoint b);

        /**
         *
         * @reurn float
         */
        float               tolerance();

        /**
         *
         * @param float tolerance
         */
        FZ::CubicBezier*    setTolerance(float tolerance);

    protected:

        /**
         *
         * @param FZ::SampleInstant[] inInstants
         * @param FZ::SampleValue[] outValues
         * @param unsigned int size
         */
        virtual void        _apply(
                                const FZ::SampleInstant inInstants[],
                                FZ::SampleValue         outValues[],
                                unsigned int            size);

    private:

        /**
         * Start point
         *
         * @var FormPoint
         */
        FormPoint   _start;

        /**
         * End point
         *
         * @var FormPoint
         */
        FormPoint   _end;

        /**
         * First control point
         *
         * @var FormPoint
         */
        FormPoint   _a;

        /**
         * Second control point
         *
         * @var FormPoint
         */
        FormPoint   _b;

        /**
         *
         * @var float _xTolerance
         */
        float       _tolerance;

    };

}

#endif // FREEZTILE_CUBICBEZIER_H
