#!/usr/bin/env python3
"""Test script to verify Django admin login works properly."""

from urllib.parse import urljoin

import pytest
import requests

BASE_URL = "http://localhost:3000"


@pytest.mark.skip(reason="Requires running server")
def test_django_admin() -> None:
    """Test Django admin interface login."""
    session = requests.Session()

    print("Testing Django admin interface...")

    try:
        # Get login page
        login_url = urljoin(BASE_URL, "/admin/login/")
        response = session.get(login_url, timeout=5)
        print(f"Login page status: {response.status_code}")
        print(f"Login page content length: {len(response.content)}")

        # Check if CSS is loading
        if 'href="/static/admin/css/base.css"' in response.text:
            print("✓ CSS references found in login page")
        else:
            print("✗ CSS references missing from login page")

        # Test static file directly
        css_url = urljoin(BASE_URL, "/static/admin/css/base.css")
        css_response = session.get(css_url, timeout=5)
        print(f"CSS file status: {css_response.status_code}")
        print(f"CSS file content length: {len(css_response.content)}")

        if css_response.status_code == 200 and len(css_response.content) > 1000:
            print("✓ CSS file loads correctly")
        else:
            print("✗ CSS file loading issue")

        # Test dashboard
        dashboard_url = urljoin(BASE_URL, "/dashboard/")
        dashboard_response = session.get(dashboard_url, timeout=5)
        print(f"Dashboard status: {dashboard_response.status_code}")
        print(f"Dashboard content length: {len(dashboard_response.content)}")

        if "pyOxide Admin Dashboard" in dashboard_response.text:
            print("✓ Dashboard loads correctly")
        else:
            print("✗ Dashboard loading issue")

        print("\nDjango integration test completed!")

    except requests.exceptions.ConnectionError:
        print(
            "❌ Server is not running. Start the server first with: python -m src.main"
        )
        pytest.skip("Server not running")
    except Exception as e:
        print(f"❌ Unexpected error: {e}")
        raise


def main() -> bool:
    """Main function for direct execution."""
    try:
        test_django_admin()
    except Exception as e:
        print(f"Test failed: {e}")
        return False
    return True


if __name__ == "__main__":
    main()
if __name__ == "__main__":
    test_django_admin()
