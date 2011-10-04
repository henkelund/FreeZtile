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

#include "Form.h"

namespace FreeZtile {

    Form::Form()
    {
        pthread_mutex_init(&_lock, NULL);
    }

    Form::~Form()
    {
        pthread_mutex_destroy(&_lock);
    }

    void Form::apply(
            const FreeZtile::SampleInstant inInstants[],
            FreeZtile::SampleValue outValues[],
            unsigned int size)
    {
        _acquire();
        _apply(inInstants, outValues, size);
        _release();
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
