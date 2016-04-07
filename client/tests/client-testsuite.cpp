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

#include <ctime>

#include "client-common.h"
#include "test-util.h"

#include <QCoreApplication>
#include <QUrl>
#include <QtDebug>

#include "backends/backend-config.h"

enum RETVAL : int {
    OK = 0,
    NetworkFailure = 1,
    TestsFailed = 10
};

/**
 * @brief Entry point for the test suite.
 * @param argv (all optional but should go in exactly this order - see defaults
 * in the code):
 *      (1) server URL with port after colon
 *      (2) the username of the main test user; (more users in
 *          the form @main-test-username\d\d:matrix.org may be created/used in
 *          the future, \d is a single digit.
 *          The common password for all will be "-+testUser+-" (without quotes)
 *      (3) test room name without the server part. It will be created as
 *          a private room or reused if it's already made.
 * @return One of RETVAL enum values, according to the outcome.
 */
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace scope_guard;
    using namespace mtrx::data;

    /* Set up */
    testsuite_scope testsuite("ALL TESTS");
    QCoreApplication app(argc, argv);

    if (argc > 2)
        cout << "Warning: extraneous arguments will be ignored." << endl;

    // FIXME: assignments below assume sane input atm
    const string_t server_url = (argc > 1) ? argv[1] : "http://matrix.org";
    const string_t username_base = (argc > 2) ? argv[2] : "@test-matrixx-user";
    const string_t main_user_name = username_base + ":" + server_url;
    const string_t main_user_pwd = "-+testUser+-";

    debug_out()
        << "libmatrixx client testsuite" << endl
        << "Parameters: " << endl
        << "  Server: " << server_url << endl
        << "  Main username: " << main_user_name << endl;

    int retval = RETVAL::TestsFailed;
    /* Tests */
    {
        testsuite_run_scope _tss(testsuite);
        using namespace mtrx::transport;
        using namespace QMatrixClient;

        Connection c{QUrl(server_url)};
        app.connect( &c, &Connection::connected,
            [&app]() {
                qInfo("Connection succesful");
                app.quit();
            }
        );
        app.connect( &c, &Connection::connectionError,
            [&app](QString error) {
                qCritical() << "Connection failed:" << error;
                app.exit(RETVAL::NetworkFailure);
            }
        );
        app.connect( &c, &Connection::loginError,
            [&app](QString error) {
                qCritical() << "Login failed:" << error;
                app.exit(RETVAL::TestsFailed);
            }
        );
        c.connectToServer(main_user_name, main_user_pwd);
        //Connection c(server_url);
        //c.login(main_user_name, main_user_pwd);
        //if (!c)
        //    return RETVAL::NetworkFailure;
        // User story: exploring the server
        // Login
        // Get the list of rooms
        // Get the list of users of each room
        // Logout
        retval = app.exec();
        _tss.set_good_exit(retval == RETVAL::OK);
    }
    /* Tear down */
    testsuite.set_good_exit();
    return retval;
}
