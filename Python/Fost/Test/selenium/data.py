# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

class User:
    def __init__(self, disambig=None):
        if disambig == None:
            self.name = "sample"
            self.email = "sample@example.com"
        else:
            self.name = "sample" + disambig
            self.email = "sample+" + disambig + "@example.com"
        self.password = "samples"

    def login(self, selenium):
        selenium.click("link=Log In")
        selenium.wait_for_page_to_load("30000")
        selenium.type("username", self.name)
        selenium.type("password", self.password)
        selenium.click("Submit")
        selenium.wait_for_page_to_load("30000")

    def logout(self, selenium):
        selenium.click("link=Log off")
        selenium.wait_for_page_to_load("30000")
        selenium.click("Submit")
        selenium.wait_for_page_to_load("30000")


class Person:
    def __init__(self, disambig=None):
        self.honorific = ''
        self.firstName = 'Kay'
        self.extraName = ''
        self.secondName = 'Ess'


class Company:
    def __init__(self, disambig=None):
        self.name = "Sample Company Ltd"
        if disambig != None:
            self.name += " (%s)" % disambig


class Address:
    def __init__(self, disambig=None):
        self.line1 = disambig + " High Street"
        self.line2 = ""
        self.line3 = ""
        self.area = "Test Data"
        self.town = "Brighton"
        self.postcode = "BN1 4GT"
        self.country = "UK"

