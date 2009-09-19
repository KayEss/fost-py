# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

class Prototype:
    def __init__(self, parent, prototype):
        self.__parent = parent
        self.__prototype = prototype

    def fetch(self, key, default=None):
        if self.__prototype.has_key(key):
            if self.__prototype[key] == None:
                return default
            else:
                return self.__prototype[key]
        elif self.__parent != None:
            return self.__parent.fetch(key, default)
        else:
            return default

Prototype.root = Prototype(None, {})
