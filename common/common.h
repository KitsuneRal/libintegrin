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

#include <string>

namespace mtrx {

/**
 * @brief Standard error strings listed in the spec.
 */
namespace Error {
    /** Forbidden access, e.g. joining a room without permission, failed login. */
    static const std::string M_FORBIDDEN{"M_FORBIDDEN"};
    /** The access token specified was not recognised. */
    static const std::string M_UNKNOWN_TOKEN{"M_UNKNOWN_TOKEN"};
    /** Request contained valid JSON; but it was malformed in some way, e.g.
     * missing required keys; invalid values for keys. */
    static const std::string M_BAD_JSON{"M_BAD_JSON"};
    /** Request did not contain valid JSON. */
    static const std::string M_NOT_JSON{"M_NOT_JSON"};
    /** No resource was found for this request. */
    static const std::string M_NOT_FOUND{"M_NOT_FOUND"};
    /** Too many requests have been sent in a short period of time.
     * Wait a while then try again. */
    static const std::string M_LIMIT_EXCEEDED{"M_LIMIT_EXCEEDED"};
    /** Encountered when trying to register a user ID which has been taken. */
    static const std::string M_USER_IN_USE{"M_USER_IN_USE"};
    /** Encountered when trying to create a room which has been taken. */
    static const std::string M_ROOM_IN_USE{"M_ROOM_IN_USE"};
    /** Encountered when specifying bad pagination query parameters. */
    static const std::string M_BAD_PAGINATION{"M_BAD_PAGINATION"};
}

/**
 * @brief Abstract data types
 *
 *
 */
namespace types {

}

/**
 * @brief Serialized format encapsulation
 *
 * So far, it's only JSON. More (COAP?) are said to come some time in the future.
 */
namespace format {

}

/**
 * @brief Transport protocol encapsulation
 *
 * So far, it's HTTP/1.1 - but WebSockets etc. are likely to arrive one day.
 */
namespace transport {

}

}
