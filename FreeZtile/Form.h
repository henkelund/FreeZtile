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

namespace FreeZtile {

    class Form
    {
    public:

        static const int STATE_NONE                 = 0;
        static const int STATE_APPLYING             = (1 << 0);
        static const int STATE_CACHED               = (1 << 1);
        static const int STATE_CACHE_INVALIDATED    = (1 << 2);

        Form();

        virtual ~Form();

        void apply(
                const FreeZtile::SampleInstant inInstants[],
                FreeZtile::SampleValue outValues[],
                unsigned int size);

        /**
         *
         * @return int
         */
        int state();

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
                unsigned int size) = 0;

        /**
         *
         * @return int
         */
        int _acquire();

        /**
         *
         * @return int
         */
        int _release();

    private:

        /**
         *
         * @var int
         */
        int             _state;

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
