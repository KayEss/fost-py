/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_INPROC_HPP
#define FOST_DETAIL_INPROC_HPP


#include <fost/python>
#include <fost/unicode>


namespace fostlib {


    namespace python {


        class FOST_PYHOST_DECLSPEC inproc_host : boost::noncopyable {
            boost::python::object p_eval_impl(const string &expression);
            public:
                inproc_host();
                ~inproc_host();

                void operator () ( const string &python_code );
                void operator () ( const boost::filesystem::wpath &file, boost::python::list args, boost::python::dict kwargs );

                template< typename T >
                T eval( const string &expression ) {
                    return boost::python::extract< T >( p_eval_impl(expression) )();
                }

                // Use this around code that makes use of the Boost.Python code to avoid Python errors
                class FOST_PYHOST_DECLSPEC gil {
                    PyGILState_STATE gstate;
                    public:
                        gil();
                        ~gil();
                };
        };


    }


}


#endif // FOST_DETAIL_INPROC_HPP
