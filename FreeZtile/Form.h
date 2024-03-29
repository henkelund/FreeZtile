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

#ifndef FREEZTILE_FORM_H
#define FREEZTILE_FORM_H

#include <pthread.h>
#include "FreeZtile/Types.h"
#include "FreeZtile/Dispatcher.h"

#define FZ_FORM_EDIT_START \
    Dispatcher::dispatch(EVENT_FORM_EDIT_START, this); \
    _acquire();

#define FZ_FORM_EDIT_END \
    _invalidateCache(); \
    _release(); \
    Dispatcher::dispatch(EVENT_FORM_EDIT_END, this);

namespace FZ {

    class Form
    {
    public:

        static const unsigned int   STATE_NONE              = 0;
        static const unsigned int   STATE_APPLYING          = (1 << 0);
        static const unsigned int   STATE_CACHE_ACTIVATED   = (1 << 1);
        static const unsigned int   STATE_CACHE_INVALIDATED = (1 << 2);

        static const char           *EVENT_FORM_EDIT_START;
        static const char           *EVENT_FORM_EDIT_END;
        static const char           *EVENT_FORM_CACHE_INVALIDATED;

        /**
         * Constructor
         *
         */
        Form();

        /**
         * Destructor
         *
         */
        virtual ~Form();

        /**
         *
         * @param FZ::SampleInstant[] inInstants
         * @param FZ::SampleValue[] outValues
         * @param unsigned int size
         */
        void            apply(
                            const FZ::SampleInstant inInstants[],
                            FZ::SampleValue         outValues[],
                            unsigned int            size);

        /**
         *
         * @return unsigned int
         */
        unsigned int    state();

        /**
         *
         * @param bool
         */
        void            useCache(bool useCache = true);

    protected:

        /**
         *
         * @param FZ::SampleInstant[] inInstants
         * @param FZ::SampleValue[] outValues
         * @param unsigned int size
         */
        virtual void    _apply(
                            const FZ::SampleInstant inInstants[],
                            FZ::SampleValue         outValues[],
                            unsigned int            size) = 0;

        /**
         *
         * @return int
         */
        int             _acquire();

        /**
         *
         * @return int
         */
        int             _release();

        /**
         *
         */
        void            _invalidateCache();

    private:

        /**
         *
         * @var int
         */
        unsigned int    _state;

        /**
         *
         * @var SampleValue*
         */
        SampleValue     *_cache;

        /**
         *
         * @var unsigned int
         */
        unsigned int    _cacheSize;

        /**
         *
         * @var unsigned int
         */
        unsigned int    _requestedCacheSize;

        /**
         * This mutex lock is acquired by _acquire() and released by _release().
         * It should be used to lock the form when changes are being made that
         * affects it's shape.
         *
         * @var pthread_mutex_t
         */
        pthread_mutex_t _lock;
    };

}

#endif // FREEZTILE_FORM_H
