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

#include "Dispatcher.h"

namespace FreeZtile {

    std::map<const char*, std::vector<Subscriber*> > Dispatcher::_subscribers;

    void Dispatcher::subscribe(const char *id, FreeZtile::Subscriber *subscriber)
    {
        std::vector<Subscriber*>::iterator it;
        for (it = _subscribers[id].begin(); it != _subscribers[id].end(); ++it) {
            if ((*it) == subscriber) {
                // listener already added
                return;
            }
        }
        _subscribers[id].push_back(subscriber);
    }

    void Dispatcher::dispatch(const char *id, void *sender, void *data)
    {
        Event event(id, sender, data);
        std::vector<Subscriber*>::iterator it;
        for (it = _subscribers[id].begin(); it != _subscribers[id].end(); ++it) {
            (*it)->recieve(&event);
        }
    }

}