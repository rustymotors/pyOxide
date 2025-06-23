#!/usr/bin/env python3
"""
Demonstration of the adduser command functionality.

This script shows how to use the CLI adduser command and verifies the user creation.
"""

import os
import subprocess
import sys


def demo_adduser_command() -> None:
    """Demonstrate the adduser command functionality."""
    print("=" * 70)
    print("PYOXIDE ADDUSER COMMAND DEMONSTRATION")
    print("=" * 70)
    print()

    print("📋 The adduser command is available in the pyOxide CLI:")
    print("   python -m src.main")
    print("   > adduser")
    print()

    print("🔧 Command features:")
    print("   ✅ Interactive prompts for username, password, and email")
    print("   ✅ Input validation with clear error messages")
    print("   ✅ Duplicate username prevention")
    print("   ✅ Secure password hashing using Django")
    print("   ✅ Optional email field")
    print("   ✅ Immediate feedback with user details")
    print("   ✅ Auto-activation of new users")
    print()

    print("📝 Example usage:")
    print(
        """
    > adduser

    ==================================================
    ADD NEW USER
    ==================================================
    Enter username: johndoe
    Enter password: secretpassword123
    Enter email (optional): john@example.com
    ✅ User 'johndoe' created successfully!
       Customer ID: 5
       Email: john@example.com
       Created: 2025-06-23 12:34:26.796961+00:00
       Active: True
    """
    )

    print("🚫 Error handling examples:")
    print("   • Empty username: ❌ Username cannot be empty.")
    print("   • Duplicate user: ❌ User 'johndoe' already exists.")
    print("   • Empty password: ❌ Password cannot be empty.")
    print()

    print("🔄 Alternative user creation methods:")
    print("   1. Django Admin Interface (/admin/)")
    print("   2. Django Shell (AuthUsers.create_user())")
    print("   3. Direct Model Creation (advanced)")
    print()

    print("🧪 Testing user creation programmatically...")
    print("   Running: python test_adduser.py")
    print()

    # Run the test script
    try:
        result = subprocess.run(
            [sys.executable, "examples/test_adduser.py"],
            capture_output=True,
            text=True,
            cwd=os.path.dirname(os.path.dirname(__file__)),
        )

        if result.returncode == 0:
            print("✅ Test output:")
            print(result.stdout)
        else:
            print("❌ Test failed:")
            print(result.stderr)
    except Exception as e:
        print(f"❌ Error running test: {e}")

    print("=" * 70)
    print("🎉 The adduser command is ready for use!")
    print("   Start pyOxide with: python -m src.main")
    print("   Then run: adduser")
    print("=" * 70)


if __name__ == "__main__":
    demo_adduser_command()
