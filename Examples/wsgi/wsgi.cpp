/*
    Copyright 2008-2009 Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/wsgi>
#include <fost/cli>
#include <fost/main>
#include <fost/internet>
#include <fost/detail/http.server.hpp>


using namespace fostlib;


namespace {
    setting< string > c_host( L"fwsgi", L"Server", L"Bind to", L"localhost", true );
    setting< int > c_port( L"fwsgi", L"Server", L"Port", 8001, true );

    setting< string > c_application( L"fwsgi", L"WSGI", L"Application", "wsgiref.simple_server.demo_app", true );
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
    python::wsgi::application app( c_application.value() );

    // Set up an environment which contains the global non-changing aspects
    boost::python::dict environment;
    environment["wsgi.version"] = boost::python::make_tuple(1, 0);
    environment["wsgi.url_scheme"] = boost::python::str("http");
    environment["wsgi.multithread"] = false;
    environment["wsgi.multiprocess"] = false;
    environment["wsgi.run_once"] = false;

    environment["SCRIPT_NAME"] = boost::python::str();
    environment["SERVER_PORT"] = boost::python::str(coerce< string >( c_port.value() ));

    // Keep serving forever
    for ( bool process( true ); process; ) {
        std::auto_ptr< http::server::request > req( server() );
        o << req->method() << L" " << req->file_spec() << std::endl;
        environment["SERVER_NAME"] = boost::python::str(c_host.value());
        (*req)( *app(*req, environment) );
    }
    return 0;
}
