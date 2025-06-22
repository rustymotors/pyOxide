"""Main module for pyOxide application."""

import socket
import threading
import time
from http.server import HTTPServer, SimpleHTTPRequestHandler
from typing import Any, Dict, List, Optional


class ServerManager:
    """Manages HTTP and TCP servers in separate threads."""

    def __init__(self) -> None:
        """Initialize the server manager."""
        self.http_server: Optional[HTTPServer] = None
        self.tcp_servers: List[socket.socket] = []
        self.server_threads: List[threading.Thread] = []
        self.running = False
        self.http_port = 3000
        self.tcp_ports = [8226, 8228, 7003, 43300]

    def start_http_server(self) -> None:
        """Start HTTP server in a separate thread."""
        try:
            handler = SimpleHTTPRequestHandler
            self.http_server = HTTPServer(("localhost", self.http_port), handler)
            print(f"HTTP server starting on port {self.http_port}")
            self.http_server.serve_forever()
        except Exception as e:
            print(f"HTTP server error: {e}")

    def start_tcp_server(self, port: int) -> None:
        """Start a TCP server on the specified port."""
        try:
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_socket.bind(("localhost", port))
            server_socket.listen(5)
            self.tcp_servers.append(server_socket)
            print(f"TCP server starting on port {port}")

            while self.running:
                try:
                    server_socket.settimeout(1.0)
                    client_socket, address = server_socket.accept()
                    print(f"TCP connection from {address} on port {port}")

                    # Send a welcome message
                    message = f"Welcome to TCP server on port {port}!\n"
                    client_socket.send(message.encode())
                    client_socket.close()
                except socket.timeout:
                    continue
                except Exception as e:
                    if self.running:
                        print(f"TCP server {port} error: {e}")
                    break
        except Exception as e:
            print(f"Failed to start TCP server on port {port}: {e}")

    def start_servers(self) -> bool:
        """Start all servers in separate threads."""
        if self.running:
            print("Servers are already running!")
            return False

        self.running = True
        self.server_threads = []

        # Start HTTP server thread
        http_thread = threading.Thread(
            target=self.start_http_server, daemon=True, name="HTTP-Server"
        )
        http_thread.start()
        self.server_threads.append(http_thread)

        # Start TCP server threads
        for port in self.tcp_ports:
            tcp_thread = threading.Thread(
                target=self.start_tcp_server,
                args=(port,),
                daemon=True,
                name=f"TCP-Server-{port}",
            )
            tcp_thread.start()
            self.server_threads.append(tcp_thread)

        # Give servers a moment to start
        time.sleep(0.5)
        print(f"Started 1 HTTP server and {len(self.tcp_ports)} TCP servers")
        return True

    def stop_servers(self) -> bool:
        """Stop all running servers."""
        if not self.running:
            print("No servers are currently running!")
            return False

        print("Stopping servers...")
        self.running = False

        # Stop HTTP server
        if self.http_server:
            self.http_server.shutdown()
            self.http_server = None

        # Close TCP servers
        for tcp_server in self.tcp_servers:
            try:
                tcp_server.close()
            except Exception as e:
                print(f"Error closing TCP server: {e}")
        self.tcp_servers.clear()

        # Wait for threads to finish (with timeout)
        for thread in self.server_threads:
            thread.join(timeout=2.0)

        self.server_threads.clear()
        print("All servers stopped")
        return True

    def get_status(self) -> Dict[str, Any]:
        """Get the status of all servers."""
        status: Dict[str, Any] = {
            "running": self.running,
            "http_server": False,
            "tcp_servers": {},
        }

        if self.running:
            # Check HTTP server
            try:
                test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                test_socket.settimeout(0.1)
                result = test_socket.connect_ex(("localhost", self.http_port))
                status["http_server"] = result == 0
                test_socket.close()
            except Exception:
                status["http_server"] = False

            # Check TCP servers
            for port in self.tcp_ports:
                try:
                    test_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    test_socket.settimeout(0.1)
                    result = test_socket.connect_ex(("localhost", port))
                    status["tcp_servers"][port] = result == 0
                    test_socket.close()
                except Exception:
                    status["tcp_servers"][port] = False

        return status


# Global server manager instance
server_manager = ServerManager()


def hello_world() -> str:
    """Return a greeting message.

    Returns:
        str: A greeting message.
    """
    return "Hello, World from pyOxide!"


def main() -> None:
    """Main entry point of the application."""
    print(hello_world())
    print("Welcome to your new Python project!")
    print("This is a modern Python project template with:")
    print("  ✓ Type hints")
    print("  ✓ Code formatting with Black")
    print("  ✓ Linting with Flake8")
    print("  ✓ Type checking with MyPy")
    print("  ✓ testing with pytest")
    print("  ✓ VS Code integration")
    print()

    # Main loop - wait for user input
    try:
        while True:
            user_input = (
                input("Enter a command (type 'help' for options or 'quit' to exit): ")
                .strip()
                .lower()
            )

            if user_input in ["quit", "exit", "q"]:
                print("Goodbye!")
                break
            elif user_input == "help":
                print("\nAvailable commands:")
                print("  help   - Show this help message")
                print("  hello  - Display greeting")
                print("  info   - Show project information")
                print("  start  - Start HTTP and TCP servers")
                print("  status - Check server status")
                print("  stop   - Stop all servers")
                print("  quit   - Exit the application")
                print()
            elif user_input == "hello":
                print(hello_world())
            elif user_input == "info":
                print("pyOxide - A modern Python project template")
                print("Version: 0.1.0")
                print("Built with love and modern Python practices! 🐍")
            elif user_input == "start":
                server_manager.start_servers()
            elif user_input == "status":
                status = server_manager.get_status()
                print(f"\nServer Status:")
                print(f"  Overall running: {status['running']}")
                print(f"  HTTP server (port 8000): {status['http_server']}")
                print("  TCP servers:")
                for port, running in status["tcp_servers"].items():
                    print(f"    Port {port}: {running}")
                print()
            elif user_input == "stop":
                server_manager.stop_servers()
            elif user_input == "":
                continue  # Skip empty input
            else:
                print(
                    f"Unknown command: '{user_input}'. Type 'help' for available commands."
                )

    except KeyboardInterrupt:
        print("\n\nReceived keyboard interrupt. Goodbye!")
    except EOFError:
        print("\n\nEnd of input reached. Goodbye!")


if __name__ == "__main__":
    main()
