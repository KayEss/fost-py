# -*- coding: utf-8 -*-
# Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

from Fost.crypto import sha1_hmac
from Fost.settings import database
from _internet import url_filespec_encode
import django.contrib.auth
import datetime

db = database()

class Authenticate:
    def authenticate(self, **kwargs):
        if kwargs.has_key('request') and kwargs.has_key('key') and kwargs.has_key('hmac'):
            #print kwargs['key'], kwargs['hmac']
            request = kwargs['request']
            d, now = datetime.datetime.strptime(request.META['HTTP_X_FOST_TIMESTAMP'][:19], '%Y-%m-%d %H:%M:%S'), datetime.datetime.utcnow()
            delta = datetime.timedelta(0, 300) # TODO: Read from settings DB
            if max(d - now, now - d) < delta and db.has_key('Django authentication middleware', kwargs['key']):
                document = '%s %s\n%s\n%s\n%s' % (
                    request.method, url_filespec_encode(request.path),
                    request.META['HTTP_X_FOST_TIMESTAMP'],
                    '\n'.join([
                        '%s' % request.META['HTTP_' + k.replace('-', '_').upper()]
                        for k in request.META['HTTP_X_FOST_HEADERS'].split()
                    ]),
                    request.META['QUERY_STRING'] or request.raw_post_data
                )
                #print document
                #print sha1_hmac(db['Django authentication middleware', kwargs['key']], document), kwargs['hmac']
                if sha1_hmac(db['Django authentication middleware', kwargs['key']], document) == kwargs['hmac']:
                    user = django.contrib.auth.models.User.objects.get(username=request.META['HTTP_X_FOST_USER'])
                    return user
                #else:
                #    print sha1_hmac(db['Django authentication middleware', kwargs['key']], document), kwargs['hmac']
        return None

    def get_user(self, user_id):
        try:
            return django.contrib.auth.models.User.objects.get(pk=user_id)
        except django.contrib.auth.models.User.DoesNotExist:
            return None


class Middleware:
    def process_request(self, request):
        #print "Authorization", request.META.has_key('HTTP_AUTHORIZATION')
        if request.META.has_key('HTTP_AUTHORIZATION'):
            [mechanism, authorization] = request.META['HTTP_AUTHORIZATION'].split()
            #print mechanism, authorization
            if mechanism == 'FOST':
                """for k in request.META.keys():
                    if k.startswith('HTTP'):
                        print k, "'%s'" % request.META[k]"""
                [username, password] = authorization.split(':')
                request.user = django.contrib.auth.authenticate(
                    request = request,
                    key = username,
                    hmac = password
                )
