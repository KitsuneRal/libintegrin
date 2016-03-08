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

#include <string>
#include <iostream>
#include <iomanip> // for std::put_time
#include <ctime>

#include "client/client-common.h"

enum RETVAL : int {
    OK = 0,
    NetworkFailure = 1,
    TestsFailed = 10
};

class my_put_time {
public:
    my_put_time(const time_t t = time(nullptr)) : _t(t) { }
private:
    time_t _t;

    friend std::ostream & operator<<(std::ostream & os, const my_put_time & tp)
    {
        return os << std::ctime(&tp._t); // Not thread safe
    }
};

/**
 * @brief Entry point for the test suite.
 * @param argv (all optional but should go in exactly this order):
 *      (1) server URL with port after colon (matrix.org by default)
 *      (2) test username base (@matrixx-user by default); up to 10 users in
 *          the form @matrixx-user01:matrix.org, @matrixx-user02:matrix.org, ...
 *          will be created (or reused if the server returns M_USER_IN_USE).
 *          The common password for all will be "-+testUser+-" (without quotes)
 *      (3) test room name without the server part (#matrixx-testroom by default);
 *          it will be created as a private room or reused if it's already made.
 * @return One of RETVAL enum values, according to the outcome.
 */
int main(int argc, char* argv[])
{
    using namespace std;

    string server_url{"matrix.org"};
    string username_base{"@matrixx-user"};

    if (argc > 2)
        cout << "Warning: extraneous arguments will be ignored." << endl;
    if (argc > 0)
        server_url = argv[0]; // FIXME: it assumes sane input atm
    if (argc > 1)
        username_base = argv[1]; // FIXME: same as above

    cout
        << "libmatrixx client testsuite" << endl
        << "Parameters: " << endl
        << "  Server: " << server_url << endl
        << "  Username base: " << username_base << endl;

    cout << "START at " << my_put_time() << endl;
    // User story: exploring the server
    // Login
    // Get the list of rooms
    // Get the list of users of each room
    // Logout

    cout << "FINISHED at " << my_put_time() << endl;
    return RETVAL::OK;
}
