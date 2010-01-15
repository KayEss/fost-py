/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/jsondb>


/*
    fostlib::jsondb
*/


fostlib::nullable< fostlib::string > filename( const fostlib::jsondb &db );
std::auto_ptr< fostlib::jsondb::local > get_local( fostlib::jsondb &db );


/*
    fostlib::jsondb::local
*/


fostlib::json get_json( const fostlib::jsondb::local &db, const fostlib::jcursor &jc );
void set_json( fostlib::jsondb::local &db, const fostlib::jcursor &j, const fostlib::json &v );

fostlib::json get_with_default( const fostlib::jsondb::local &db, const fostlib::jcursor &j, const fostlib::json &d );

namespace fostlib {
    bool operator ==( const fostlib::jsondb::local &db, const fostlib::json &j );
}
