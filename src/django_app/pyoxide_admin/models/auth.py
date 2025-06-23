# Copyright (C) 2025 rustymotors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""Authentication models for pyOxide."""

import secrets
from typing import Any

from django.contrib.auth.hashers import check_password, make_password
from django.db import models


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


class AuthSessions(models.Model):
    """User session tracking with unique tickets."""

    customer_id: Any = models.ForeignKey(
        AuthUsers,
        on_delete=models.CASCADE,
        db_column="customer_id",
        to_field="customer_id",
    )
    ticket: Any = models.CharField(max_length=40, unique=True)
    created_at: Any = models.DateTimeField(auto_now_add=True)
    updated_at: Any = models.DateTimeField(auto_now=True)
    expires_at: Any = models.DateTimeField()
    is_active: Any = models.BooleanField(default=True)

    class Meta:
        db_table = "auth_sessions"
        verbose_name = "Authentication Session"
        verbose_name_plural = "Authentication Sessions"
        ordering = ["-created_at"]

    def __str__(self) -> str:
        username = getattr(self.customer_id, "username", "Unknown")
        return f"Session {self.ticket[:8]}... for {username}"

    def save(self, *args: Any, **kwargs: Any) -> None:
        """Generate unique ticket on creation."""
        if not self.ticket:
            self.ticket = self.generate_ticket()
        super().save(*args, **kwargs)

    @staticmethod
    def generate_ticket() -> str:
        """Generate a unique 40-character hex ticket."""
        return secrets.token_hex(20)  # 20 bytes = 40 hex characters

    @classmethod
    def create_session(cls, user: AuthUsers, expires_at: Any = None) -> "AuthSessions":
        """Create a new session for a user."""
        from datetime import timedelta

        from django.utils import timezone

        if expires_at is None:
            expires_at = timezone.now() + timedelta(days=30)  # 30-day default

        session = cls(customer_id=user, expires_at=expires_at)
        session.save()
        return session

    @classmethod
    def get_or_create_session(
        cls, user: AuthUsers, expires_at: Any = None
    ) -> "AuthSessions":
        """Get existing session for user or create/update one with new ticket."""
        from datetime import timedelta

        from django.utils import timezone

        if expires_at is None:
            expires_at = timezone.now() + timedelta(days=30)  # 30-day default

        try:
            # Try to get existing session for this customer
            session = cls.objects.get(customer_id=user)
            # Update existing session with new ticket and expiration
            session.ticket = cls.generate_ticket()
            session.expires_at = expires_at
            session.is_active = True
            session.save()
            return session
        except cls.DoesNotExist:
            # No existing session, create new one
            session = cls(customer_id=user, expires_at=expires_at)
            session.save()
            return session

    def is_expired(self) -> bool:
        """Check if session is expired."""
        from django.utils import timezone

        return timezone.now() > self.expires_at  # type: ignore[attr-defined]

    def refresh(self, days: int = 30) -> None:
        """Refresh session expiration."""
        from datetime import timedelta

        from django.utils import timezone

        self.expires_at = timezone.now() + timedelta(days=days)
        self.updated_at = timezone.now()
        self.save()
