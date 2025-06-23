"""Unit tests for HTTP routes without requiring a running server."""

import json
from io import BytesIO
from typing import Any, Dict, Optional
from unittest.mock import Mock, patch

from src._version import __version__
from src.http_handler import PyOxideHTTPHandler


class TestHTTPRoutes:
    """Test HTTP routes without requiring a running server."""

    def setup_method(self) -> None:
        """Set up test fixtures."""
        # Create a mock handler instance
        self.mock_request = Mock()
        self.mock_client_address = ("127.0.0.1", 8080)
        self.mock_server = Mock()

        # Mock the base class initialization
        with patch("src.http_handler.BaseHTTPRequestHandler.__init__"):
            self.handler = PyOxideHTTPHandler(
                self.mock_request, self.mock_client_address, self.mock_server
            )

        # Set up mock attributes
        self.handler.wfile = BytesIO()  # type: ignore
        self.handler.rfile = BytesIO()  # type: ignore
        self.handler.path = "/"

        # Create a mock headers object
        self.mock_headers = Mock()
        self.mock_headers.get = Mock(return_value=None)
        self.handler.headers = self.mock_headers  # type: ignore

        # Mock response tracking
        self.response_status: Optional[int] = None
        self.response_headers: Dict[str, str] = {}
        self.response_sent = False

        def mock_send_response(code: int, message: Optional[str] = None) -> None:
            self.response_status = code

        def mock_send_header(keyword: str, value: str) -> None:
            self.response_headers[keyword] = value

        def mock_end_headers() -> None:
            self.response_sent = True

        self.handler.send_response = mock_send_response  # type: ignore
        self.handler.send_header = mock_send_header  # type: ignore
        self.handler.end_headers = mock_end_headers  # type: ignore

    def get_response_body(self) -> str:
        """Get the response body as string."""
        wfile = self.handler.wfile
        assert isinstance(wfile, BytesIO)
        return wfile.getvalue().decode()

    def get_response_json(self) -> Any:
        """Get the response body as JSON."""
        return json.loads(self.get_response_body())

    def test_route_home(self) -> None:
        """Test the home route (/)."""
        self.handler.route_home()

        assert self.response_status == 200
        assert self.response_headers.get("Content-type") == "text/html"

        response_body = self.get_response_body()
        assert "pyOxide Server - Home" in response_body
        assert "Welcome to pyOxide!" in response_body
        assert "Available Endpoints:" in response_body
        assert "GET /" in response_body
        assert "GET /status" in response_body

    def test_route_health(self) -> None:
        """Test the health route (/health)."""
        self.handler.route_health()

        assert self.response_status == 200
        assert self.response_headers.get("Content-type") == "application/json"

        health_data = self.get_response_json()
        assert health_data["status"] == "healthy"
        assert "timestamp" in health_data
        assert health_data["uptime"] == "running"

    def test_route_api_info(self) -> None:
        """Test the API info route (/api/info)."""
        self.handler.route_api_info()

        assert self.response_status == 200
        assert self.response_headers.get("Content-type") == "application/json"

        api_info = self.get_response_json()
        assert api_info["name"] == "pyOxide API"
        assert api_info["version"] == "0.1.0"
        assert api_info["description"] == "Modern Python project API"
        assert "features" in api_info
        assert len(api_info["features"]) == 4
        assert "timestamp" in api_info

    def test_route_api_echo_valid_json(self) -> None:
        """Test the API echo route with valid JSON."""
        test_data: Dict[str, Any] = {"message": "hello", "test": True}
        json_body = json.dumps(test_data).encode()

        # Set up request data
        self.mock_headers.get.return_value = str(len(json_body))
        rfile = BytesIO(json_body)
        self.handler.rfile = rfile  # type: ignore

        self.handler.route_api_echo()

        assert self.response_status == 200
        assert self.response_headers.get("Content-type") == "application/json"

        response_data = self.get_response_json()
        assert response_data["echo"] == test_data
        assert "timestamp" in response_data
        assert "received_at" in response_data

    def test_route_api_echo_invalid_json(self) -> None:
        """Test the API echo route with invalid JSON."""
        invalid_json = b"invalid json data"

        # Set up request data
        self.mock_headers.get.return_value = str(len(invalid_json))
        rfile = BytesIO(invalid_json)
        self.handler.rfile = rfile  # type: ignore

        self.handler.route_api_echo()

        assert self.response_status == 400
        assert self.response_headers.get("Content-type") == "application/json"

        error_data = self.get_response_json()
        assert error_data["error"] is True
        assert error_data["status_code"] == 400
        assert "Invalid JSON data" in error_data["message"]

    def test_route_not_found(self) -> None:
        """Test the 404 not found route."""
        self.handler.path = "/nonexistent"
        self.handler.route_not_found()

        assert self.response_status == 404
        assert self.response_headers.get("Content-type") == "text/html"

        response_body = self.get_response_body()
        assert "404 - Page Not Found" in response_body
        assert "/nonexistent" in response_body
        assert "Return to Home Page" in response_body

    def test_route_method_not_allowed(self) -> None:
        """Test the 405 method not allowed route."""
        self.handler.route_method_not_allowed()

        assert self.response_status == 405
        assert self.response_headers.get("Content-type") == "application/json"

        error_data = self.get_response_json()
        assert error_data["error"] is True
        assert error_data["status_code"] == 405
        assert "Method not allowed" in error_data["message"]

    def test_jinja_environment_setup(self) -> None:
        """Test that Jinja2 environment is properly set up."""
        assert self.handler.jinja_env is not None
        assert self.handler.jinja_env.loader is not None

        # Test that templates can be loaded
        template = self.handler.jinja_env.get_template("home.html")
        assert template is not None

        template = self.handler.jinja_env.get_template("404.html")
        assert template is not None

    def test_route_mapping_completeness(self) -> None:
        """Test that all routes are properly mapped."""
        # Check GET routes
        expected_get_routes = ["/", "/status", "/api/info", "/health"]
        for route in expected_get_routes:
            assert route in self.handler.get_routes
            assert callable(self.handler.get_routes[route])

        # Check POST routes
        expected_post_routes = ["/api/echo"]
        for route in expected_post_routes:
            assert route in self.handler.post_routes
            assert callable(self.handler.post_routes[route])

    @patch("src.http_handler.time.strftime")
    def test_timestamp_consistency(self, mock_strftime: Mock) -> None:
        """Test that timestamps are consistent across routes."""
        mock_strftime.return_value = "2025-06-22 12:00:00"

        self.handler.route_status()

        status_data = self.get_response_json()
        assert status_data["timestamp"] == "2025-06-22 12:00:00"
        mock_strftime.assert_called_with("%Y-%m-%d %H:%M:%S")

    def test_template_rendering_with_data(self) -> None:
        """Test that templates render with correct data."""
        self.handler.route_home()

        response_body = self.get_response_body()

        # Check that template variables were rendered
        assert "Running" in response_body  # status
        assert __version__ in response_body  # version

        # Check that endpoints were rendered
        assert "GET /" in response_body
        assert "Server status information" in response_body
        assert "Health check endpoint" in response_body

    def test_json_response_format(self) -> None:
        """Test that JSON responses are properly formatted."""
        self.handler.route_status()

        # Should be valid JSON
        response_data = self.get_response_json()

        # Should have proper structure
        assert isinstance(response_data, dict)
        assert isinstance(response_data.get("endpoints"), list)
        endpoints = response_data.get("endpoints", [])
        assert all(isinstance(ep, dict) for ep in endpoints)

        # Each endpoint should have required fields
        for endpoint in endpoints:
            assert "method" in endpoint
            assert "path" in endpoint
            assert "description" in endpoint

    def test_error_response_format(self) -> None:
        """Test that error responses have consistent format."""
        self.handler.route_method_not_allowed()

        error_data = self.get_response_json()

        # Check required error fields
        assert error_data["error"] is True
        assert isinstance(error_data["status_code"], int)
        assert isinstance(error_data["message"], str)
        assert "timestamp" in error_data
