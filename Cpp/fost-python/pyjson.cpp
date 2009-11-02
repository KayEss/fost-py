/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-python.hpp"
#include <fost/detail/pybind.hpp>


using namespace fostlib;
namespace bp = boost::python;


namespace {
    struct from_python {
        static void *convertible( PyObject *object );
        static void construct( PyObject *object, bp::converter::rvalue_from_python_stage1_data *data );
    private:
        static json to_json( bp::object o );
    };
    struct to_python {
        static PyObject *convert( const json &j );
        static PyTypeObject const* get_pytype();
    private:
        static bp::object from_json( const json &j );
    };
}


void fostlib::python_json_registration() {
    static bool executed = false;
    if ( !executed ) {
        executed = true;
        bp::converter::registry::push_back(
            from_python::convertible, from_python::construct,
            bp::type_id< json >()
        );
        bp::to_python_converter< json, to_python, false >();
    }
}


/*
    From Python to fostlib::json
*/

void *from_python::convertible( PyObject *object ) {
    return object;
}
void from_python::construct( PyObject *object, bp::converter::rvalue_from_python_stage1_data *data ) {
    void *storage = reinterpret_cast<
        boost::python::converter::rvalue_from_python_storage< json >*
    >( data )->storage.bytes;
    new (storage) json( to_json( bp::object( bp::handle<>( bp::borrowed( object ) ) ) ) );
    data->convertible = storage;
}
json from_python::to_json( bp::object o ) {
    try {
        if ( o.ptr() == Py_None )
            return json();
        else if ( o.ptr() == Py_False )
            return json( false );
        else if ( o.ptr() == Py_True )
            return json( true );
        else if ( bp::extract< int64_t >( o ).check() )
            return json( bp::extract< int64_t >( o )() );
        else if ( bp::extract< double >( o ).check() )
            return json( bp::extract< double >( o )() );
        else if (
            bp::extract< bp::list >( o ).check()
            || bp::extract< bp::tuple >( o ).check()
            || PyAnySet_Check( o.ptr() )
        ) {
            if ( PyAnySet_Check( o.ptr() ) )
                o = bp::list(o);
            std::size_t len = bp::len( o );
            json::array_t array;
            array.reserve(len);
            for ( std::size_t p = 0; p != len; ++p )
                array.push_back( boost::shared_ptr< json >( new json( to_json( o[ p ] ) ) ) );
            return array;
        } else if ( bp::extract< bp::dict >( o ).check() ) {
            json::object_t object;
            bp::object keys = o.attr("keys")();
            for ( std::size_t len = bp::len( keys ); len > 0; --len ) {
                bp::object key = keys[ len - 1 ];
                object.insert( std::make_pair(
                    bp::extract< string >( key )(),
                    boost::shared_ptr< json >( new json( bp::extract< json >( o[ key ] )() ) )
                ) );
            }
            return object;
        } else if ( bp::extract< string >( o ).check() )
            return json( bp::extract< string >( o )() );
        else
            throw exceptions::not_implemented( L"from_python::to_json( boost::python::object )" );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Whilst converting a Python object to a fostlib::json" << std::endl;
        throw;
    }
}


/*
    From fostlib::json to Python
*/

PyObject *to_python::convert( const json &j ) {
    return bp::incref( from_json( j ).ptr() );
}
bp::object to_python::from_json( const json &j ) {
    try {
        if ( j.isnull() )
            return bp::object();
        else if ( !j.get< bool >().isnull() )
            return bp::object( j.get< bool >().value() );
        else if ( !j.get< int64_t >().isnull() )
            return bp::object( j.get< int64_t >().value() );
        else if ( !j.get< double >().isnull() )
            return bp::object( j.get< double >().value() );
        else if ( !j.get< string >().isnull() )
            return bp::object( j.get< string >().value() );
        else if ( j.isarray() ) {
            bp::list list;
            for ( json::const_iterator it( j.begin() ); it != j.end(); ++it )
                list.attr( "append" )( from_json( *it ) );
            return list;
        } else if ( j.isobject() ) {
            bp::dict object;
            json::object_t inside( j.get< json::object_t >().value() );
            for ( json::object_t::const_iterator it( inside.begin() ); it != inside.end(); ++it )
                object[ it->first ] = from_json( *it->second );
            return object;
        } else
            throw exceptions::not_implemented( L"to_python::from_json( const json &j )" );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info()
            << L"Whilst converting a fostlib::json to its equivalent Python type\n"
            << json::unparse( j, true );
        throw;
    }
}
PyTypeObject const* to_python::get_pytype() {
    throw exceptions::not_implemented( L"to_python::get_pytype()" );
}
