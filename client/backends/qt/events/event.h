/******************************************************************************
 * Copyright (C) 2015 Felix Rohrbach <kde@fxrh.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef QMATRIXCLIENT_EVENT_H
#define QMATRIXCLIENT_EVENT_H

#include <algorithm>

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>

namespace QMatrixClient
{
    enum class EventType
    {
        RoomMessage, RoomName, RoomAliases, RoomCanonicalAlias,
        RoomMember, RoomTopic, Typing, Unknown
    };
    
    class Event
    {
        public:
            Event(EventType type);
            virtual ~Event();
            
            EventType type() const;
            QString id() const;
            QDateTime timestamp() const;
            QString roomId() const;
            // only for debug purposes!
            QString originalJson() const;

            /**
             * @brief Finds a place in the time-ordered list of events where a new
             * event could be inserted
             * @return an iterator to an event inside 'events' with the earliest
             * timestamp after the one of 'ev'; events.end(), if all events are earlier
             * than 'ev'
             */
            template <template <typename> class EvContT>
            typename EvContT<Event *>::iterator
            findEarliestAfterMe(EvContT<Event *> & events) const
            {
                auto comp = [](const Event * a, const Event * b) {
                    return a->timestamp() < b->timestamp();
                };
                return std::lower_bound (events.begin(), events.end(), this, comp);
            }
        protected:
            bool parseJson(const QJsonObject& obj);
        
        private:
            class Private;
            Private* d;
    };

    Event* makeEventFrom(const QJsonObject& obj);
    void appendEventsFromJson(const QJsonArray& array, QList<Event *> *evlist);
}

#endif // QMATRIXCLIENT_EVENT_H
