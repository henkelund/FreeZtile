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

#ifndef FREEZTILE_TYPES_H
#define FREEZTILE_TYPES_H

namespace FZ {

    /**
     * Used for sample values.
     * Will be expected to be a floating-point type
     * holding a value in the range: -1.f <= x <= 1.f
     */
    typedef float SampleValue;

    /**
     * Used for sample instant in a period.
     * Will be expected to be a floating-point type
     * holding a value in the range: 0.f <= x < 1.f
     */
    typedef float SampleInstant;

    /**
     * An instant - value mapping structure
     */
    struct FormPoint
    {
        FormPoint(SampleInstant instant = 0.f, SampleValue value = 0.f) :
            ins(instant), val(value)
        {}
        float ins, val;
    };

}

#endif // FREEZTILE_TYPES_H
