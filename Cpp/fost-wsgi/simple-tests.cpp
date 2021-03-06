/*
    Copyright 2009-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/insert>
#include <fost/pyhost>
#include <fost/test>
#include <fost/wsgi>


using namespace fostlib;


FSL_TEST_SUITE( wsgi );


namespace {
    void iterable_with_one_block( const string &appname ) {
        try {
            python::inproc_host host;

            python::wsgi::application app(appname);

            http::server::request req(
                "GET", url::filepath_string("/"),
                std::make_unique<binary_body>());
            auto response = app(req);

            FSL_CHECK_EQ(response->headers()["Content-Type"].value(), L"text/plain");
            FSL_CHECK(not response->headers()["Content-Type"].subvalue(L"charset"));

            mime::const_iterator iterator = response->begin();
            const_memory_block block = *iterator;
            FSL_CHECK(block.first != NULL && block.second != NULL);
            FSL_CHECK(++iterator == response->end());

        } catch ( exceptions::exception &e ) {
            insert(e.data(), "wsgi", "application", appname);
            throw;
        }
    }
}
FSL_TEST_FUNCTION( iterable_with_one_block ) {
    iterable_with_one_block("wsgiref.simple_server.demo_app");
}
