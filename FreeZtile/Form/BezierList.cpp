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
#include <stdlib.h>

namespace FreeZtile {

    BezierList::BezierList() :
        Form(),
        std::vector<CubicBezier*>(),
        Listener(),
        _curveShares(),
        _instantsBuffer(NULL),
        _instantsBufferSize(0)
    {
        Observer::addListsener(EVENT_FORM_EDIT_START, this);
        Observer::addListsener(EVENT_FORM_EDIT_END, this);
        Observer::addListsener(EVENT_FORM_CACHE_INVALIDATED, this);
    }

    BezierList::~BezierList()
    {
        while (size() > 0) {
            pop_back(); // <- calls objects destructor
        }
        if (_instantsBuffer != NULL) {
            free(_instantsBuffer);
        }
    }

    FreeZtile::CubicBezier* BezierList::addCurve(int index)
    {
        FZ_FORM_EDIT_START
        CubicBezier *curve = new CubicBezier();
        if (index < 0 || index > size()) {
            index = size();
        }
        insert(begin() + index, curve);
        curve->setStartValue(index == 0 ? 0 : at(index - 1)->endValue());
        curve->setEndValue(index == (size() - 1) ? 0 : at(index + 1)->startValue());
        _curveShares.insert(_curveShares.begin() + index, 1.f);
        _normalizeCurveShares();
        FZ_FORM_EDIT_END
        return curve;
    }

    void BezierList::useCache(bool useCache, bool syncChildren)
    {
        Form::useCache(useCache);
        if (syncChildren) {
            std::vector<CubicBezier*>::iterator it;
            for (it = begin(); it != end(); ++it) {
                (*it)->useCache(useCache);
            }
        }
    }

    void BezierList::recieve(const FreeZtile::Event *event)
    {
        if (event->id == EVENT_FORM_CACHE_INVALIDATED) {
            std::vector<CubicBezier*>::iterator it;
            for (it = begin(); it != end(); ++it) {
                // if child cache is invalidated ->
                // update list cache aswell
                if (event->sender == *it) {
                    _invalidateCache();
                    return;
                }
            }
        }
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
        if (_instantsBuffer == NULL || _instantsBufferSize < size) {
            if (_instantsBuffer != NULL) {
                free(_instantsBuffer);
            }
            _instantsBuffer = (SampleInstant*) malloc(
                        sizeof(SampleInstant)*(_instantsBufferSize = size));
        }

        float progress = 0.f, share;
        unsigned int i, curves = this->size(), firstInstant, lastInstant, shareInstants;
        for (i = 0; i < curves; ++i) {

            share = _curveShares[i];
            firstInstant = progress*size;
            lastInstant = (i == curves - 1 ? size :
                std::min((unsigned int) (firstInstant + share*size), size)) - 1;

            for (shareInstants = 0;
                 firstInstant + shareInstants <= lastInstant;
                 ++shareInstants) {

                _instantsBuffer[shareInstants] =
                    // distance between current instant & first relative instant
                    (inInstants[firstInstant + shareInstants] - inInstants[firstInstant])*
                    // multiplied with inverted share for normalization
                    (1.f/(inInstants[lastInstant] - inInstants[firstInstant]));
            }
            at(i)->apply(_instantsBuffer, outValues + firstInstant, shareInstants);
            progress += share;
        }
    }

}
