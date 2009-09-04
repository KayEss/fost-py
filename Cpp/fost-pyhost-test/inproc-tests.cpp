/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/test>


FSL_TEST_SUITE( inproc );


FSL_TEST_FUNCTION( singleton ) {
    {
        fostlib::python::inproc_host host1;
        FSL_CHECK_EXCEPTION(fostlib::python::inproc_host host2, fostlib::exceptions::not_implemented&);
    }
    fostlib::python::inproc_host host;
}
