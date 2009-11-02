# -*- coding: utf-8 -*-
# Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import _json

assert _json.parse("{}") == dict(), _json.parse("{}")
assert _json.unparse(_json.parse("{}"), False) == "{}", _json.unparse(_json.parse("{}"), False)
assert _json.unparse(_json.parse("{}"), True) == "{\n}\n", _json.unparse(_json.parse("{}"), True)

assert _json.unparse(None, False) == "null", _json.unparse(None, False)
assert _json.unparse(12, False) == "12", _json.unparse(12, False)
assert _json.unparse({}, False) == "{}", _json.unparse({}, False)
assert _json.unparse(dict(), False) == "{}", _json.unparse({}, False)
assert _json.unparse([12], False) == "[12]", _json.unparse([12], False)
assert _json.unparse(set([1,2,4]), False) == "[1,2,4]", _json.unparse(set([1,2,4]), False)
