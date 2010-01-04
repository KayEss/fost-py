# Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import _json_fost

# Basic constructor
j1 = _json_fost.jcursor()

# Add to the path of the jcursor
j1 /= "path"
j2 = _json_fost.jcursor() / "path"
assert j1 == j2
assert j1 == _json_fost.jcursor() / "path"
assert j2 == j1
assert j1 != _json_fost.jcursor()
assert j2 != _json_fost.jcursor() / "no path"

# Length checking
assert len(j1) == 1
assert len(j2) == 1
assert len(j1 / 2) == 2

assert j1[0] == "path"
assert j2[0] == "path"
assert (j2/2)[1] == 2
try:
    j1[1]
    assert false, "Should have had an exception"
except:
    pass
