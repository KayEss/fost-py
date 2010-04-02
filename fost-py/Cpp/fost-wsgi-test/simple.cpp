/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/pyhost>
#include <fost/wsgi>
#include <fost/test>


using namespace fostlib;


FSL_TEST_SUITE( wsgi );


namespace {
    void iterable_with_one_block( const string &appname ) {
        try {
            python::inproc_host host;

            python::wsgi::application app(appname);

            http::server::request req(
                "GET", url::filepath_string("/"),
                std::auto_ptr< binary_body >( new binary_body )
            );
            std::auto_ptr< mime > response = app(req);
            FSL_CHECK_EQ(response->headers()["Content-Type"].value(), L"text/plain");
            FSL_CHECK(response->headers()["Content-Type"].subvalue(L"charset").isnull());
            mime::const_iterator iterator = response->begin();
            const_memory_block block = *iterator;
            FSL_CHECK(block.first != NULL && block.second != NULL);
            FSL_CHECK(++iterator == response->end());
        } catch ( exceptions::exception &e ) {
            e.info() << L"WSGI application: " << appname << std::endl;
            throw;
        }
    }
}
FSL_TEST_FUNCTION( iterable_with_one_block ) {
    iterable_with_one_block("wsgiref.simple_server.demo_app");
}
