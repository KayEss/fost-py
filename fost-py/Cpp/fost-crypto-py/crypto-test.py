import _crypto

assert _crypto.md5("") \
    == "d41d8cd98f00b204e9800998ecf8427e", \
    _crypto.md5("")
assert _crypto.md5(u"The quick brown fox jumps over the lazy dog") \
    == "9e107d9d372bb6826bd81d3542a419d6", \
    _crypto.md5("The quick brown fox jumps over the lazy dog")
assert _crypto.md5("The quick brown fox jumps over the lazy dog.") \
    == "e4d909c290d0fb1ca068ffaddf22cbd0", \
    _crypto.md5("The quick brown fox jumps over the lazy dog")

assert _crypto.sha1("The quick brown fox jumps over the lazy dog") \
    == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", \
    _crypto.sha1("The quick brown fox jumps over the lazy dog")
assert _crypto.sha1(u"The quick brown fox jumps over the lazy dog") \
    == "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12", \
    _crypto.sha1(u"The quick brown fox jumps over the lazy dog")

assert _crypto.sha1_hmac(
        "uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/johnsmith/photos/puppy.jpg"
    ) == "xXjDGYUmKxnwqr5KXNPGldn5LbA=", \
    _crypto.sha1_hmac(
        "uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/johnsmith/photos/puppy.jpg"
    )
