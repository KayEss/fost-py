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
            public:
                inproc_host();
                ~inproc_host();

                void operator () ( const boost::filesystem::wpath &file, boost::python::list args, boost::python::dict kwargs );
        };


    }


}


#endif // FOST_DETAIL_INPROC_HPP
