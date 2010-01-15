/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "settings.hpp"


using namespace fostlib;


namespace {


#ifdef WIN32
    const setting< string > s_machine_config( L"fost-settings-py", L"settings-py", L"Machine configuration", L"%WINDIR%\\fost.ini", true );
#else
    const setting< string > s_machine_config( L"fost-settings-py", L"settings-py", L"Machine configuration", L"/etc/fost.conf", true );
#endif
    const setting< bool > s_reload( L"fost-settings-py", L"settings-py", L"Reload machine configuration", true, true );


}


settings::settings() {
    if ( s_reload.value() ) {
        database.push_back( store_t( new setting_t( s_reload.domain(), s_reload.section(), s_reload.name(), true, false ) ) );
        ini_database.push_back( ini_store_t( new ini_settings_t( s_machine_config.value() ) ) );
    }
}


void settings::file( const string &location ) {
    ini_database.push_back( ini_store_t( new ini_settings_t( location ) ) );
}


json settings::get( const string &d, const string &n ) const {
    return setting< json >::value( d, n );
}


void settings::set( const string &d, const string &n, const json &v ) {
    database.push_back( store_t( new setting_t( L"fost.pybind.settings", d, n, v, false ) ) );
}


bool settings::has_key( const string &d, const string &n ) const {
    return setting< json >::exists( d, n );
}

