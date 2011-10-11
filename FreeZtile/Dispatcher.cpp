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

namespace FZ {

    std::map<const char*, std::vector<Subscription> > Dispatcher::_subscriptions;

    void Dispatcher::subscribe(
            const char *id, FZ::Subscriber *subscriber, const void *publisher)
    {
        std::vector<Subscription>::iterator it;
        for (it = _subscriptions[id].begin(); it != _subscriptions[id].end(); ++it) {
            if ((*it).publisher == publisher && (*it).subscriber == subscriber) {
                // subscription already added
                return;
            }
        }
        _subscriptions[id].push_back(Subscription(publisher, subscriber));
    }

    void Dispatcher::unsubscribe(
            FZ::Subscriber *subscriber, const char *id, const void *publisher)
    {
        std::map<const char*, std::vector<Subscription> >::iterator it;
        for (it = _subscriptions.begin(); it != _subscriptions.end(); ++it) {

            if (id == NULL || id == it->first) {
                std::vector<Subscription>::iterator subIt = it->second.begin();
                while (subIt != it->second.end()) {
                    if ((*subIt).subscriber == subscriber &&
                            (publisher == NULL || (*subIt).publisher == publisher)) {
                        subIt = it->second.erase(subIt);
                    } else {
                        ++subIt;
                    }
                }
            }
        }
    }

    void Dispatcher::dispatch(const char *id, const void *publisher, const void *data)
    {
        Event event(id, publisher, data);
        std::vector<Subscription>::iterator it;
        for (it = _subscriptions[id].begin(); it != _subscriptions[id].end(); ++it) {
            if ((*it).publisher == publisher || (*it).publisher == NULL) {
                (*it).subscriber->recieve(&event);
            }
        }
    }

}
