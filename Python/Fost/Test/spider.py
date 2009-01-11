import unittest, urllib, urllib2, urlparse, random
from BeautifulSoup import BeautifulSoup

spidered_urls = {}

def spider_test(suite, url):
    import Fost.settings as settings
    global spidered_urls
    if len(spidered_urls) < int(settings.database()[("Spider", "Count")]) - 1 and not spidered_urls.has_key(url):
        spidered_urls[url] = True
        class Test(unittest.TestCase):
            def fetch(self, fetch, data = None):
                try:
                    response = urllib2.urlopen(fetch, data)
                    if response.url != url:
                        spidered_urls[response.url] = True
                    return response.read()
                except urllib2.HTTPError, e:
                    if data:
                        self.assert_(False, u"HTTP error with POST against %s with data\n%s\nBase URL %s\n%s\n\n%s" % (fetch, e, url, data, e.read()))
                    elif data == "":
                        self.assert_(False, u"HTTP error with POST against %s with empty data\n%s\nBase URL %s\n\n%s" % (fetch, e, url, e.read()))
                    else:
                        self.assert_(False, u"HTTP error with GET against %s\n%s\nBase URL %s\n\n%s" % (fetch, e, url, e.read()))
            def runTest(self):
                soup = BeautifulSoup(self.fetch(url))
                # Check links in a random order
                links = soup.findAll('a')
                random.shuffle(links)
                for link in links:
                    if link.has_key('href') and not link['href'].startswith('http') and not link['href'].startswith('/extranet/logout/'):
                        spider_test(suite, urlparse.urljoin(url, link['href']))
                # Look for forms to submit
                for form in soup.findAll('form'):
                    submit, query = build_form_query(self, form, url)
                    if submit:
                        if form.get('method', 'get') == 'get':
                            self.fetch(urlparse.urljoin(url, u'%s?%s' % (form['action'], urllib.urlencode(query))))
                        else:
                            self.fetch(urlparse.urljoin(url, form['action']), urllib.urlencode(query))
        suite.addTest(Test())


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
                key,value = qpart.split('=')
                query[key] = value
            print '->', location, query
            test_response(test, test.client.get(location, query))
        else:
            print '->', response_location
            test_response(test, test.client.get(response_location))
            print '-> POST', response_location
            test_response(test, test.client.post(response_location))
    elif not response.status_code in [200, 403]:
        test.assertIn(response.status_code, [200, 403])
    return response


def build_form_query(test, form, base_url):
    query, submits, radios = {}, [], {}
    test.assert_(form.has_key('action') and form['action'], u'Empty action in %s' % form)
    for ta in form.findAll('textarea'):
        test.assert_(ta.has_key('name'), u'%s in %s' % (ta, base_url))
        test.assert_(len(ta.contents) <= 1, u"Content of a textarea should just be some text\n" % ta.contents)
        if len(ta.contents) == 1:
            [query[ta['name']]] = ta.contents
    for inp in form.findAll('input'):
        if inp['type'] == "submit":
            submits.append(inp)
        elif inp['type'] == "checkbox":
            if inp.has_key('checked'):
                query[inp['name']] = inp.get('value', "")
        elif not inp['type'] == "reset":
            test.assert_(inp.has_key('name'), u'%s in %s' % (inp, base_url))
            query[inp['name']] = inp.get('value', "")
    for select in form.findAll('select'):
        test.assert_(select.has_key('name'), u'Select in form at %s has no name\n%s' % (base_url, select))
        options = select.findAll('option')
        test.assert_(len(options), u"No options found in select at %s" % base_url)
        for option in options:
            test.assert_(option.has_key('value'), u'No value found for option %s in select at %s' % (option, base_url))
            query[select['name']] = option['value']
    if len(submits):
        button = submits[random.randint(0, len(submits) - 1)]
        if button.has_key('name'):
            query[button['name']] = button['value']
        return True, query
    return False, query

