"""Django models for pyOxide admin interface."""

from typing import Any

from django.contrib.auth.hashers import check_password, make_password
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


class AuthUsers(models.Model):
    """Custom authentication users with secure password handling."""

    customer_id: Any = models.AutoField(primary_key=True)
    username: Any = models.CharField(max_length=150, unique=True)
    password_hash: Any = models.CharField(max_length=128)  # Store hashed password
    email: Any = models.EmailField(blank=True)
    is_active: Any = models.BooleanField(default=True)
    created_at: Any = models.DateTimeField(auto_now_add=True)
    updated_at: Any = models.DateTimeField(auto_now=True)
    last_login: Any = models.DateTimeField(null=True, blank=True)

    class Meta:
        db_table = "auth_users"
        verbose_name = "Authentication User"
        verbose_name_plural = "Authentication Users"
        ordering = ["username"]

    def __str__(self) -> str:
        return f"Customer {self.customer_id}: {self.username}"

    def set_password(self, raw_password: str) -> None:
        """Set password using Django's secure hashing."""
        self.password_hash = make_password(raw_password)  # type: ignore[attr-defined]

    def check_password(self, raw_password: str) -> bool:
        """Check password against stored hash."""
        return check_password(
            raw_password, self.password_hash
        )  # type: ignore[attr-defined]

    def get_customer_id(self) -> int:
        """Get the customer ID (primary key)."""
        return self.customer_id  # type: ignore[attr-defined]

    @classmethod
    def create_user(cls, username: str, password: str, email: str = "") -> "AuthUsers":
        """Create a new user with hashed password."""
        user = cls(username=username, email=email)
        user.set_password(password)
        user.save()
        return user

    def authenticate(self, password: str) -> bool:
        """Authenticate user with password."""
        if not self.is_active:  # type: ignore[attr-defined]
            return False
        return self.check_password(password)
