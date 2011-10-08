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

#ifndef FREEZTILE_BEZIERLIST_H
#define FREEZTILE_BEZIERLIST_H

#include <vector>
#include "FreeZtile/Form/CubicBezier.h"

namespace FreeZtile {

    class BezierList : public FreeZtile::Form, private std::vector<CubicBezier*>
    {
    public:

        /**
         * Constructor
         *
         */
        BezierList();

        /**
         * Destructor
         *
         */
        virtual ~BezierList();

        /**
         *
         * @param int Position
         * @return FreeZtile::CubicBezier* The new curve
         */
        FreeZtile::CubicBezier* addCurve(int index = -1);

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

    private:

        /**
         *
         * @var std::vector<float>
         */
        std::vector<float>  _curveShares;

        /**
         *
         * @var SampleInstant*
         */
        SampleInstant       *_instantsBuffer;

        /**
         *
         * @var unsigned int
         */
        unsigned int        _instantsBufferSize;

        /**
         *
         */
        void _normalizeCurveShares();
    };

}

#endif // FREEZTILE_BEZIERLIST_H
