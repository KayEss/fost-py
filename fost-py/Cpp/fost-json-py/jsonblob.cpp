/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "jsonblob.hpp"


using namespace fostlib;


nullable< string > filename( const jsondb &db ) {
    return db.filename();
}
std::auto_ptr< jsondb::local > get_local( jsondb &db ) {
    return std::auto_ptr< jsondb::local >( new jsondb::local( db, jcursor() ) );
}

json get_json( const jsondb::local &db, const jcursor &jc ) {
    return db[ jc ];
}
void set_json( jsondb::local &db, const jcursor &j, const json &v ) {
    if ( db.has_key( j ) )
        db.update( j, v );
    else
        db.insert( j, v );
}

json get_with_default( const jsondb::local &db, const jcursor &j, const json &d ) {
    if ( db.has_key( j ) )
        return db[ j ];
    else
        return d;
}

bool fostlib::operator ==( const jsondb::local &db, const json &j ) {
    return get_json( db, jcursor() ) == j;
}
