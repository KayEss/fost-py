/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include "url.hpp"


using namespace fostlib;


BOOST_PYTHON_MODULE( _internet ) {
    using namespace boost::python;
    python_string_registration();
    python_json_registration();

    def( "url_filespec_encode", url_filespec_encode );
    def( "url_filespec_assert_valid", url_filespec_assert_valid );

    def( "x_www_form_urlencoded", x_www_form_urlencoded );

    class_< url >(
        "url", init< optional< string > >()
    )
        .def("__unicode__", url_to_fostlib_string)
        .def("__call__", url_join)
    ;

    class_<
        http::user_agent::request, std::auto_ptr< http::user_agent::request >, boost::noncopyable
    >(
        "ua_request", init< string, url, optional< string > >()
    )
        .add_property("method",
            accessors_getter< http::user_agent::request, string, &http::user_agent::request::method >
        )
        .add_property("url",
            accessors_getter< http::user_agent::request, url, &http::user_agent::request::address >
        )
    ;

    class_<
        http::user_agent::response, std::auto_ptr< http::user_agent::response >, boost::noncopyable
    >(
        "ua_response", no_init
    )
        .add_property("method",
            accessors_getter< http::user_agent::response, const string, &http::user_agent::response::method >
        )
        .add_property("url",
            accessors_getter< http::user_agent::response, const url, &http::user_agent::response::address >
        )
        .add_property("protocol",
            accessors_getter< http::user_agent::response, const string, &http::user_agent::response::protocol >
        )
        .add_property("status",
            accessors_getter< http::user_agent::response, const int, &http::user_agent::response::status >
        )
        .add_property("message",
            accessors_getter< http::user_agent::response, const string, &http::user_agent::response::message >
        )
        .add_property("body", ua_response_body)
    ;

    class_<
        http::user_agent, std::auto_ptr< http::user_agent >, boost::noncopyable
    >(
        "user_agent", init< optional< url > >()
    )
        .def("__call__", &http::user_agent::operator ())
        .add_property("base",
            accessors_getter< http::user_agent, url, &http::user_agent::base >
        )
        .def("fost_authenticate", ua_fost_authenticate)
    ;

}
