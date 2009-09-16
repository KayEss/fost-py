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
    PyEval_InitThreads();
    // The previous call has acquired the GIL, but we don't need it here as we have our own
    // when this host object is created so we can release it straight away.
    PyEval_ReleaseLock();
    try {
        fostlib::python_string_registration();
        fostlib::python_json_registration();

        // We need these two to provide context for the scripts
        main_module = boost::python::import( "__main__" );
        main_namespace = boost::python::dict(main_module.attr( "__dict__" ));
    } catch ( ... ) {
        PyErr_Clear();
        Py_Finalize();
        throw;
    }
}
host::~host() {
    try {
        PyErr_Clear();
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


boost::python::object fostlib::python::inproc_host::p_eval_impl( const string &code ) {
    try {
        return boost::python::eval(boost::python::str(code), g_host->main_namespace, g_host->main_namespace);
    } catch ( boost::python::error_already_set& ) {
        PyErr_Print();
        throw exceptions::not_implemented("Boost.Python error handling for eval");
    }
}

void fostlib::python::inproc_host::operator () ( const string &code ) {
    boost::python::exec( boost::python::str(code), g_host->main_namespace, g_host->main_namespace );
}

void fostlib::python::inproc_host::operator () (
    const boost::filesystem::wpath &f,
    boost::python::list args, boost::python::dict kwargs
) {
    try {
        boost::python::exec_file(
            fostlib::coerce< fostlib::utf8string >( fostlib::coerce< fostlib::string >( f.string() ) ).c_str(),
            g_host->main_namespace, g_host->main_namespace
        );

        // Find main and call it through a lambda to handle the arguments for us
        if ( !g_host->main_namespace.has_key( "main" ) )
            throw fostlib::exceptions::null( L"No main() in the loaded Python file" );
        boost::python::object main_func = g_host->main_namespace[ "main" ];
        boost::python::eval(
            "lambda f, a, k: f(*a, **k)", g_host->main_namespace, g_host->main_namespace
        )( main_func, args, kwargs );
    } catch ( boost::python::error_already_set& ) {
        PyErr_Print();
        throw exceptions::not_implemented("Boost.Python error handling when executing a file");
    }
}


/*
    fostlib::python::inproc_host::gil
*/


fostlib::python::inproc_host::gil::gil()
: gstate( PyGILState_Ensure() ) {
}
fostlib::python::inproc_host::gil::~gil() {
    PyGILState_Release(gstate);
}
