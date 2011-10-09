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
#include <iostream>

namespace FreeZtile {

    Form::Form() :
        _state(STATE_CACHE_ACTIVATED | STATE_CACHE_INVALIDATED),
        _cache(NULL),
        _cacheSize(0),
        _requestedCacheSize(44100)
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

        bool useCache = _state & STATE_CACHE_ACTIVATED;
        bool buildCache = useCache && (_state & STATE_CACHE_INVALIDATED);

        if (useCache && _requestedCacheSize > _cacheSize) {
            _invalidateCache();
        }

        if (buildCache) {
            if (_cache == NULL || _requestedCacheSize > _cacheSize) {
                // free old garbage
                if (_cache != NULL) {
                    free(_cache);
                    _cache = NULL;
                }
                // allocate new space
                _cache = (SampleValue*) malloc(
                            sizeof(SampleValue)*(_cacheSize = _requestedCacheSize));
            }

            SampleValue cacheInstants[_cacheSize];

            // render form
            for (unsigned int i = 0; i < _cacheSize; ++i) {
                cacheInstants[i] = ((float)i)/_cacheSize;
            }
            _apply(cacheInstants, _cache, _cacheSize);

            // update state
            _state &= ~STATE_CACHE_INVALIDATED;
        }

        // fill output
        if (useCache) {

            for (unsigned int i = 0; i < size; ++i) {
                outValues[i] =
                    _cache[((int)(inInstants[i]*_cacheSize))%_cacheSize];
            }
        } else {
            _apply(inInstants, outValues, size);
        }

        _state &= ~STATE_APPLYING;
        _release();
    }

    unsigned int Form::state()
    {
        return _state;
    }

    void Form::useCache(bool useCache)
    {
        if (useCache) {
            _state |= STATE_CACHE_ACTIVATED;
        } else {
            _state &= ~STATE_CACHE_ACTIVATED;
        }
    }

    int Form::_acquire()
    {
        return pthread_mutex_lock(&_lock);
    }

    int Form::_release()
    {
        return pthread_mutex_unlock(&_lock);
    }

    void Form::_invalidateCache()
    {
        _state |= STATE_CACHE_INVALIDATED;
    }
}
