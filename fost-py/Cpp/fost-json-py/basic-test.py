# -*- coding: utf-8 -*-
# Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import _json_fost

assert _json_fost.parse("{}") == dict(), _json_fost.parse("{}")
assert _json_fost.unparse(_json_fost.parse("{}"), False) == "{}", _json_fost.unparse(_json_fost.parse("{}"), False)
assert _json_fost.unparse(_json_fost.parse("{}"), True) == "{\n}\n", _json_fost.unparse(_json_fost.parse("{}"), True)

assert _json_fost.unparse(None, False) == "null", _json_fost.unparse(None, False)
assert _json_fost.unparse(12, False) == "12", _json_fost.unparse(12, False)
assert _json_fost.unparse({}, False) == "{}", _json_fost.unparse({}, False)
assert _json_fost.unparse(dict(), False) == "{}", _json_fost.unparse({}, False)
assert _json_fost.unparse([12], False) == "[12]", _json_fost.unparse([12], False)
assert _json_fost.unparse(set([1,2,4]), False) == "[1,2,4]", _json_fost.unparse(set([1,2,4]), False)
