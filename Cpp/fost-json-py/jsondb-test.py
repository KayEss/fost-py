# -*- coding: utf-8 -*-
# Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt
import _json_fost, os

def rmfile(f):
    if os.path.isfile(f):
        os.remove(f)

# Perform some basic tests to show we can create the objects
blob1 = _json_fost.jsonblob()
assert blob1.filename == None, "filename: '%s'" % blob1.filename
blob2 = _json_fost.jsonblob("../fost-py/Cpp/fost-json-py/testfile.json")
assert blob2.filename == "../fost-py/Cpp/fost-json-py/testfile.json", "filename: '%s'" % blob2.filename

# Make sure we can make a file on disk
rmfile("Cpp/fost-json-py/testfile-new.json")
blob3 = _json_fost.jsonblob("../fost-py/Cpp/fost-json-py/testfile-new.json", dict(
    hello = 'nightclub',
    goodbye = 'country',
))

# Aliasing the same database
blob4 = blob2
assert blob4.filename == "../fost-py/Cpp/fost-json-py/testfile.json", "filename: '%s'" % blob4.filename

# Grab a local transaction from the BLOB
local3 = blob3.local
assert local3 == dict(hello = 'nightclub', goodbye = 'country'), local3.json

# Grab two locals, update 1 and check that is not visible in the other then do a commit and check again
local1 = blob3.local
local2 = blob3.local
local1[_json_fost.jcursor()/'hello'] = 'goodbye'
assert local1 == dict(hello = 'goodbye', goodbye = 'country'), local1
assert local2 == dict(hello = 'nightclub', goodbye = 'country'), local2
local1.commit()
assert local1 == dict(hello = 'goodbye', goodbye = 'country'), local1
# This is right because local2's transaction started before the commit
assert local2 == dict(hello = 'nightclub', goodbye = 'country'), local2

# This time grab the second local after the update has started on the first
# This indrectly tests that the earlier transaction becomes visible to later ones
local1 = blob3.local
local1[_json_fost.jcursor()/'hello'] = 'goodbye 2'
local2 = blob3.local
assert local1 == dict(hello = 'goodbye 2', goodbye = 'country'), local1
assert local2 == dict(hello = 'goodbye', goodbye = 'country'), local2
local1.commit()
assert local1 == dict(hello = 'goodbye 2', goodbye = 'country'), local1
# This is still right because local2's transaction started before the commit
assert local2 == dict(hello = 'goodbye', goodbye = 'country'), local2


# Tidy up files
rmfile("../fost-py/Cpp/fost-json-py/testfile-new.json")
