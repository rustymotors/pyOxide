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

"""Tests for AuthSessions model."""

import os
from datetime import timedelta
from uuid import uuid4

import django
import pytest
from django.utils import timezone

# Setup Django
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")
django.setup()

from typing import TYPE_CHECKING

from src.django_app.pyoxide_admin.models import AuthSessions, AuthUsers

if TYPE_CHECKING:
    pass  # Can add type imports here if needed


class TestAuthSessions:
    """Test suite for AuthSessions model."""

    @pytest.fixture
    def test_user(self) -> AuthUsers:
        """Create a test user with unique username for each test."""
        # Use UUID to ensure unique usernames across test runs
        unique_username = f"testuser_{str(uuid4())[:8]}"
        # Clean up any existing user with this username (shouldn't happen with UUID)
        AuthUsers.objects.filter(username=unique_username).delete()
        return AuthUsers.create_user(
            username=unique_username,
            password="testpass123",
            email=f"{unique_username}@example.com",
        )

    def test_create_session(self, test_user: AuthUsers) -> None:
        """Test creating a new session."""
        session = AuthSessions.create_session(test_user)

        assert session.customer_id == test_user
        assert len(session.ticket) == 40  # 40 hex characters
        assert session.is_active is True
        assert not session.is_expired()

    def test_generate_ticket(self) -> None:
        """Test ticket generation."""
        ticket1 = AuthSessions.generate_ticket()
        ticket2 = AuthSessions.generate_ticket()

        assert len(ticket1) == 40
        assert len(ticket2) == 40
        assert ticket1 != ticket2  # Should be unique
        # Check if it's valid hex
        int(ticket1, 16)  # Should not raise exception
        int(ticket2, 16)

    def test_session_expiration(self, test_user: AuthUsers) -> None:
        """Test session expiration logic."""
        # Create expired session
        past_time = timezone.now() - timedelta(days=1)
        session = AuthSessions.create_session(test_user, expires_at=past_time)

        assert session.is_expired() is True

    def test_refresh_session(self, test_user: AuthUsers) -> None:
        """Test refreshing session expiration."""
        session = AuthSessions.create_session(test_user)
        original_expires = session.expires_at

        session.refresh(days=60)

        assert session.expires_at > original_expires
        assert not session.is_expired()

    def test_session_str_representation(self, test_user: AuthUsers) -> None:
        """Test string representation of session."""
        session = AuthSessions.create_session(test_user)
        str_repr = str(session)

        assert "Session" in str_repr
        assert test_user.username in str_repr
        assert session.ticket[:8] in str_repr

    def test_unique_ticket_constraint(self, test_user: AuthUsers) -> None:
        """Test that tickets are unique."""
        session1 = AuthSessions.create_session(test_user)
        session2 = AuthSessions.create_session(test_user)

        assert session1.ticket != session2.ticket

    def test_foreign_key_relationship(self, test_user: AuthUsers) -> None:
        """Test foreign key relationship with AuthUsers."""
        session = AuthSessions.create_session(test_user)

        # Should be able to access user data through the relationship
        assert session.customer_id.username == test_user.username
        assert session.customer_id.customer_id == test_user.customer_id

    def test_cascade_delete(self, test_user: AuthUsers) -> None:
        """Test that sessions are deleted when user is deleted."""
        session = AuthSessions.create_session(test_user)
        session_ticket = session.ticket  # Use ticket instead of id

        # Delete the user
        test_user.delete()

        # Session should also be deleted due to CASCADE
        with pytest.raises(AuthSessions.DoesNotExist):
            AuthSessions.objects.get(ticket=session_ticket)
