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

namespace FZ {

    struct Subscription
    {
        const void *publisher;
    };

    struct Event
    {
        Event(
            const char              *id,
            const void              *sender,
            const void              *data,
            const FZ::Subscription  *subscription) :
            id(id),
            sender(sender),
            data(data),
            subscription(subscription)
        {}
        const char              *id;
        const void              *sender;
        const void              *data;
        const FZ::Subscription  *subscription;
    };

    class Subscriber
    {
    public:

        /**
         *
         * @param FZ::Event*
         */
        virtual void recieve(const FZ::Event *event) = 0;
    };

    class Dispatcher
    {
    public:

        /**
         *
         * @param const char*
         * @param FZ::Subscriber*
         * @param const void*
         */
        static void subscribe(
                        const char      *id,
                        FZ::Subscriber  *subscriber,
                        const void      *publisher = NULL);

        /**
         *
         * @param FZ::Subscriber*
         * @param const char*
         * @param const void*
         */
        static void unsubscribe(
                        FZ::Subscriber  *subscriber,
                        const char      *id = NULL,
                        const void      *publisher = NULL);

        /**
         *
         * @param const char*
         * @param void*
         * @param void*
         */
        static void dispatch(
                        const char *id,
                        const void *publisher,
                        const void *data = NULL);

    private:

        /**
         *
         * @var std::map<const char*, std::vector<Subscriber*> >
         */
        static std::map<const char*, std::vector<Subscriber*> > _subscribers;
    };

}

#endif // FREEZTILE_DISPATCHER_H
