/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "url.hpp"
#include <fost/http>


using namespace fostlib;


std::string url_filespec_encode( const fostlib::string &s ) {
    return coerce< url::filepath_string >( s ).underlying().underlying();
}
void url_filespec_assert_valid( const fostlib::string &s ) {
    url::filepath_string_tag::check_encoded( coerce< ascii_printable_string >( s ) );
}


std::string x_www_form_urlencoded( const json &j ) {
    url::query_string qs;
    for ( json::const_iterator i = j.begin(); i != j.end(); ++i )
        qs.append( coerce< string >( i.key() ), coerce< string >( *i ) );
    return qs.as_string().value( ascii_printable_string() ).underlying();
}


string url_to_fostlib_string(const url &u) {
    return coerce< string >( u );
}
url url_join(const url &u, const string &r) {
    std::pair< string, nullable< string > > parts = partition(r, "?");
    url ret(u, coerce< url::filepath_string >(parts.first));
    if ( !parts.second.isnull() )
        ret.query() = url::query_string(coerce<ascii_printable_string>(
            parts.second.value()
        ));
    return ret;
}

