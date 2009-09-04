/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inproc.hpp"
#include <fost/python>
#include <fost/detail/inproc.hpp>
#include <fost/threading>


namespace {
    boost::mutex &g_mutex() {
        static boost::mutex m;
        return m;
    }
    struct host {
        host();
        ~host();
        boost::python::object main_module;
        boost::python::dict main_namespace;
    };
    boost::scoped_ptr< host > g_host;
}


host::host() {
    Py_Initialize();

    try {
        fostlib::python_string_registration();
        fostlib::python_json_registration();

        // We need these two to provide context for the scripts
        main_module = boost::python::import( "__main__" );
        main_namespace = boost::python::dict(main_module.attr( "__dict__" ));
    } catch ( ... ) {
        Py_Finalize();
        throw;
    }
}
host::~host() {
    try {
        Py_Finalize();
    } catch ( ... ) {
        fostlib::absorbException();
    }
}


fostlib::python::inproc_host::inproc_host() {
    boost::mutex::scoped_lock lock(g_mutex());
    if ( g_host.get() )
        throw exceptions::not_implemented("Error handling for creating more than one inproc_host");
    g_host.reset( new host );
}

fostlib::python::inproc_host::~inproc_host() {
    g_host.reset(NULL);
}


void fostlib::python::inproc_host::operator () (
    const boost::filesystem::wpath &f,
    boost::python::list args, boost::python::dict kwargs
) {
    boost::python::exec_file(
        fostlib::coerce< fostlib::utf8string >( fostlib::coerce< fostlib::string >( f.string() ) ).c_str(),
        g_host->main_namespace, g_host->main_namespace
    );

    // Find main and call it through a lambda to handle the arguments for us
    if ( !g_host->main_namespace.has_key( "main" ) )
        throw fostlib::exceptions::null( L"No main() in the loaded Python file" );
    boost::python::object main_func = g_host->main_namespace[ "main" ];
    boost::python::eval( "lambda f, a, k: f(*a, **k)" )( main_func, args, kwargs );
}