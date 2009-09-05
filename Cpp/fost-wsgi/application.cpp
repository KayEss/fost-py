/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-wsgi.hpp"
#include <fost/python>
#include <fost/threading>
#include <fost/detail/wsgi.application.hpp>

#include <boost/python/stl_iterator.hpp>


using namespace fostlib;


namespace {
    struct wsgi_mime : public mime {
        wsgi_mime( const mime_headers &h )
        : mime( h, h.exists("Content-Type") ? h["Content-Type"].value() : "text/plain" ) {
        }
        boost::python::object output_iterator;
        struct const_iterator : public iterator_implementation {
            const_memory_block operator () () {
                throw exceptions::not_implemented("wsgi_mime::const_iterator::operator () ()");
            }
        };
        std::auto_ptr<fostlib::mime::iterator_implementation> iterator() const {
            throw exceptions::not_implemented("wsgi_mime::iterator() const");
        }
        bool boundary_is_ok( const string &boundary ) const {
            throw exceptions::not_implemented("wsgi_mime::boundary_is_ok( const string &boundary ) const");
        }
        std::ostream &print_on( std::ostream & ) const {
            throw exceptions::not_implemented("wsgi_mime::print_on( std::ostream & ) const");
        }
    };
    struct wsgi_response : boost::noncopyable {
        std::auto_ptr< mime > response;

        boost::python::object start_response(boost::python::object status, boost::python::list headers) {
            mime::mime_headers response_headers;
            for ( boost::python::stl_input_iterator<boost::python::tuple> i(headers), e; i != e; ++i )
                response_headers.set( boost::python::extract<string>((*i)[0])(), boost::python::extract<string>((*i)[1])() );
            response = std::auto_ptr< mime >( new wsgi_mime( response_headers ) );
            return boost::python::object();
        }
    };
}


fostlib::python::wsgi::application::application( const string &appname ) {
    std::size_t last_dot = appname.find_last_of(".");
    if ( last_dot == string::npos )
        throw exceptions::not_implemented("Not implemented error handling\n"
            "The application name must include a . (dot) as there must be a module to import it from", appname
        );

    m_module = boost::python::import(coerce< boost::python::str >(appname.substr(0, last_dot)));
    m_application = boost::python::getattr(m_module, coerce< boost::python::str >(appname.substr(last_dot + 1)));
    boost::python::class_< wsgi_response, boost::shared_ptr< wsgi_response >, boost::noncopyable >(
        "wsgi_response", boost::python::no_init
    )
        .def("start_response", &wsgi_response::start_response)
    ;
}

std::auto_ptr< mime > fostlib::python::wsgi::application::operator () (http::server::request &req) const {
    // Set up the environment for the request
    boost::python::dict environ;
    environ["wsgi.version"] = boost::python::make_tuple(1, 0);
    environ["wsgi.url_scheme"] = boost::python::str("http");
    environ["wsgi.multithread"] = false;
    environ["wsgi.multiprocess"] = false;
    environ["wsgi.run_once"] = false;

    environ["SCRIPT_NAME"] = boost::python::str();
    environ["PATH_INFO"] = boost::python::str(req.file_spec().underlying().underlying());
    environ["SERVER_NAME"] = boost::python::str("localhost");
    environ["SERVER_PORT"] = boost::python::str(coerce< string >( 8001 ));

    for ( mime::mime_headers::const_iterator header( req.data().headers().begin() ); header != req.data().headers().end(); ++header ) {
        std::string name = replaceAll(header->first, L"-", L"_").std_str();
        for ( std::string::iterator c(name.begin()); c != name.end(); ++c)
            *c = std::toupper(*c);
        environ["HTTP_" + name] = header->second.value();
    }

    // Process the request
    boost::shared_ptr< wsgi_response > response( new wsgi_response );
    boost::python::object response_object = boost::python::object(response);

    boost::python::object strings = m_application(environ, boost::python::getattr(response_object, "start_response"));
    utf8string result;
    for ( boost::python::stl_input_iterator<utf8string> i(strings), e; i != e; ++i )
        result += *i;
    return response->response;
}
