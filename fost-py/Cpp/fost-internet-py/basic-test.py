# -*- coding: utf-8 -*-
# Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

print "Importing _internet"
import _internet
print "Import success"


def check_throw(f, *args, **kwargs):
    threw = False
    try:
        f(*args, **kwargs)
    except:
        threw = True
    assert threw
def check_eq(left, right):
    assert left==right, left


print "Testing url_filespec_encode"
assert _internet.url_filespec_encode("abc") == "abc"
assert _internet.url_filespec_encode("abc ") == "abc%20"
assert _internet.url_filespec_encode(u"abc ") == u"abc%20"

print "Testing url_filespec_assert_valid"
_internet.url_filespec_assert_valid("abc")
_internet.url_filespec_assert_valid("abc/def")
check_throw(_internet.url_filespec_assert_valid, "abc def")
_internet.url_filespec_assert_valid(u"abc/def")

print "Testing x_www_form_urlencoded"
assert _internet.x_www_form_urlencoded({}) == "", _internet.x_www_form_urlencoded({})
assert _internet.x_www_form_urlencoded({'a':12}) == "a=12", _internet.x_www_form_urlencoded({})
assert _internet.x_www_form_urlencoded({'a':'b'}) == "a=b", _internet.x_www_form_urlencoded({})

print "Testing host"
check_eq( unicode(_internet.host("localhost")), "localhost" )
check_eq( unicode(_internet.host("localhost", 8888)), "localhost:8888" )
check_eq( unicode(_internet.host("localhost", 8888).name), "localhost" )

print "Testing url"
assert unicode(_internet.url()) == "http://localhost/", unicode(_internet.url())
assert unicode(_internet.url()('/test/')) == "http://localhost/test/", unicode(_internet.url()('/test/'))
assert unicode(_internet.url()('/test/?k=v')) == "http://localhost/test/?k=v", unicode(_internet.url()('/test/?k=v'))

print "Testing ua_request"
request = _internet.ua_request("PUT", _internet.url())
assert request.method == "PUT", request.method
assert unicode(request.url) == "http://localhost/", unicode(request.url)
request = _internet.ua_request("PUT", _internet.url(), "some data")
assert request.data == "some data", request.data
assert request.data == "some data", request.data # Make sure data is not consumed by the first call

print "Testing user_agent and ua_response - HEAD"
ua = _internet.user_agent()
response = ua(_internet.ua_request("HEAD", _internet.url("http://www.google.com/")))
assert unicode(response.url) == "http://www.google.com/", unicode(response.url)
assert response.method == "HEAD", response.method
assert response.protocol.startswith("HTTP/"), response.protocol
assert response.status == 302, response.status
assert response.message in ["Found", "Moved Temporarily"], response.message
assert not len(response.body), response.body

print "Testing user_agent and ua_response - GET"
ua = _internet.user_agent(_internet.url("http://svn.felspar.com/"))
response = ua(_internet.ua_request("GET", ua.base("/public/")))
assert unicode(response.url) == "http://svn.felspar.com/public/", unicode(response.url)
assert response.status == 200, response.status

print "Testing pop3 mailbox iteration"
check_throw(_internet.iterate_pop3_mailbox, 
    _internet.host("localhost"),
    lambda x: False,
    "nouser", "password"
)
