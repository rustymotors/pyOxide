"""Django models for pyOxide admin interface."""

from django.db import models


class ServerStatus(models.Model):
    """Log entries for server operations."""

    timestamp = models.DateTimeField(auto_now_add=True)
    level = models.CharField(
        max_length=20,
        choices=[
            ("INFO", "Info"),
            ("WARNING", "Warning"),
            ("ERROR", "Error"),
            ("DEBUG", "Debug"),
        ],
    )
    message = models.TextField()
    source = models.CharField(max_length=50, default="pyOxide")

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.timestamp} - {self.level}: {self.message[:50]}"


class APIRequest(models.Model):
    """Track API requests for monitoring."""

    timestamp = models.DateTimeField(auto_now_add=True)
    method = models.CharField(max_length=10)
    path = models.CharField(max_length=200)
    status_code = models.IntegerField()
    response_time_ms = models.FloatField(null=True, blank=True)
    user_agent = models.TextField(blank=True)
    ip_address = models.GenericIPAddressField(null=True, blank=True)

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.method} {self.path} - {self.status_code}"


class Configuration(models.Model):
    """Store configuration settings."""

    key = models.CharField(max_length=100, unique=True)
    value = models.TextField()
    description = models.TextField(blank=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self) -> str:
        return f"{self.key}: {self.value[:30]}"
