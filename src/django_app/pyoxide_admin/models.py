"""Django models for pyOxide admin interface."""

from typing import Any

from django.db import models


class ServerStatus(models.Model):
    """Log entries for server operations."""

    timestamp: Any = models.DateTimeField(auto_now_add=True)
    level: Any = models.CharField(
        max_length=20,
        choices=[
            ("INFO", "Info"),
            ("WARNING", "Warning"),
            ("ERROR", "Error"),
            ("DEBUG", "Debug"),
        ],
    )
    message: Any = models.TextField()
    source: Any = models.CharField(max_length=50, default="pyOxide")

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.timestamp} - {self.level}: {self.message[:50]}"


class APIRequest(models.Model):
    """Track API requests for monitoring."""

    timestamp: Any = models.DateTimeField(auto_now_add=True)
    method: Any = models.CharField(max_length=10)
    path: Any = models.CharField(max_length=200)
    status_code: Any = models.IntegerField()
    response_time_ms: Any = models.FloatField(null=True, blank=True)
    user_agent: Any = models.TextField(blank=True)
    ip_address: Any = models.GenericIPAddressField(null=True, blank=True)

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.method} {self.path} - {self.status_code}"


class Configuration(models.Model):
    """Store configuration settings."""

    key: Any = models.CharField(max_length=100, unique=True)
    value: Any = models.TextField()
    description: Any = models.TextField(blank=True)
    created_at: Any = models.DateTimeField(auto_now_add=True)
    updated_at: Any = models.DateTimeField(auto_now=True)

    def __str__(self) -> str:
        return f"{self.key}: {self.value[:30]}"
