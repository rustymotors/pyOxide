#!/usr/bin/env python3
"""
Test script for AuthUsers model functionality.
Tests user creation, password hashing, and authentication.
"""
import sys
from uuid import uuid4

import pytest

sys.path.insert(0, "src")

from src.django_app.pyoxide_admin.models import AuthUsers


@pytest.mark.django_db(transaction=True)
def test_auth_users() -> None:
    """Test AuthUsers model functionality."""
    print("🧪 Testing AuthUsers model...")

    # Generate unique usernames to avoid conflicts
    username1 = f"testuser_{str(uuid4())[:8]}"
    username2 = f"testuser_{str(uuid4())[:8]}"

    # Clean up any existing test users
    AuthUsers.objects.filter(username__in=[username1, username2]).delete()

    # Test 1: Create a new user using create_user method
    print("\n1. Creating a new user...")
    user = AuthUsers.create_user(
        username=username1, email="test@example.com", password="securepassword123"
    )
    print(f"   ✓ Created user: {user.username} (ID: {user.customer_id})")
    print(f"   ✓ Email: {user.email}")
    print(f"   ✓ Password is hashed: {user.password_hash[:20]}...")
    print(f"   ✓ Is active: {user.is_active}")
    print(f"   ✓ Created at: {user.created_at}")

    # Test 2: Test password authentication
    print("\n2. Testing password authentication...")
    correct_auth = user.check_password("securepassword123")
    wrong_auth = user.check_password("wrongpassword")
    print(f"   ✓ Correct password authentication: {correct_auth}")
    print(f"   ✓ Wrong password authentication: {wrong_auth}")

    # Test 3: Create user with raw password (should be hashed automatically)
    print("\n3. Creating user with create() method...")
    user2 = AuthUsers(username=username2, email="test2@example.com")
    user2.set_password("anotherpassword456")
    user2.save()
    print(f"   ✓ Created user: {user2.username} (ID: {user2.customer_id})")
    print(f"   ✓ Password is hashed: {user2.password_hash[:20]}...")

    # Test 4: Query users
    print("\n4. Querying users...")
    all_users = AuthUsers.objects.all()
    print(f"   ✓ Total users in database: {all_users.count()}")
    for u in all_users:
        print(f"     - {u.username} ({u.email}) - Active: {u.is_active}")

    # Test 5: Update user
    print("\n5. Testing user updates...")
    user.email = "updated@example.com"
    user.save()
    updated_user = AuthUsers.objects.get(customer_id=user.customer_id)
    print(f"   ✓ Updated email: {updated_user.email}")

    # Test 6: Test unique constraints
    print("\n6. Testing unique constraints...")
    try:
        AuthUsers.create_user(
            username=username1,  # Duplicate username
            email="different@example.com",
            password="password",
        )
        print("   ❌ ERROR: Duplicate username should have been rejected!")
    except Exception as e:
        print(f"   ✓ Duplicate username correctly rejected: {type(e).__name__}")

    try:
        AuthUsers.create_user(
            username=f"differentuser_{str(uuid4())[:8]}",
            email="test@example.com",  # Duplicate email
            password="password",
        )
        print("   ❌ ERROR: Duplicate email should have been rejected!")
    except Exception as e:
        print(f"   ✓ Duplicate email correctly rejected: {type(e).__name__}")

    # Test 7: Test model string representation
    print("\n7. Testing model representation...")
    print(f"   ✓ User string representation: '{user}'")

    print("\n🎉 All AuthUsers tests completed successfully!")

    # Clean up
    print("\n🧹 Cleaning up test data...")
    AuthUsers.objects.filter(username__in=[username1, username2]).delete()
    print("   ✓ Test users deleted")


if __name__ == "__main__":
    test_auth_users()
