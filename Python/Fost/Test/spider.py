# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

import cookielib, glob, os, random, unittest, urllib, urllib2, urlparse
from BeautifulSoup import BeautifulSoup
from Fost.internet import x_www_form_urlencoded
from Fost.internet.useragent import agent

import Fost.settings
fostsettings = Fost.settings.database()

def queue_links(spider, response):
    soup = response.soup
    # Check links in a random order
    links = soup.findAll('a')
    random.shuffle(links)
    for link in links:
        if link.has_key('href') and not link['href'].startswith('http') and not link['href'].startswith('/__'):
            spider.spider_test(urlparse.urljoin(response.url, link['href']))
    return soup
def ignore_links(spider, response):
    return response.soup


class Spider(object):
    def __init__(self, urls = [], visited = {}, links = queue_links):
        self.agent = agent()
        self.suite = unittest.TestSuite()
        self.pages = dict([(urlparse.urljoin(fostsettings["Spider", "host"], url), v) for url, v in visited.items()])
        [self.spider_test(urlparse.urljoin(fostsettings["Spider", "host"], url)) for url in urls]
        self.links = queue_links

    def url_data(self, url):
        if url.find('?') > 0:
            url = url[:url.find('?')]
        if not self.pages.has_key(url):
            self.pages[url] = dict()
        if not self.pages[url].has_key('remaining'):
            self.pages[url]['remaining'] = 1
        return self.pages[url]

    def run_suite(self):
        unittest.TextTestRunner().run(self.suite)

    def addTest(spider, url, data=None, ql = queue_links, url_data = None):
        if not url_data: url_data = spider.url_data
        if url[0]=='/':
            url = urlparse.urljoin(fostsettings["Spider", "host"], url)
            if not spider.pages.has_key(url):
                spider.pages[url] = dict()
            if not spider.pages[url].has_key('remaining'):
                spider.pages[url]['remaining'] = 1

        def test_process(self, response):
            soup = self.links(spider, response)
            # Look for forms to submit
            if soup and url_data(response.url).get('use_forms', True):
                for form in soup.findAll('form'):
                    spider_url_data = url_data(response.url)
                    form_id = form.get('id', form.get('name', None))
                    if form_id and spider_url_data.has_key('forms') and spider_url_data['forms'].has_key(form_id):
                        form_data = spider_url_data['forms'][form_id]
                    else:
                        form_data = spider_url_data
                    spider.process_form(response, form, form_data, lambda s, r: r.soup)
        def test_runTest(self):
            self.process(spider.agent.process(url, spider.url_data(url), data))

        testtype = type(str(url), (unittest.TestCase,), dict(
            process = test_process,
            runTest = test_runTest,
        ))
        def test_constructor(self, *args, **kwargs):
            super(testtype, self).__init__(*args, **kwargs)
            self.links = ql
        testtype.__init__ = test_constructor

        spider.suite.addTest(testtype())

    def spider_test(spider, url, data=None):
        if spider.suite.countTestCases() < fostsettings["Spider", "Count"]:
            if spider.url_data(url)['remaining']:
                spider.url_data(url)['remaining'] -= 1
                spider.addTest(url, data)

    def test_form(spider, url, form_id, data = {}, check_fn = lambda s, r: r.soup):
        response = spider.agent.process(url)
        form = response.soup.find(id=form_id)
        spider.process_form(response, form, data, check_fn)

    def process_form(spider, page_response, form, data = {}, check_fn = lambda s, r: r.soup):
        assert form, page_response.soup
        submit, query = build_form_query(spider, form, page_response.url)
        for k, v in data.items():
            if v is None:
                if query.has_key(k): query.pop(k)
            else:
                query[k] = v
        if form.get('method', 'get').lower() == 'get':
            response = spider.agent.process(
                urlparse.urljoin(page_response.url, u'%s?%s' % (form['action'], x_www_form_urlencoded(query)))
            )
        elif submit:
            response = spider.agent.process(
                urlparse.urljoin(page_response.url, form['action']),
                {},
                x_www_form_urlencoded(query)
            )
        else:
            return None
        check_fn(spider, response)
        return response

