# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

from Fost.Test.prototype import *
import unittest


class TestPrototype(unittest.TestCase):
    def setUp(self):
        pass


    def testroot(self):
        self.assertEquals(Prototype.root.fetch('hello'), None)
        self.assertEquals(Prototype.root.fetch('world'), None)

    def testfirst(self):
        first = Prototype(Prototype.root,{'hello':'world'})
        self.assertEquals(first.fetch('hello'), 'world')
        self.assertEquals(first.fetch('world'), None)
        self.assertEquals(first.fetch('world', 'goodbye'), 'goodbye')

    def testsecond(self):
        first = Prototype(Prototype.root,{'hello':'world'})
        second = Prototype(first, {'world':'goodbye'})
        self.assertEquals(second.fetch('hello'), 'world')
        self.assertEquals(second.fetch('world'), 'goodbye')

    def testthird(self):
        first = Prototype(Prototype.root,{'hello':'world'})
        second = Prototype(first, {'world':'goodbye'})
        third = Prototype(second, {'world': None})
        self.assertEquals(third.fetch('hello'), 'world')
        self.assertEquals(third.fetch('world'), None)
        self.assertEquals(third.fetch('world', 'goodbye'), 'goodbye')


    def tearDown(self):
            pass


if __name__ == "__main__":
    unittest.main()

