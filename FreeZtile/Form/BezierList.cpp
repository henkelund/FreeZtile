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

namespace FZ {

    BezierList::BezierList() :
        Form(),
        std::vector<CubicBezier*>(),
        Subscriber(),
        _curveShares(),
        _instantsBuffer(NULL),
        _instantsBufferSize(0)
    {
    }

    BezierList::~BezierList()
    {
        Dispatcher::unsubscribe(this);
        while (size() > 0) {
            pop_back(); // <- calls objects destructor
        }
        if (_instantsBuffer != NULL) {
            free(_instantsBuffer);
        }
    }

    FZ::CubicBezier* BezierList::addCurve(int index)
    {
        FZ_FORM_EDIT_START
        CubicBezier *curve = new CubicBezier();
        if (index < 0 || index > size()) {
            index = size();
        }
        insert(begin() + index, curve);
        curve->setStartValue(
            index == 0 ?
            0 : at(index - 1)->endValue()
        )->setEndValue(
            index == (size() - 1) ?
            0 : at(index + 1)->startValue()
        );
        _curveShares.insert(_curveShares.begin() + index, 1.f/size());
        _syncHandles(index, false);
        Dispatcher::subscribe(Form::EVENT_FORM_EDIT_END, this, curve);
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

    void BezierList::recieve(const FZ::Event *event)
    {
        int index;
        if ((index = _childIndex(event->sender)) >= 0) {

            if (event->id == Form::EVENT_FORM_EDIT_END) {
                FZ_FORM_EDIT_START
                Dispatcher::unsubscribe(this, Form::EVENT_FORM_EDIT_END, event->sender);
                _syncHandles(index);
                Dispatcher::subscribe(Form::EVENT_FORM_EDIT_END, this, event->sender);
                FZ_FORM_EDIT_END
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

    void BezierList::_syncHandles(unsigned int index, bool master)
    {
        _normalizeCurveShares();

        unsigned int    prevI = (index - 1)%size(),
                        nextI = (index + 1)%size();
        float aIns, bIns;

        if (!master) {
            index = (index + 1)%size();
            prevI = (prevI + 1)%size();
            nextI = (nextI + 1)%size();
        }

        aIns = at(index)->a().ins*_curveShares[index];
        bIns = 1.f - aIns/_curveShares[prevI];
        at(prevI)->setB(bIns, -at(index)->a().val);

        if (!master) {
            index = (index - 2)%size();
            prevI = (prevI - 2)%size();
            nextI = (nextI - 2)%size();
        }

        if (index != nextI) {
            bIns = (1.f - at(index)->b().ins)*_curveShares[index];
            aIns = bIns/_curveShares[nextI];
            at(nextI)->setA(aIns, -at(index)->b().val);
        }
    }

    int BezierList::_childIndex(const void *object)
    {
        for (unsigned int i = 0; i < size(); ++i) {
            if (at(i) == object) {
                return i;
            }
        }
        return -1;
    }

    void BezierList::_apply(
        const FZ::SampleInstant inInstants[],
        FZ::SampleValue         outValues[],
        unsigned int            size)
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
