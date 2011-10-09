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

#include "Observer.h"
#include <iostream>

namespace FreeZtile {

    std::map<const char*, std::vector<Listener*> > Observer::_listeners;

    void Observer::addListsener(const char *id, const FreeZtile::Listener *listener)
    {
        std::cout << id << " added" << std::endl;
    }

    void Observer::fireEvent(const char *id, void *sender, void *data)
    {
        std::cout << id << " fired" << std::endl;
    }

}
