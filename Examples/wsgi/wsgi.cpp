/*
    Copyright 2008-2009 Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/cli>
#include <fost/main>
#include <fost/internet>
#include <fost/detail/http.server.hpp>


using namespace fostlib;


namespace {
    setting< string > c_host( L"fwsgi", L"Server", L"Bind to", L"localhost", true );
    setting< int > c_port( L"fwsgi", L"Server", L"Port", 8001, true );

    setting< string > c_application( L"fwsgi", L"WSGI", L"Application", "wsgiref.simple_server.demo_app", true );

    void start_response(boost::python::object status, boost::python::list headers) {
    }
}


FSL_MAIN(
    L"fwsgi",
    L"Simple HTTP WSGI server\nCopyright (c) 2008-2009, Felspar Co. Ltd."
)( fostlib::ostream &o, fostlib::arguments &args ) {
    // Work out the IP and port bindings
    http::server server( host( args[1].value(c_host.value()) ), c_port.value() );
    o << L"Answering requests on http://" << server.binding() << L":" << server.port() << L"/" << std::endl;
    // Create the Python host
    fostlib::python::inproc_host host;

    // Find the Python function that corresponds to the application
    string appname = c_application.value();
    std::size_t last_dot = appname.find_last_of(".");
    if ( last_dot == string::npos )
        throw exceptions::not_implemented("The application name must include a . as there must be a module to import it from", appname);
    boost::python::object module = boost::python::import(coerce< boost::python::str >(appname.substr(0, last_dot)));
    boost::python::object app = boost::python::getattr(module, coerce< boost::python::str >(appname.substr(last_dot + 1)));

    // Keep serving forever
    for ( bool process( true ); process; ) {
        std::auto_ptr< http::server::request > req( server() );
        o << req->method() << L" " << req->file_spec() << std::endl;
        boost::python::dict environ;
        app(environ, start_response);
    }
    return 0;
}
