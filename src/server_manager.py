# -*- coding: utf-8 -*-
"""
pyOxide Server Manager - HTTP and TCP server lifecycle management.

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

import socket
import threading
import time
from http.server import HTTPServer
from typing import Any, Dict, List, Optional, Tuple

from .http_handler import PyOxideHTTPHandler


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
            # nosec B104: Development server needs to bind to all interfaces
            self.http_server = HTTPServer(
                ("0.0.0.0", self.http_port), PyOxideHTTPHandler  # nosec
            )
            print(f"HTTP server starting on port {self.http_port}")
            self.http_server.serve_forever()
        except Exception as e:
            print(f"HTTP server error: {e}")

    def start_tcp_server(self, port: int) -> None:
        """Start a TCP server on the specified port."""
        try:
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            # nosec B104: Development TCP server needs to bind to all interfaces
            server_socket.bind(("0.0.0.0", port))  # nosec
            server_socket.listen(5)
            self.tcp_servers.append(server_socket)
            print(f"TCP server starting on port {port}")

            while self.running:
                try:
                    server_socket.settimeout(1.0)
                    client_socket, address = server_socket.accept()
                    print(f"TCP connection from {address} on port {port}")

                    # Handle client in a separate thread
                    client_thread = threading.Thread(
                        target=self._handle_tcp_client,
                        args=(client_socket, address, port),
                        daemon=True,
                        name=f"TCP-Client-{address[0]}:{address[1]}-{port}",
                    )
                    client_thread.start()

                except socket.timeout:
                    continue
                except Exception as e:
                    if self.running:
                        print(f"TCP server {port} error: {e}")
                    break
        except Exception as e:
            print(f"Failed to start TCP server on port {port}: {e}")

    def _handle_tcp_client(
        self, client_socket: socket.socket, address: Tuple[str, int], port: int
    ) -> None:
        """Handle individual TCP client connections with hex data dumping."""
        try:
            client_socket.settimeout(30.0)  # 30 second timeout for client operations
            print(f"[TCP:{port}] Client {address[0]}:{address[1]} connected")

            while self.running:
                try:
                    # Receive data from client
                    data = client_socket.recv(4096)

                    if not data:
                        print(
                            f"[TCP:{port}] Client {address[0]}:{address[1]} "
                            f"disconnected"
                        )
                        break

                    # Convert data to hex string with leading zeros (no spaces)
                    hex_data = "".join(f"{byte:02X}" for byte in data)

                    # Print hex dump with metadata
                    print(
                        f"[TCP:{port}] RX from {address[0]}:{address[1]} "
                        f"({len(data)} bytes): {hex_data}"
                    )

                    # For debugging, also show as ASCII (printable chars only)
                    ascii_data = "".join(
                        chr(b) if 32 <= b <= 126 else "." for b in data
                    )
                    print(f"[TCP:{port}] ASCII: {ascii_data}")

                    # Echo the data back to client (for testing)
                    client_socket.send(data)
                    print(
                        f"[TCP:{port}] TX to {address[0]}:{address[1]} "
                        f"({len(data)} bytes): {hex_data}"
                    )

                except socket.timeout:
                    # Client timeout - check if server is still running
                    continue
                except Exception as e:
                    print(f"[TCP:{port}] Client {address[0]}:{address[1]} error: {e}")
                    break

        except Exception as e:
            print(f"[TCP:{port}] Client handler error: {e}")
        finally:
            try:
                client_socket.close()
                print(
                    f"[TCP:{port}] Client {address[0]}:{address[1]} connection closed"
                )
            except Exception:
                pass

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
