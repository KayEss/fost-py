# Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#     http://www.boost.org/LICENSE_1_0.txt

def reset_sequence_points(settings):
    from django.db import connection
    from django.db.models import get_apps, get_models
    from django.core.management.color import no_style
    import psycopg2
    dbcnx = psycopg2.connect('user=%s dbname=%s password=%s host=%s' % (
        settings.DATABASE_USER,
        settings.DATABASE_NAME,
        settings.DATABASE_PASSWORD,
        settings.DATABASE_HOST or 'localhost'
    ))
    cursor = dbcnx.cursor()
    for app in get_apps():
        for statement in connection.ops.sequence_reset_sql(no_style(), get_models(app)):
            cursor.execute(statement)
        dbcnx.commit()
