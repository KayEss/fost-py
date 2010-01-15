/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include <fost/pyhost>
#include <fost/main>


FSL_MAIN_INTERPRETER(
    L"fpython",
    L"fpython\nCopyright (c) 1995-2009 Felspar Co. Ltd."
)( const fostlib::ini_settings &settings, fostlib::ostream &out, fostlib::arguments &args ) {
    fostlib::python::inproc_host host;

    /*
        We want to load the INI file for the script before the INI file on the command line
        This means we have to load the script one and then load the ini one so its
        settings get layered on top.
        This has the side effect of allowing the script's INI file to override the interpreter's INI
        file if no -i switch is provided.
    */
    fostlib::ini_file script( args[ 1 ].value().substr( 0, args[ 1 ].value().find_last_of( L'.' ) ) + L".ini" );
    args.commandSwitch( L"i", settings.name, L"IniFile" );
    fostlib::ini_file ifile( settings.c_iniFile.value() );
    fostlib::standard_arguments( settings, out, args );

    /*
        Now we should set up the environment for the script and execute it
    */
    try {
        // Build the argument list
        boost::python::list main_args;
        for ( std::size_t p( 2 ); p < args.size(); ++p )
            main_args.append( fostlib::coerce< boost::python::str >( args[ p ] ) );

        // Build the command line switches
        boost::python::dict switches;
        for ( std::map< fostlib::string, fostlib::string >::const_iterator p( args.switches().begin() ); p != args.switches().end(); ++p )
            switches[ fostlib::coerce< boost::python::str >( p->first ) ] = fostlib::coerce< boost::python::str >( p->second );

        // Run the file
        host( fostlib::coerce< boost::filesystem::wpath >( args[ 1 ].value() ), main_args, switches );
    } catch ( boost::python::error_already_set const & ) {
        PyErr_Print();
    }

    return 0;
}
