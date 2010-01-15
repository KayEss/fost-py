import _test

class SomeClass:
    """Just a simple class whose instances can be converted to Unicode"""
    def __unicode__(self):
        return u"Some class instance"

# Some basic tests for passing and returning strings
assert _test.annotate("Some string") == "Passed in 'Some string'", _test.annotate("Some string")
assert _test.annotate(u"Some string") == "Passed in 'Some string'", _test.annotate(u"Some string")

# This should throw as we can't pass None instead of a string
try:
    _test.annotate(None)
    assert False, "Should have gotten an exception as we can't pass None to a func that takes fostlib::string"
except:
    pass


# Test what happens with a class instance
assert _test.annotate(unicode(SomeClass())) == "Passed in 'Some class instance'", _test.annotate(unicode(SomeClass()))
try:
    # It might be nice if this would work, but we then have to manually call __unicode__ on the instance
    _test.annotate(SomeClass())
    assert False, "Should have gotten an exception as we have to manually convert to unicode"
except:
    pass

# Check that we can fetch None from a nullable< string > function and that we can pass in strings and None
assert _test.getnull() == None, _test.getnull()
assert _test.isnone(None), _test.isnone(None)
assert not _test.isnone("Something"), _test.isnone("Something")
assert not _test.isnone(unicode(SomeClass())), _test.isnone(unicode(SomeClass()))
try:
    _test.isnone(SomeClass())
    assert False, "Should have gotten an exception as we have to manually convert to unicode"
except:
    pass
# This last one is odd, we would probably prefer this to be different
assert not _test.isnone(unicode(None)), _test.annotate(unicode(None))
