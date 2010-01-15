/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "jcursor.hpp"


using namespace fostlib;


boost::python::object get_jcursor_item( const fostlib::jcursor &c, fostlib::jcursor::size_type i ) {
    jcursor::value_type v( c[ i ] );
    if ( boost::get< json::array_t::size_type >( &v ) )
        return boost::python::object( boost::get< json::array_t::size_type >( v ) );
    else
        return boost::python::object( boost::get< string >( v ) );
}
