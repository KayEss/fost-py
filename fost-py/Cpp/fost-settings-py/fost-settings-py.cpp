/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include "settings.hpp"


BOOST_PYTHON_MODULE( _settings ) {
    using namespace boost::python;
    fostlib::python_string_registration();
    fostlib::python_json_registration();

    class_<settings>("settings", init<>() )
        .def( "file", &settings::file )
        .def( "get", &settings::get )
        .def( "set", &settings::set )
        .def( "has_key", &settings::has_key )
    ;
}
