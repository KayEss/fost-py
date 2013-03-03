# -*- coding: utf-8 -*-
# Copyright 2008-2013, Felspar Co Ltd. http://support.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt
import _settings

def test():
    """
        This function will create only local settings which will disappear after it terminates
    """
    s = _settings.settings()
    print s.get("Settings database tests", "Loaded base settings file")
    assert s.has_key("Settings database tests", "Loaded base settings file")
    assert s.get("Settings database tests", "Loaded base settings file") == True

    s.set("Settings database tests", "Loaded base settings file", "HTML")
    assert s.get("Settings database tests", "Loaded base settings file") == "HTML"

test()
n = _settings.settings()
assert n.get("Settings database tests", "Loaded base settings file") == True

# Load an ini file and make sure we can read a value from it
n.file("../fost-py/Cpp/fost-settings-py/test.ini")
assert n.get("Test ini", "A value") == True


# Make sure grabbing non-existant value throws
assert not n.has_key("Not a section name", "Not a value name")
try:
    n.get("Not a section name", "Not a value name")
    assert False
except:
    pass
