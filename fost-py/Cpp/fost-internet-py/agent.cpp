/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "agent.hpp"
#include <fost/http>
#include <boost/lambda/bind.hpp>


using namespace fostlib;


void ua_fost_authenticate(
    http::user_agent &ua, const string &key, const string &secret
) {
    fostlib::http::fost_authentication(ua, key, secret);
}

std::string ua_response_body(http::user_agent::response &response) {
    std::string ret;
    boost::shared_ptr< const binary_body > body = response.body();
    for ( mime::const_iterator part( body->begin() ); part != body->end(); ++part )
        ret += std::string(
            reinterpret_cast< nliteral >((*part).first),
            reinterpret_cast< nliteral >((*part).second)
        );
    return ret;
}
