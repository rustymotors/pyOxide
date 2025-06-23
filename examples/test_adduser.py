#!/usr/bin/env python3
"""Test script for the adduser command."""

import os
import sys

# Add the parent directory to the Python path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import django

# Setup Django
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")
django.setup()

from src.command_handler import CommandHandler
from src.django_app.pyoxide_admin.models import AuthUsers
from src.server_manager import ServerManager


def test_adduser():
    """Test the adduser functionality."""
    # Create handler
    handler = CommandHandler(ServerManager())

    # Test creating a user programmatically
    try:
        # Clean up any existing test user
        AuthUsers.objects.filter(username="testuser").delete()

        # Create user using the model directly
        user = AuthUsers.create_user(
            username="testuser", password="password123", email="test@example.com"
        )

        print("✅ User creation test successful!")
        print(f"   Username: {user.username}")
        print(f"   Customer ID: {user.customer_id}")
        print(f"   Email: {user.email}")
        print(f"   Active: {user.is_active}")
        print(f"   Created: {user.created_at}")

        # Test authentication
        if user.authenticate("password123"):
            print("✅ Password authentication works!")
        else:
            print("❌ Password authentication failed!")

        # Clean up
        user.delete()
        print("✅ Test user cleaned up.")

    except Exception as e:
        print(f"❌ Error: {str(e)}")


if __name__ == "__main__":
    test_adduser()
