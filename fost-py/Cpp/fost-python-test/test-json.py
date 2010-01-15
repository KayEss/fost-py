import _test, sys

def to_json(value, expected, processed = None):
    if processed is None:
        processed = value
    print value, "->", expected, "->", processed
    sys.stdout.flush()
    assert _test.tojson(value, False) == "%s" % expected, _test.tojson(value, True)
    assert _test.fromjson(_test.tojson(value, False)) == processed, _test.fromjson(_test.tojson(value, False))

to_json(None, "null")
to_json(True, "true")
to_json(False, "false")
to_json(0, "0")
to_json(123, "123")
to_json(123.45, "123.45")
to_json('some string', '"some string"')
to_json("""some string\twith a tab""", '"some string\\twith a tab"')
to_json([], "[]")
to_json([1,2], "[1,2]")
to_json([1,2,[3,"hello"]], "[1,2,[3,\"hello\"]]")
to_json((1,2), "[1,2]", [1,2])
to_json((1,2,[3,"hello"]), "[1,2,[3,\"hello\"]]", [1,2,[3,"hello"]])
to_json(dict(hello="country", goodbye="nightclub"), '{"goodbye":"nightclub","hello":"country"}')

to_json(dict(
    hello="country",
    goodbye="nightclub",
    tracks=[],
), '{"goodbye":"nightclub","hello":"country","tracks":[]}')

for v in range(1, 10):
    to_json(dict(a=1, b=None, c=dict(start=None, end=None)), '{"a":1,"b":null,"c":{"end":null,"start":null}}')
