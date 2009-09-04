/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_WSGI_APPLICATION_HPP
#define FOST_DETAIL_WSGI_APPLICATION_HPP


#include <fost/python>
#include <fost/unicode>
#include <fost/internet>
#include <fost/detail/http.server.hpp>


namespace fostlib {


    namespace python {


        namespace wsgi {


            class FOST_WSGI_DECLSPEC application : boost::noncopyable {
                boost::python::object m_module, m_application;
                public:
                    explicit application( const string & );

                    std::auto_ptr< mime > operator () ( http::server::request & ) const;
            };


        }


    }


}


#endif // FOST_DETAIL_WSGI_APPLICATION_HPP