spider_instance = Spider()


def test_response(test, response):
    if response.status_code in [301, 302, 303]:
        response_location = response['Location']
        if 'https' in response_location:
            raise Exception("Need to turn off the HTTPS middleware")
        response_location = response_location[response_location.find('/office'):]
        if '?' in response_location:
            location, qstring = response_location.split('?')
            qstring = qstring.split('&')
            query = {}
            for qpart in qstring:
                if qpart:
                    key, value = qpart.split('=')
                    query[key] = value
            print '->', location, query
            test_response(test, test.client.get(location, query))
        else:
            print '->', response_location
            test_response(test, test.client.get(response_location))
            print '-> POST', response_location
            test_response(test, test.client.post(response_location))
    else:
        test.assert_(response.status_code in [200], "Incorrect response status of %s" % response.status_code)
    return response


def build_form_query(test, form, base_url, form_data = {}):
    query, submits, radios = {}, [], {}
    assert form.has_key('action') and form['action'], u'Empty action in %s' % form
    for ta in form.findAll('textarea'):
        assert ta.has_key('name'), u'%s in %s' % (ta, base_url)
        assert len(ta.contents) <= 1, u"Content of a textarea should just be some text\n" % ta.contents
        if len(ta.contents) == 1:
            [query[ta['name']]] = ta.contents
    for inp in form.findAll('input'):
        input_type = inp.get('type', 'text')
        if input_type == "submit":
            submits.append(inp)
        elif input_type == "checkbox":
            if inp.has_key('checked') and inp.get('disabled', 'false').lower() != 'true':
                query[inp['name']] = inp.get('value', "")
        elif not input_type == "reset":
            assert inp.has_key('name'), u'%s in %s' % (inp, base_url)
            query[inp['name']] = inp.get('value', "")
    for select in form.findAll('select'):
        assert select.has_key('name'), u'Select in form at %s has no name\n%s' % (base_url, select)
        options = select.findAll('option')
        assert len(options), u"No options found in select at %s" % base_url
        for option in options:
            assert option.has_key('value'), u'No value found for option %s in select at %s' % (option, base_url)
            if option.get('selected', None):
                query[select['name']] = option['value']
        if not query.has_key(select['name']) and len(options):
            query[select['name']] = options[0]['value']
    if len(submits):
        button = submits[random.randint(0, len(submits) - 1)]
        if button.has_key('name'):
            query[button['name']] = button['value']
        return True, query
    return False, query


def main(path=None, host=None, **kwargs):
    """
        This spider will start off running queries that it finds in a JSON configuration file.
    """
    print "Havok spider - Copyright (C) 2008-2009 Felspar Co Ltd."
    def run_blob(config_file):
        local_settings = Fost.settings.database()
        # Load configuration
        from Fost.json import root as jroot, jsonblob
        if config_file:
            configuration = jsonblob(config_file)
        else:
            configuration = jsonblob()
        local = configuration.local
        # Base URL configuration
        if local.has_key(jroot/"url"):
            local_settings["Spider", "host"] = local[jroot/"url"]
        if host:
            local_settings["Spider", "host"] = host
        # Number of tests configuration
        if local.has_key(jroot/"count"):
            local_settings["Spider", "Count"] = local[jroot/"count"]
        if kwargs.has_key('t'):
            local_settings["Spider", "Count"] = int(kwargs['t'])
        # Make spider and run
        spider = Spider(
            local[jroot / "urls"] if local.has_key(jroot/"urls") else ['/'],
            local[jroot / "pages"] if local.has_key(jroot/"pages") else {}
        )
        if local.has_key(jroot/'fost_authentication'):
            spider.agent.fost_authenticate(
                local[jroot/'fost_authentication'/'key'], local[jroot/'fost_authentication'/'secret'],
                local[jroot/'fost_authentication'/'headers']
            )
        spider.run_suite()
    if os.path.isdir(path):
        for infile in glob.glob(os.path.join(path, '*.json')):
            run_blob(infile)
    else:
        run_blob(path)
