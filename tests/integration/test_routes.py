#!/usr/bin/env python3
"""Simple test script to verify all routes are working."""

from time import sleep

import pytest
import requests

BASE_URL = "http://localhost:3000"


def route_test_helper(method: str, path: str, description: str) -> bool:
    """Test a single route."""
    response = None
    try:
        if method == "GET":
            response = requests.get(f"{BASE_URL}{path}", timeout=5)
        elif method == "POST":
            # For POST /api/echo, send some test data
            test_data = {"test": "data", "route": path}
            response = requests.post(f"{BASE_URL}{path}", json=test_data, timeout=5)

        if response:
            print(
                f"✅ {method} {path} - Status: {response.status_code} - {description}"
            )
            return True
        else:
            print(f"❌ {method} {path} - Unsupported method - {description}")
            return False
    except Exception as e:
        print(f"❌ {method} {path} - Error: {e} - {description}")
        return False


@pytest.mark.skip(reason="Requires running server")
def test_routes_with_server() -> None:
    """Test all routes - requires server to be running."""
    main()


def main() -> None:
    """Test all routes."""
    print("🧪 Testing pyOxide HTTP Routes with New Routing System")
    print("=" * 60)

    routes = [
        ("GET", "/", "Home page with Jinja2 template"),
        ("GET", "/status", "Server status JSON"),
        ("GET", "/health", "Health check JSON"),
        ("GET", "/api/info", "API information JSON"),
        ("POST", "/api/echo", "Echo JSON data"),
        ("GET", "/nonexistent", "404 error page with Jinja2 template"),
    ]

    passed = 0
    total = len(routes)

    for method, path, description in routes:
        if route_test_helper(method, path, description):
            passed += 1
        sleep(0.1)  # Small delay between requests

    print("=" * 60)
    print(f"📊 Results: {passed}/{total} routes working correctly")

    if passed == total:
        print("🎉 All routes are working with the new routing system!")
    else:
        print("⚠️  Some routes may need attention.")


if __name__ == "__main__":
    main()
