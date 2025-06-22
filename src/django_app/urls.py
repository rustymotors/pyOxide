"""URL configuration for embedded Django app."""

from django.contrib import admin
from django.urls import path, include
from django.conf import settings
from django.conf.urls.static import static
from . import views

urlpatterns = [
    path("admin/", admin.site.urls),
    path("api/django/", views.api_info, name="django-api-info"),
    path("api/django/data/", views.data_management, name="django-data"),
    path("api/django/users/", views.user_management, name="django-users"),
    path("dashboard/", views.dashboard, name="django-dashboard"),
]

# Serve static files during development
if settings.DEBUG:
    # Serve Django admin static files
    import django
    import os
    admin_static_root = os.path.join(django.__path__[0], 'contrib', 'admin', 'static')
    urlpatterns += static('/static/', document_root=admin_static_root)
