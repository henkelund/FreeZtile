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

#include <stdlib.h>
#include "Form.h"

namespace FreeZtile {

    Form::Form() :
        _state(Form::STATE_NONE),
        _cache(NULL),
        _cacheSize(44100)
    {
        pthread_mutex_init(&_lock, NULL);
    }

    Form::~Form()
    {
        if (_cache != NULL) {
            free(_cache);
            _cache = NULL;
        }
        pthread_mutex_destroy(&_lock);
    }

    void Form::apply(
            const FreeZtile::SampleInstant inInstants[],
            FreeZtile::SampleValue outValues[],
            unsigned int size)
    {
        _acquire();
        _state |= STATE_APPLYING;

        if ((~_state & STATE_CACHED) || (_state & STATE_CACHE_INVALIDATED)) {

            // free old garbage
            if (_cache != NULL) {
                free(_cache);
                _cache = NULL;
            }

            // allocate new space
            unsigned int cacheByteSize = sizeof(SampleValue) * _cacheSize;
            _cache = (SampleValue*) malloc(cacheByteSize);

            // render form
            SampleValue cacheInstants[_cacheSize];
            for (unsigned int i = 0; i < _cacheSize; ++i) {
                cacheInstants[i] = ((float)i)/_cacheSize;
            }
            _apply(cacheInstants, _cache, _cacheSize);

            // update state
            _state |= STATE_CACHED;
            _state &= ~STATE_CACHE_INVALIDATED;
        }

        // fill output
        for (unsigned int i = 0; i < size; ++i) {
            outValues[i] = _cache[((int)(inInstants[i]*_cacheSize))%_cacheSize];
        }

        _state &= ~STATE_APPLYING;
        _release();
    }

    int Form::state()
    {
        return _state;
    }

    int Form::_acquire()
    {
        return pthread_mutex_lock(&_lock);
    }

    int Form::_release()
    {
        return pthread_mutex_unlock(&_lock);
    }
}
