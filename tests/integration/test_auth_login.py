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

"""Tests for AuthLogin HTTP endpoint."""

import os

import django
import pytest

# Setup Django
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")
django.setup()

from src.django_app.pyoxide_admin.models import AuthSessions, AuthUsers


class TestAuthLoginEndpoint:
    """Test suite for AuthLogin HTTP endpoint."""

    @pytest.fixture
    def test_user(self):
        """Create a test user for authentication tests."""
        # Clean up any existing user first
        try:
            existing_user = AuthUsers.objects.get(username="authtest")
            AuthSessions.objects.filter(customer_id=existing_user).delete()
            existing_user.delete()
        except AuthUsers.DoesNotExist:
            pass

        return AuthUsers.create_user(
            username="authtest", password="authpass123", email="authtest@example.com"
        )

    def test_get_or_create_session_creates_new(self, test_user):
        """Test that get_or_create_session creates new session when none exists."""
        # Ensure no existing sessions
        AuthSessions.objects.filter(customer_id=test_user).delete()

        session = AuthSessions.get_or_create_session(test_user)

        assert session.customer_id == test_user
        assert len(session.ticket) == 40
        assert session.is_active is True
        assert not session.is_expired()

    def test_get_or_create_session_updates_existing(self, test_user):
        """Test that get_or_create_session updates existing session with new ticket."""
        # Create initial session
        session1 = AuthSessions.get_or_create_session(test_user)
        initial_ticket = session1.ticket
        initial_id = session1.id

        # Create second session - should update existing
        session2 = AuthSessions.get_or_create_session(test_user)

        # Should be same session ID but different ticket
        assert session2.id == initial_id
        assert session2.ticket != initial_ticket
        assert len(session2.ticket) == 40

        # Verify only one session exists
        session_count = AuthSessions.objects.filter(customer_id=test_user).count()
        assert session_count == 1

    def test_multiple_session_updates(self, test_user):
        """Test multiple consecutive session updates."""
        tickets = []
        session_id = None

        # Create/update sessions multiple times
        for i in range(3):
            session = AuthSessions.get_or_create_session(test_user)
            tickets.append(session.ticket)

            if session_id is None:
                session_id = session.id
            else:
                # Should always be the same session ID
                assert session.id == session_id

        # All tickets should be different
        assert len(set(tickets)) == 3

        # Should still only have one session
        session_count = AuthSessions.objects.filter(customer_id=test_user).count()
        assert session_count == 1

    def test_session_expiration_reset(self, test_user):
        """Test that get_or_create_session resets expiration time."""
        from datetime import timedelta

        from django.utils import timezone

        # Create session with past expiration
        past_time = timezone.now() - timedelta(days=1)
        session1 = AuthSessions.get_or_create_session(test_user, expires_at=past_time)
        assert session1.is_expired()

        # Update session - should extend expiration
        session2 = AuthSessions.get_or_create_session(test_user)
        assert session2.id == session1.id
        assert not session2.is_expired()

    def test_session_reactivation(self, test_user):
        """Test that inactive sessions are reactivated."""
        # Create session and deactivate it
        session1 = AuthSessions.get_or_create_session(test_user)
        session1.is_active = False
        session1.save()

        # Update session - should reactivate
        session2 = AuthSessions.get_or_create_session(test_user)
        assert session2.id == session1.id
        assert session2.is_active is True
