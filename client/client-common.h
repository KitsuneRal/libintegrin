/*
 * Copyright (C) 2016 Kitsune Ral <kitsune-ral@users.sf.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common.h"

namespace mtrx {

/**
 * @brief Transport protocol encapsulation
 *
 * So far, it's HTTP/1.1 - but WebSockets etc. are likely to arrive one day.
 */
namespace transport {

/**
 * @brief A server connection line and settings.
 */
class Server {

};

/**
 * @brief A session with the server.
 *
 * Encapsulation of access_token.
 */
class ServerSession {

};

}

/**
 * @brief Data storage encapsulation.
 *
 * User information, the list of servers, caches etc. are encapsulated here.
 */
namespace storage {

}

}
