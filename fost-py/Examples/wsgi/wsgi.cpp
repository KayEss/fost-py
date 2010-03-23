/*
    Copyright 2008-2010 Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/wsgi>
#include <fost/main>
#include <fost/internet>
#include <fost/http.server.hpp>
#include <fost/threading>


using namespace fostlib;


namespace {
    setting< string > c_host( L"fwsgi", L"Server", L"Bind to", L"localhost", true );
    setting< int > c_port( L"fwsgi", L"Server", L"Port", 8001, true );

    setting< string > c_application( L"fwsgi", L"WSGI", L"Application", "wsgiref.simple_server.demo_app", true );

    bool service( python::wsgi::application &app, http::server::request &req ) {
        req( *app(req) );
        return true;
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
    // Build the application object that will handle the web service
    python::wsgi::application app( c_application.value() );
    /*
        Creating the inproc_host instance has left us holding the GIL. If we don't drop it before we try
        to service new threads all sorts of horrid things are going to happen (which involve the word "deadlock").
    */
    PyEval_ReleaseLock();
    // Keep serving forever
    server( boost::lambda::bind( service, boost::ref(app), boost::ref(boost::lambda::_1) ) );
    // We'll never get here
    return 0;
}
