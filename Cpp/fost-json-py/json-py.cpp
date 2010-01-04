/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include "connection.hpp"
#include "jcursor.hpp"
#include "jsonblob.hpp"


using namespace fostlib;


BOOST_PYTHON_MODULE( _json_fost ) {
    using namespace boost::python;
    python_string_registration();
    python_json_registration();

    def("parse", (json (*)( const string & ))json::parse),
    def("unparse", json::unparse),

    class_<
        jcursor
    >(
        "jcursor", init<>()
    )
        .def(self == jcursor())
        .def(self != jcursor())
        .def(self / json())
        .def(self /= json())
        .def("__len__", &jcursor::size)
        .def("__getitem__", get_jcursor_item)
    ;

    class_<
        jsondb::local, std::auto_ptr< jsondb::local >, boost::noncopyable
    >(
        "jsonblob_local", no_init
    )
        .def(self == json())
        .def("__getitem__", get_json)
        .def("__setitem__", set_json)
        .def("get", get_json)
        .def("get", get_with_default)
        .def("has_key", &jsondb::local ::has_key< jcursor >)
        .def("commit", &jsondb::local ::commit)
    ;

    class_<
        jsondb, std::auto_ptr< jsondb >, boost::noncopyable
    >(
        "jsonblob", init< optional< string, json > >()
    )
        .add_property("filename", filename)
        .add_property("local", get_local)
    ;
}
