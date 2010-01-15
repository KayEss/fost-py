/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include <fost/db>


using namespace fostlib;


BOOST_PYTHON_MODULE( _schema ) {
    using namespace boost::python;
    python_string_registration();
    python_json_registration();

    class_<
        enclosure,
        boost::shared_ptr< enclosure >, boost::noncopyable
    >(
        "enclosure", init< string >()
    )
        .def(init<const enclosure &, string >())
        .def("fq_name", &meta_instance::fq_name)
    ;

    class_<
        meta_instance, bases< enclosure >,
        boost::shared_ptr< meta_instance >, boost::noncopyable
    >(
        "meta_instance", init< string >()
    )
        .def(init<const enclosure &, string >())
    ;
}
