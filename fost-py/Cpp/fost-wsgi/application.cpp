/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-wsgi.hpp"
#include <fost/pyhost>
#include <fost/threading>
#include <fost/detail/wsgi.application.hpp>

#include <boost/python/stl_iterator.hpp>


using namespace fostlib;


namespace {
    /*
        Because start_response can be called a number of times we need to
        process things in an odd order.
        The wsgi_mime instance could be replaced multiple times up until the
        point where the first response iterable
        returns a non-empty string. This is watched for with the
        start_response_illegal flag.
    */
    struct wsgi_mime : public mime {
        // This is set true after the iterable yields its first non-empty string
        bool start_response_illegal;
        // We need to store the start_response closure here so that it can
        // error when the illegal flag is set
        boost::python::object start_response_closure;
        // The list iterable that is returned from start_response
        boost::python::object output_iterator;
        // The current iterable positions
        boost::python::stl_input_iterator<boost::python::str> current, end;
        // The first block needs to be handled in a special way
        boost::python::str first_block;

        wsgi_mime( const mime_headers &h )
        : mime( h, h.exists("Content-Type") ? h["Content-Type"].value() : "text/plain" ),
                start_response_illegal( false ) {
        }

        /*
            There are some things we can do to help to optimize things after
            we know what the headers will be and before we start to send
            data. Each of these should be optional (controlled through settings).
        */
        void optimize() {
        }

        struct iterator_wrapper : public iterator_implementation {
            bool sent_first;
            const boost::python::str &first;
            boost::python::stl_input_iterator<boost::python::str> current, end;
            std::string this_block;
            iterator_wrapper(
                const boost::python::str &first,
                boost::python::stl_input_iterator<boost::python::str> current,
                boost::python::stl_input_iterator<boost::python::str> end
            ) : sent_first( false ), first( first ), current( current ), end( end ) {
            }
            const_memory_block from_python_str( const boost::python::str &s ) {
                this_block = boost::python::extract<std::string>(s)();
                return const_memory_block(
                    this_block.c_str(), this_block.c_str() + this_block.length()
                );
            }
            const_memory_block operator () () {
                fostlib::python::inproc_host::gil gil;
                if ( !sent_first ) {
                    sent_first = true;
                    return from_python_str(first);
                } else if ( current == end )
                    return const_memory_block(NULL, NULL);
                else
                    throw exceptions::not_implemented(
                        "wsgi_mime::const_iterator::operator () () -- subsequent parts"
                    );
            }
        };
        std::auto_ptr<iterator_implementation> iterator() const {
            return std::auto_ptr<iterator_implementation>(new iterator_wrapper(
                first_block, current, end
            ));
        }

        bool boundary_is_ok( const string &boundary ) const {
            throw exceptions::not_implemented(
                "wsgi_mime::boundary_is_ok( const string &boundary ) const"
            );
        }
        std::ostream &print_on( std::ostream & ) const {
            throw exceptions::not_implemented(
                "wsgi_mime::print_on( std::ostream & ) const"
            );
        }
    };
    /*
        We simply aren't going to support this right now. It might be possible
        to add it in later on.
    */
    void write(boost::python::str) {
        throw exceptions::not_implemented(
            "This WSGI server does not support the write outputter"
        );
    }
    /*
        This acts as a closure where we can store the response that
        start_response starts to build.
        Each time start_response is called it will replace the existing mime.
        This allows the Python application to replace the response headers
        etc. in things like its error handling. It can do this as many times
        as it likes during the initial application start up, or when it is
        generating the first (non empty) string output. Once it has delivered
        a string then it cannot call start_response again as we have already
        started to send data to the client.
    */
    struct wsgi_response : boost::noncopyable {
        std::auto_ptr< wsgi_mime > response;

        boost::python::object start_response(
            boost::python::object status, boost::python::list headers
        ) {
            if ( response.get() && response->start_response_illegal )
                throw exceptions::not_implemented(
                    "You cannot call start_response after your iterable"
                    " returns its first non-empty string"
                );
            mime::mime_headers response_headers;
            for (
                boost::python::stl_input_iterator<boost::python::tuple> i(headers), e;
                i != e;
                ++i
            ) response_headers.set(
                boost::python::extract<string>((*i)[0])(),
                boost::python::extract<string>((*i)[1])()
            );
            response = std::auto_ptr< wsgi_mime >( new wsgi_mime(
                response_headers
            ) );
            return boost::python::object(&write);
        }
    };
}


fostlib::python::wsgi::application::application( const string &appname ) {
    std::size_t last_dot = appname.find_last_of(".");
    if ( last_dot == string::npos )
        throw exceptions::not_implemented(
            "The application name must include a . (dot) as there must be"
            " a module to import it from", appname
        );

    m_module = boost::python::import(
        coerce< boost::python::str >(appname.substr(0, last_dot))
    );
    m_application = boost::python::getattr(
        m_module, coerce< boost::python::str >(appname.substr(last_dot + 1))
    );
    boost::python::class_<
        wsgi_response, boost::shared_ptr< wsgi_response >, boost::noncopyable
    >(
        "wsgi_response", boost::python::no_init
    )
        .def("start_response", &wsgi_response::start_response)
    ;
}

std::auto_ptr< mime > fostlib::python::wsgi::application::operator () (
    http::server::request &req
) const {
    fostlib::python::inproc_host::gil gil;
    // Set up the environment for the request
    boost::python::dict environ;
    environ["wsgi.version"] = boost::python::make_tuple(1, 0);
    environ["wsgi.url_scheme"] = boost::python::str("http");
    environ["wsgi.multithread"] = true;
    environ["wsgi.multiprocess"] = false;
    environ["wsgi.run_once"] = false;

    environ["SCRIPT_NAME"] = boost::python::str();
    environ["PATH_INFO"] = boost::python::str(req.file_spec().underlying().underlying());
    environ["SERVER_NAME"] = boost::python::str("localhost");
    environ["SERVER_PORT"] = boost::python::str(coerce< string >( 8001 ));

    for (
        mime::mime_headers::const_iterator header( req.data()->headers().begin() );
        header != req.data()->headers().end();
        ++header
    ) {
        boost::python::str name =
            boost::python::str(header->first).replace("-", "_").upper();
        environ["HTTP_" + name] = header->second.value();
    }

    // Process the request
    boost::shared_ptr< wsgi_response > response( new wsgi_response );
    boost::python::object response_object = boost::python::object(response);

    boost::python::object strings = m_application(
        environ, boost::python::getattr(response_object, "start_response")
    );
    boost::python::stl_input_iterator<boost::python::str> current(strings), end;
    // Once we have a valid first string then we need to send it on
    boost::python::str first_string;
    while ( boost::python::len(first_string) == 0 && current != end )
        first_string = *current++;
    response->response->start_response_illegal = true;
    response->response->start_response_closure = response_object;
    response->response->first_block = first_string;
    response->response->output_iterator = strings;
    response->response->current = current;
    response->response->end = end;
    response->response->optimize();
    return std::auto_ptr< mime >( response->response.release() );
}
