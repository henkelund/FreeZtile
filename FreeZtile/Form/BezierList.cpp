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

#include "BezierList.h"

namespace FreeZtile {

    BezierList::BezierList() :
        Form(),
        std::vector<CubicBezier*>(),
        _curveShares()
    {
    }

    BezierList::~BezierList()
    {
        while (size() > 0) {
            pop_back(); // <- calls objects destructor
        }
    }

    FreeZtile::CubicBezier* BezierList::addCurve(int index)
    {
        CubicBezier *curve = new CubicBezier();
        if (index < 0 || index > size()) {
            index = size();
        }
        insert(begin() + index, curve);
        curve->setStartY(index == 0 ? 0 : at(index - 1)->endY());
        curve->setEndY(index == (size() - 1) ? 0 : at(index + 1)->startY());
        _curveShares.insert(_curveShares.begin() + index, 0.1f);
        _normalizeCurveShares();
        return curve;
    }

    void BezierList::_normalizeCurveShares()
    {
        float totalShare = 0;
        unsigned int i, size = _curveShares.size();
        for (i = 0; i < size; ++i) {
            totalShare += _curveShares[i];
        }
        for (i = 0; i < size; ++i) {
            _curveShares[i] /= totalShare;
        }
    }

    void BezierList::_apply(
        const FreeZtile::SampleInstant inInstants[],
        FreeZtile::SampleValue outValues[],
        unsigned int size)
    {
        //@todo implement
    }

}
