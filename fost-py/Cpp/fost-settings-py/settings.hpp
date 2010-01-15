/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/python>
#include <fost/settings.hpp>
#include <fost/cli>
#include <fost/inifile.hpp>


struct settings {
    typedef fostlib::ini_file ini_settings_t;
    typedef boost::shared_ptr< ini_settings_t > ini_store_t;
    std::list< ini_store_t > ini_database;
    typedef fostlib::setting< fostlib::json > setting_t;
    typedef boost::shared_ptr< setting_t > store_t;
    std::list< store_t > database;

    settings();

    void file( const fostlib::string & );
    fostlib::json get( const fostlib::string &, const fostlib::string & ) const;
    void set( const fostlib::string &, const fostlib::string &, const fostlib::json & );
    bool has_key( const fostlib::string &, const fostlib::string & ) const;
};

