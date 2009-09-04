/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-wsgi.hpp"
#include <fost/python>
#include <fost/detail/wsgi.application.hpp>

#include <boost/python/stl_iterator.hpp>


using namespace fostlib;


namespace {
    void start_response(boost::python::object status, boost::python::list headers) {
    }
}


fostlib::python::wsgi::application::application( const string &appname ) {
    std::size_t last_dot = appname.find_last_of(".");
    if ( last_dot == string::npos )
        throw exceptions::not_implemented("The application name must include a . as there must be a module to import it from", appname);
    m_module = boost::python::import(coerce< boost::python::str >(appname.substr(0, last_dot)));
    m_application = boost::python::getattr(m_module, coerce< boost::python::str >(appname.substr(last_dot + 1)));
}


std::auto_ptr< mime > fostlib::python::wsgi::application::operator () ( http::server::request & ) const {
    boost::python::dict environ;
    boost::python::object strings = m_application(environ, start_response);
    string result;
    for ( boost::python::stl_input_iterator<string> i(strings), e; i != e; ++i )
        result += *i;
    return std::auto_ptr< mime >( new text_body( result, mime::mime_headers(), L"text/plain" ) );
}

