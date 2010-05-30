/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include <fost/mailbox>


void iterate_pop3_mailbox_string_host(
    const fostlib::string &host,
    boost::python::object lambda,
    const fostlib::string &name, const fostlib::string &password );
void iterate_pop3_mailbox(
    const fostlib::host &host,
    boost::python::object lambda,
    const fostlib::string &name, const fostlib::string &password );
