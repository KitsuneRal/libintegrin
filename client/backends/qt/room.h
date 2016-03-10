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

#ifndef QMATRIXCLIENT_ROOM_H
#define QMATRIXCLIENT_ROOM_H

#include <QLinkedList>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QJsonObject>

#include "jobs/syncjob.h"
#include "joinstate.h"

namespace QMatrixClient
{
    class Event;
    class State;
    class Connection;
    class User;

    class Room: public QObject
    {
            Q_OBJECT
        public:
            typedef QLinkedList<Event*> events_list_t;
            typedef QList<User*> users_list_t;

            Room(Connection* connection, QString id);
            virtual ~Room();

            QString id() const;
            events_list_t messages() const;
            QString name() const;
            QStringList aliases() const;
            QString canonicalAlias() const;
            QString displayName() const;
            QString topic() const;
            JoinState joinState() const;
            users_list_t usersTyping() const;

            users_list_t users() const;

            void addMessage( Event* event );
            void addInitialState( State* state );
            void updateData( const SyncRoomData& data );
            void setJoinState( JoinState state );

        public slots:
            void getPreviousContent();

        signals:
            void newMessage(Event* event);
            void namesChanged(Room* room);
            void topicChanged();
            void userAdded(User* user);
            void userRemoved(User* user);
            void joinStateChanged(JoinState oldState, JoinState newState);
            void typingChanged();

        private:
            class Private;
            Private* d;
    };
}

#endif // QMATRIXCLIENT_ROOM_H
