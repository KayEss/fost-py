# -*- coding: utf-8 -*-
import _schema

"""
    Check constructors
"""
hello = _schema.enclosure("hello")
assert hello.fq_name('.') == "hello", hello.fq_name('.')
assert _schema.enclosure(hello, "there").fq_name('.') == "hello.there", _schema.enclosure(hello, "there").fq_name('.')

world = _schema.meta_instance(hello, "world")
assert world.fq_name('.') == "hello.world", world.fq_name('.')

goodbye = _schema.meta_instance("goodbye")
assert goodbye.fq_name('.') == "goodbye", goodbye.fq_name('.')

country = _schema.meta_instance(goodbye, "country")
assert country.fq_name('.') == "goodbye.country", country.fq_name('.')
