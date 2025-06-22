"""HTTP request handling module for pyOxide application."""

import json
import time
from http.server import BaseHTTPRequestHandler
from typing import Any, Dict
from urllib.parse import urlparse


class PyOxideHTTPHandler(BaseHTTPRequestHandler):
    """Custom HTTP request handler for pyOxide application."""

    def __init__(self, *args: Any, **kwargs: Any) -> None:
        """Initialize the HTTP handler."""
        super().__init__(*args, **kwargs)

    def do_GET(self) -> None:
        """Handle GET requests."""
        parsed_path = urlparse(self.path)
        path = parsed_path.path

        if path == "/":
            self._handle_root()
        elif path == "/status":
            self._handle_status()
        elif path == "/api/info":
            self._handle_api_info()
        elif path == "/health":
            self._handle_health()
        else:
            self._handle_not_found()

    def do_POST(self) -> None:
        """Handle POST requests."""
        parsed_path = urlparse(self.path)
        path = parsed_path.path

        if path == "/api/echo":
            self._handle_api_echo()
        else:
            self._handle_method_not_allowed()

    def _handle_root(self) -> None:
        """Handle root path requests."""
        html_content = f"""
        <!DOCTYPE html>
        <html>
        <head>
            <title>pyOxide Server</title>
            <style>
                body {{ font-family: Arial, sans-serif; margin: 40px; }}
                .header {{ color: #2c3e50; }}
                .info {{ background: #ecf0f1; padding: 20px; border-radius: 5px; }}
                .endpoints {{ margin-top: 20px; }}
                .endpoint {{
                    margin: 10px 0; padding: 5px; background: #3498db;
                    color: white; border-radius: 3px;
                }}
            </style>
        </head>
        <body>
            <h1 class="header">🐍 pyOxide HTTP Server</h1>
            <div class="info">
                <p><strong>Welcome to pyOxide!</strong></p>
                <p>A modern Python project template with HTTP and TCP server
                capabilities.</p>
                <p><strong>Server Status:</strong> Running</p>
                <p><strong>Started:</strong> {time.strftime("%Y-%m-%d %H:%M:%S")}</p>
            </div>
            <div class="endpoints">
                <h3>Available Endpoints:</h3>
                <div class="endpoint">GET / - This page</div>
                <div class="endpoint">GET /status - Server status information</div>
                <div class="endpoint">GET /health - Health check endpoint</div>
                <div class="endpoint">GET /api/info - JSON API information</div>
                <div class="endpoint">POST /api/echo - Echo back JSON data</div>
            </div>
        </body>
        </html>
        """

        self._send_html_response(html_content)

    def _handle_status(self) -> None:
        """Handle status requests."""
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
                    "method": "POST",
                    "path": "/api/echo",
                    "description": "Echo JSON data",
                },
            ],
        }
        self._send_json_response(status_data)

    def _handle_health(self) -> None:
        """Handle health check requests."""
        health_data = {
            "status": "healthy",
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "uptime": "running",
        }
        self._send_json_response(health_data)

    def _handle_api_info(self) -> None:
        """Handle API info requests."""
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

    def _handle_api_echo(self) -> None:
        """Handle API echo requests."""
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

    def _handle_not_found(self) -> None:
        """Handle 404 not found responses."""
        error_html = """
        <!DOCTYPE html>
        <html>
        <head>
            <title>404 - Not Found</title>
            <style>
                body {{
                    font-family: Arial, sans-serif; margin: 40px;
                    text-align: center;
                }}
                .error {{ color: #e74c3c; }}
            </style>
        </head>
        <body>
            <h1 class="error">404 - Page Not Found</h1>
            <p>The requested path was not found on this server.</p>
            <p><a href="/">Return to home page</a></p>
        </body>
        </html>
        """
        self._send_html_response(error_html, status_code=404)

    def _handle_method_not_allowed(self) -> None:
        """Handle 405 method not allowed responses."""
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

    def log_message(self, format: str, *args: Any) -> None:
        """Override log message to provide custom logging."""
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        print(f"[{timestamp}] HTTP: {format % args}")

    def version_string(self) -> str:
        """Return server version string."""
        return "pyOxide/0.1.0"
