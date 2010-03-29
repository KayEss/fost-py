/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/internet>


std::string url_filespec_encode( const fostlib::string &s );
void url_filespec_assert_valid( const fostlib::string &s );

std::string x_www_form_urlencoded( const fostlib::json &j );

fostlib::string url_to_fostlib_string(const fostlib::url &u);
fostlib::url url_join(const fostlib::url &u, const fostlib::string &r);

void ua_fost_authenticate(
    fostlib::http::user_agent &ua, const fostlib::string &key, const fostlib::string &secret
);
std::string ua_response_body(fostlib::http::user_agent::response &);
