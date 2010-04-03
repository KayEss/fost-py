/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PYBIND_HPP
#define FOST_PYBIND_HPP


#include <boost/python.hpp>
#include <fost/core>
#include <fost/exception/out_of_range.hpp>


namespace fostlib {

    ///Call this inside the BOOST_PYTHON_MODULE function to enable fostlib::string handling
    FOST_PYTHON_DECLSPEC void python_string_registration();

    template<>
    struct coercer< boost::python::str, string > {
        boost::python::str coerce( const string &s ) {
            return fostlib::coerce< utf8_string >( s ).underlying().c_str();
        }
    };

    /// Call this inside the BOOST_PYTHON_MODULE function to enable fostlib::json handling
    FOST_PYTHON_DECLSPEC void python_json_registration();

    /// Allow meta accessors to be mapped to properties or methods
    template< typename O, typename V, accessors< V > O::*a >
    V accessors_getter(const O *o) {
        return (o->*a)();
    }


}


#endif // FOST_PYBIND_HPP
