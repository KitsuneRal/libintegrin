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

    friend std::ostream & operator<<(std::ostream & os,
                                     const my_put_time & tp) {
        return os << std::ctime(&tp._t); // Not thread safe
    }
};

namespace scope_guard
{
using namespace std;

class scope_base {
public:
    scope_base(ostream & os = cout)
        : _os(os)
        , _good_exit(false)
    { }
    bool good_exit() { return _good_exit; }
    void set_good_exit(bool new_value = true) { _good_exit = new_value; }
    ~scope_base() {
    }

protected:
    ostream & _os;

private:
    bool _good_exit;
};

class named : virtual public scope_base {
public:
    named(const string & scope_name) : _name(scope_name) { }
    const string & name() const { return _name; }
private:
    const string _name;
};

struct timed : virtual public scope_base {
    timed() {
        _os << "Enter time: " << my_put_time();
    }
    ~timed() {
        _os << "Exit time: " << my_put_time();
    }
};

struct testsuite_scope : public named {
    testsuite_scope(const string & n) : named(n) {
        _os << name() << ": Setup started" << endl;
    }
    ~testsuite_scope() {
        _os << name() << ": Tear-down "
            << (good_exit() ? "complete" : "FAILED") << endl;
    }
};

class testrun_scope : public timed {
public:
    testrun_scope(testsuite_scope & tbs) : _trs(tbs) {
        _os << _trs.name() << ": Tests START" << endl;
    }
    ~testrun_scope() {
        _os << _trs.name() << ": Tests FINISHED, tear-down follows" << endl;
    }
private:
    testsuite_scope &_trs;
};

}

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
    using namespace scope_guard;

    if (argc > 2)
        cout << "Warning: extraneous arguments will be ignored." << endl;

    // FIXME: assignments below assume sane input atm
    const string server_url = (argc > 0) ? argv[0] : "matrix.org";
    const string username_base = (argc > 1) ? argv[1] : "@matrixx-user";

    cout
        << "libmatrixx client testsuite" << endl
        << "Parameters: " << endl
        << "  Server: " << server_url << endl
        << "  Username base: " << username_base << endl;

    const string main_user_name = username_base + ":" + server_url;
    {
        /* Set up */
        testsuite_scope testsuite("ALL TESTS");
        /* Tests */
        {
            testrun_scope _tss(testsuite);
            using namespace mtrx::transport;
            //ServerSession c(server_url, main_user_name);
            //if (!c)
            //    return RETVAL::NetworkFailure;
            // User story: exploring the server
            // Login
            // Get the list of rooms
            // Get the list of users of each room
            // Logout
        }
        /* Tear down */
        testsuite.good_exit();
    }

    return RETVAL::OK;
}
