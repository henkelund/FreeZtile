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

#ifndef FREEZTILE_DISPATCHER_H
#define FREEZTILE_DISPATCHER_H

#include <map>
#include <vector>

namespace FreeZtile {

    struct Event
    {
        Event(const char* id, void *sender, void *data) :
            id(id), sender(sender), data(data)
        {}
        const char *id;
        void *sender, *data;
    };

    class Subscriber
    {
    public:
        /**
         *
         * @param FreeZtile::Event*
         */
        virtual void recieve(const FreeZtile::Event *event) = 0;
    };

    class Dispatcher
    {
    public:

        /**
         *
         * @param const char*
         * @param FreeZtile::Subscriber*
         */
        static void subscribe(const char *id, FreeZtile::Subscriber *subscriber);

        /**
         *
         * @param FreeZtile::Subscriber*
         * @param const char*
         */
        static void unsubscribe(FreeZtile::Subscriber *subscriber, const char *id = NULL);

        /**
         *
         * @param const char*
         * @param void*
         * @param void*
         */
        static void dispatch(const char *id, void *sender, void *data = NULL);

    private:

        /**
         *
         * @var std::map<const char*, std::vector<Subscriber*> >
         */
        static std::map<const char*, std::vector<Subscriber*> > _subscribers;
    };

}

#endif // FREEZTILE_DISPATCHER_H
