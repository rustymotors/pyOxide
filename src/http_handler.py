# -*- coding: utf-8 -*-
"""
pyOxide HTTP Handler - Custom HTTP request handling with Django integration.

Copyright (C) 2025 rustymotors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

import json
import os
import time
from http.server import BaseHTTPRequestHandler
from typing import Any, Callable, Dict, Optional
from urllib.parse import parse_qs, urlparse

from jinja2 import Environment, FileSystemLoader, select_autoescape

from ._version import __version__
from .django_integration import DjangoWSGIIntegration


class PyOxideHTTPHandler(BaseHTTPRequestHandler):
    """Custom HTTP request handler for pyOxide application."""

    def __init__(self, *args: Any, **kwargs: Any) -> None:
        """Initialize the HTTP handler."""
        # Set up Jinja2 environment
        template_dir = os.path.join(
            os.path.dirname(os.path.dirname(__file__)), "templates"
        )
        self.jinja_env = Environment(
            loader=FileSystemLoader(template_dir),
            autoescape=select_autoescape(["html", "xml"]),
        )

        # Initialize Django integration
        self.django_integration: Optional[DjangoWSGIIntegration]
        try:
            self.django_integration = DjangoWSGIIntegration()
        except Exception as e:
            print(f"Warning: Django integration failed: {e}")
            self.django_integration = None

        # Define route mappings
        self.get_routes: Dict[str, Callable[[], None]] = {
            "/": self.route_home,
            "/status": self.route_status,
            "/api/info": self.route_api_info,
            "/health": self.route_health,
            "/AuthLogin": self.route_auth_login,
            "/test": self.route_test_pages,
            "/license": self.route_license,
        }

        self.post_routes: Dict[str, Callable[[], None]] = {
            "/api/echo": self.route_api_echo,
        }

        super().__init__(*args, **kwargs)

    def do_GET(self) -> None:
        """Handle GET requests using route mapping or Django."""
        parsed_path = urlparse(self.path)
        path = parsed_path.path

        # Check if this should be handled by Django
        if self.django_integration and self.django_integration.is_django_path(path):
            self._handle_django_request("GET")
            return

        # Handle with our custom routes
        if path in self.get_routes:
            self.get_routes[path]()
        else:
            self.route_not_found()

    def do_POST(self) -> None:
        """Handle POST requests using route mapping or Django."""
        parsed_path = urlparse(self.path)
        path = parsed_path.path

        # Check if this should be handled by Django
        if self.django_integration and self.django_integration.is_django_path(path):
            self._handle_django_request("POST")
            return

        # Handle with our custom routes
        if path in self.post_routes:
            self.post_routes[path]()
        else:
            self.route_method_not_allowed()

    def do_HEAD(self) -> None:
        """Handle HEAD requests (same as GET but without body)."""
        parsed_path = urlparse(self.path)
        path = parsed_path.path

        # Check if this should be handled by Django
        if self.django_integration and self.django_integration.is_django_path(path):
            self._handle_django_request("HEAD")
            return

        # Handle with our custom routes (HEAD same as GET but no body)
        if path in self.get_routes:
            # For HEAD requests, we send headers but no body
            self.get_routes[path]()
        else:
            self.route_not_found()

    # Route handlers
    def route_home(self) -> None:
        """Handle home page route (/)."""
        template = self.jinja_env.get_template("home.html")

        endpoints = [
            {"method": "GET", "path": "/", "description": "This page"},
            {
                "method": "GET",
                "path": "/test",
                "description": "🧪 **Interactive test page for all routes**",
            },
            {
                "method": "GET",
                "path": "/license",
                "description": "📄 GPL v3.0 license information",
            },
            {
                "method": "GET",
                "path": "/status",
                "description": "Server status information",
            },
            {
                "method": "GET",
                "path": "/health",
                "description": "Health check endpoint",
            },
            {
                "method": "GET",
                "path": "/api/info",
                "description": "JSON API information",
            },
            {
                "method": "GET",
                "path": "/AuthLogin",
                "description": "Authentication login page",
            },
            {
                "method": "GET",
                "path": "/admin/",
                "description": "Django admin interface",
            },
            {
                "method": "GET",
                "path": "/dashboard/",
                "description": "Admin dashboard",
            },
            {
                "method": "GET",
                "path": "/api/django/",
                "description": "Django API information",
            },
            {
                "method": "POST",
                "path": "/api/echo",
                "description": "Echo back JSON data",
            },
        ]

        html_content = template.render(
            status="Running",
            timestamp=time.strftime("%Y-%m-%d %H:%M:%S"),
            version=__version__,
            endpoints=endpoints,
        )

        self._send_html_response(html_content)

    def route_status(self) -> None:
        """Handle status route (/status)."""
        status_data: Dict[str, Any] = {
            "server": "pyOxide HTTP Server",
            "status": "running",
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "version": "0.1.0",
            "endpoints": [
                {"method": "GET", "path": "/", "description": "Main page"},
                {"method": "GET", "path": "/status", "description": "Server status"},
                {"method": "GET", "path": "/health", "description": "Health check"},
                {
                    "method": "GET",
                    "path": "/api/info",
                    "description": "API information",
                },
                {
                    "method": "GET",
                    "path": "/AuthLogin",
                    "description": "Authentication login",
                },
                {
                    "method": "POST",
                    "path": "/api/echo",
                    "description": "Echo JSON data",
                },
            ],
        }
        self._send_json_response(status_data)

    def route_health(self) -> None:
        """Handle health check route (/health)."""
        health_data = {
            "status": "healthy",
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "uptime": "running",
        }
        self._send_json_response(health_data)

    def route_api_info(self) -> None:
        """Handle API info route (/api/info)."""
        api_info: Dict[str, Any] = {
            "name": "pyOxide API",
            "version": "0.1.0",
            "description": "Modern Python project API",
            "features": [
                "HTTP server management",
                "TCP server management",
                "Health monitoring",
                "Status reporting",
            ],
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
        }
        self._send_json_response(api_info)

    def route_api_echo(self) -> None:
        """Handle API echo route (POST /api/echo)."""
        try:
            content_length = int(self.headers.get("Content-Length", 0))
            post_data = self.rfile.read(content_length).decode("utf-8")

            try:
                json_data = json.loads(post_data)
                response_data: Dict[str, Any] = {
                    "echo": json_data,
                    "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
                    "received_at": time.time(),
                }
                self._send_json_response(response_data)
            except json.JSONDecodeError:
                self._send_error_response(400, "Invalid JSON data")
        except Exception as e:
            self._send_error_response(500, f"Server error: {str(e)}")

    def route_auth_login(self) -> None:
        """Handle authentication login route (GET /AuthLogin)."""
        # Parse query parameters from the URL
        parsed_url = urlparse(self.path)
        query_params = parse_qs(parsed_url.query)

        # Extract username and password from query parameters
        username = query_params.get("username", [None])[0]
        password = query_params.get("password", [None])[0]

        # Check if both parameters are provided
        if not username or not password:
            # Return failure response for missing parameters
            response_text = (
                "reasoncode=1\n"
                "reasontext=Missing username or password\n"
                "reasonurl=\n"
            )
            self._send_plain_text_response(response_text)
            return

        try:
            # Import Django models (after Django setup)
            from src.django_app.pyoxide_admin.models import AuthSessions, AuthUsers

            # Try to find user by username
            try:
                user = AuthUsers.objects.get(username=username)
            except AuthUsers.DoesNotExist:
                # User not found
                response_text = (
                    "reasoncode=2\n"
                    "reasontext=Invalid username or password\n"
                    "reasonurl=\n"
                )
                self._send_plain_text_response(response_text)
                return

            # Check if user is active and password is correct
            if not user.is_active or not user.authenticate(password):
                response_text = (
                    "reasoncode=3\n"
                    "reasontext=Invalid username or password\n"
                    "reasonurl=\n"
                )
                self._send_plain_text_response(response_text)
                return

            # Authentication successful - get or update existing session
            session = AuthSessions.get_or_create_session(user)

            # Update last_login timestamp
            from django.utils import timezone

            user.last_login = timezone.now()
            user.save()

            # Return success response
            response_text = f"Valid=TRUE\nTicket={session.ticket}\n"
            self._send_plain_text_response(response_text)

        except Exception as e:
            # Handle any unexpected errors
            response_text = (
                "reasoncode=500\n"
                f"reasontext=Internal server error: {str(e)}\n"
                "reasonurl=\n"
            )
            self._send_plain_text_response(response_text)

    def _send_plain_text_response(self, content: str) -> None:
        """Send a plain text response."""
        self.send_response(200)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.send_header("Content-Length", str(len(content.encode("utf-8"))))
        self.end_headers()
        self.wfile.write(content.encode("utf-8"))

    def route_test_pages(self) -> None:
        """Handle test pages route (GET /test)."""
        template = self.jinja_env.get_template("test_pages.html")
        html_content = template.render()
        self._send_html_response(html_content)

    def route_license(self) -> None:
        """Handle license route (GET /license) - GPL v3.0 compliance."""
        template = self.jinja_env.get_template("license.html")
        html_content = template.render()
        self._send_html_response(html_content)

    def route_not_found(self) -> None:
        """Handle 404 not found route."""
        template = self.jinja_env.get_template("404.html")
        error_html = template.render(requested_path=self.path)
        self._send_html_response(error_html, status_code=404)

    def route_method_not_allowed(self) -> None:
        """Handle 405 method not allowed route."""
        self._send_error_response(405, "Method not allowed")

    def _send_html_response(self, content: str, status_code: int = 200) -> None:
        """Send HTML response."""
        self.send_response(status_code)
        self.send_header("Content-type", "text/html")
        self.send_header("Content-length", str(len(content.encode())))
        self.end_headers()
        self.wfile.write(content.encode())

    def _send_json_response(self, data: Dict[str, Any], status_code: int = 200) -> None:
        """Send JSON response."""
        json_content = json.dumps(data, indent=2)
        self.send_response(status_code)
        self.send_header("Content-type", "application/json")
        self.send_header("Content-length", str(len(json_content.encode())))
        self.end_headers()
        self.wfile.write(json_content.encode())

    def _send_error_response(self, status_code: int, message: str) -> None:
        """Send error response."""
        error_data: Dict[str, Any] = {
            "error": True,
            "status_code": status_code,
            "message": message,
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
        }
        self._send_json_response(error_data, status_code)

    def _handle_django_request(self, method: str) -> None:
        """Handle request through Django WSGI application."""
        if not self.django_integration:
            self._send_error_response(500, "Django integration not available")
            return

        try:
            # Read request body
            content_length = int(self.headers.get("Content-Length", 0))
            body = self.rfile.read(content_length) if content_length > 0 else b""

            # Prepare headers dict
            headers = {}
            for header_name in self.headers:
                headers[header_name.lower()] = self.headers[header_name]

            # Call Django integration
            status_code, response_headers, response_body = (
                self.django_integration.handle_request(method, self.path, headers, body)
            )

            # Send response
            self.send_response(status_code)
            for header_name, header_value in response_headers.items():
                self.send_header(header_name, header_value)

            # Ensure Content-Length is set
            if "Content-Length" not in response_headers:
                self.send_header("Content-Length", str(len(response_body)))

            self.end_headers()
            self.wfile.write(response_body)

        except Exception as e:
            self._send_error_response(500, f"Django error: {str(e)}")

    def log_message(self, format: str, *args: Any) -> None:
        """Override log message to provide custom logging."""
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] HTTP: {format % args}")

    def version_string(self) -> str:
        """Return server version string."""
        return "pyOxide/0.1.0"
