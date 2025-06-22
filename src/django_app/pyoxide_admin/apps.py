"""Django app configuration for pyOxide admin."""

from django.apps import AppConfig


class PyoxideAdminConfig(AppConfig):
    """Configuration for pyOxide admin app."""

    default_auto_field = "django.db.models.BigAutoField"
    name = "src.django_app.pyoxide_admin"
    verbose_name = "pyOxide Administration"
