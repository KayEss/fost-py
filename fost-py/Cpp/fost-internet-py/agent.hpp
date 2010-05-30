/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/internet>


void ua_fost_authenticate(
    fostlib::http::user_agent &ua, const fostlib::string &key, const fostlib::string &secret
);
std::string ua_response_body(fostlib::http::user_agent::response &);
