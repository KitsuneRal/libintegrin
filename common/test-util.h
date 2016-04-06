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

#include <ctime>
#include <cassert>
#include <iostream>
#include <string>

class my_put_time
{
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
    scope_base() : _good_exit(false) { }
    bool good_exit() { return _good_exit; }
    void set_good_exit(bool new_value = true) { _good_exit = new_value; }
    ~scope_base() { }

private:
    bool _good_exit;
};

class named
{
public:
    explicit named(const string & scope_name) : _name(scope_name) { }
    const string & name() const { return _name; }
private:
    const string _name;
};

struct timed
{
    explicit timed(ostream & os/* = cout*/) : _os(os) {
        _os << "Enter time: " << my_put_time();
    }
    ~timed() {
        _os << "Exit time: " << my_put_time();
    }

protected:
    ostream &_os;
};

struct testsuite_scope : public scope_base, public named
{
    explicit testsuite_scope(const string & n, ostream & os = cout)
        : named(n), _os(os)
    {
        _os << name() << ": Setup started" << endl;
    }
    ~testsuite_scope() {
        _os << name() << ": Tear-down "
            << (good_exit() ? "complete" : "FAILED") << endl;
    }

protected:
    ostream &_os;

    friend class testsuite_run_scope;
};

class testsuite_run_scope : public scope_base, public timed
{
public:
    explicit testsuite_run_scope(testsuite_scope & tbs)
        : timed(tbs._os), _trs(tbs)
    {
        _os << _trs.name() << ": Tests START" << endl;
    }
    ~testsuite_run_scope()
    {
        _os << _trs.name() << ": ";
        _os
            << (good_exit()
                ? "Tests FINISHED"
                : "One or more tests FAILED")
            << endl;
    }

private:
    testsuite_scope &_trs;
};

}
