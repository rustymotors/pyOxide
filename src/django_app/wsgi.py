"""WSGI config for embedded Django application."""

import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")

application = get_wsgi_application()
