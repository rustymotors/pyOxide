"""Django WSGI integration for pyOxide HTTP server."""

import io
import os
import sys
from typing import Any, Callable, Dict, List, Tuple
from urllib.parse import unquote

import django
from django.conf import settings
from django.core.wsgi import get_wsgi_application


class DjangoWSGIIntegration:
    """Integrates Django WSGI application with our HTTP server."""

    def __init__(self) -> None:
        """Initialize Django integration."""
        self._setup_django()
        self.application = get_wsgi_application()

    def _setup_django(self) -> None:
        """Set up Django environment."""
        os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")

        if not settings.configured:
            django.setup()

    def handle_request(
        self, method: str, path: str, headers: Dict[str, str], body: bytes = b""
    ) -> Tuple[int, Dict[str, str], bytes]:
        """
        Handle HTTP request through Django WSGI application.

        Args:
            method: HTTP method (GET, POST, etc.)
            path: Request path
            headers: Request headers
            body: Request body

        Returns:
            Tuple of (status_code, response_headers, response_body)
        """
        # Parse query string
        if "?" in path:
            path_info, query_string = path.split("?", 1)
        else:
            path_info, query_string = path, ""

        # Prepare WSGI environ
        environ = {
            "REQUEST_METHOD": method,
            "PATH_INFO": unquote(path_info),
            "QUERY_STRING": query_string,
            "CONTENT_TYPE": headers.get("content-type", ""),
            "CONTENT_LENGTH": str(len(body)),
            "SERVER_NAME": "localhost",
            "SERVER_PORT": "3000",
            "SERVER_PROTOCOL": "HTTP/1.1",
            "wsgi.version": (1, 0),
            "wsgi.url_scheme": "http",
            "wsgi.input": io.BytesIO(body),
            "wsgi.errors": sys.stderr,
            "wsgi.multithread": True,
            "wsgi.multiprocess": False,
            "wsgi.run_once": False,
        }

        # Add HTTP headers to environ
        for header_name, header_value in headers.items():
            # Convert header name to CGI format
            key = "HTTP_" + header_name.upper().replace("-", "_")
            environ[key] = header_value

        # Handle special headers
        if "host" in headers:
            environ["HTTP_HOST"] = headers["host"]
        if "user-agent" in headers:
            environ["HTTP_USER_AGENT"] = headers["user-agent"]

        # Capture response
        response_data: Dict[str, Any] = {"status": "200 OK", "headers": [], "body": []}

        def start_response(
            status: str, response_headers: List[Tuple[str, str]], exc_info: Any = None
        ) -> Callable[[bytes], None]:
            """WSGI start_response callable."""
            response_data["status"] = status
            response_data["headers"] = response_headers

            def write(data: bytes) -> None:
                response_data["body"].append(data)

            return write

        # Call Django WSGI application
        try:
            response_iter = self.application(environ, start_response)

            # Collect response body
            for chunk in response_iter:
                if chunk:
                    response_data["body"].append(chunk)

            # Close iterator if it has close method
            if hasattr(response_iter, "close"):
                response_iter.close()

        except Exception as e:
            # Handle Django errors
            error_body = f"Django Error: {str(e)}".encode()
            return 500, {"Content-Type": "text/plain"}, error_body

        # Parse status code
        status_code = int(response_data["status"].split(" ", 1)[0])

        # Convert headers to dict
        response_headers = dict(response_data["headers"])

        # Combine body chunks
        response_body = b"".join(response_data["body"])

        return status_code, response_headers, response_body

    def is_django_path(self, path: str) -> bool:
        """Check if path should be handled by Django."""
        django_prefixes = [
            "/admin/",
            "/api/django/",
            "/dashboard/",
            "/static/",
        ]

        return any(path.startswith(prefix) for prefix in django_prefixes)
