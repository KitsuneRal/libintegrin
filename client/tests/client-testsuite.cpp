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
#include <ctime>

#include "client-common.h"
#include "test-util.h"

#include <QCoreApplication>
#include <QUrl>
#include <QtDebug>
#include "backends/libqmatrixclient/connection.h"

enum RETVAL : int {
    OK = 0,
    NetworkFailure = 1,
    TestsFailed = 10
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
    using namespace scope_guard;

    if (argc > 2)
        cout << "Warning: extraneous arguments will be ignored." << endl;

    // FIXME: assignments below assume sane input atm
    const string server_url = (argc > 1) ? argv[1] : "http://matrix.org";
    const string username_base = (argc > 2) ? argv[2] : "@matrixx-user";
    const string main_user_pwd = "-+testUser+-";

    cout
        << "libmatrixx client testsuite" << endl
        << "Parameters: " << endl
        << "  Server: " << server_url << endl
        << "  Username base: " << username_base << endl;

    const string main_user_name = username_base + ":" + server_url;

    /* Set up */
    testsuite_scope testsuite("ALL TESTS");
    QCoreApplication app(argc, argv);
    int retval = RETVAL::TestsFailed;
    const QString qserver = QString::fromStdString(server_url);
    const QString qusername = QString::fromStdString(main_user_name);
    const QString qpassword = QString::fromStdString(main_user_pwd);
    /* Tests */
    {
        testsuite_run_scope _tss(testsuite);
        using namespace mtrx::transport;
        using namespace QMatrixClient;

        Connection c{QUrl(qserver)};
        app.connect( &c, &QMatrixClient::Connection::connected,
                     [&app]() {
            qInfo("Connection succesful");
            app.quit(); } );
        app.connect( &c, &QMatrixClient::Connection::connectionError,
                     [&app](QString error) {
            qCritical() << "Connection failed:" << error;
            app.exit(RETVAL::NetworkFailure);
        } );
        app.connect( &c, &QMatrixClient::Connection::loginError,
                     [&app](QString error) {
            qCritical() << "Login failed:" << error;
            app.exit(RETVAL::TestsFailed);
        } );
        c.connectToServer(qusername, qpassword);
        //ServerSession c(server_url, main_user_name);
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
