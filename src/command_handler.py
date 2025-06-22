#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
pyOxide Command Handler - Interactive command-line interface.

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

from typing import Callable, Dict

from .server_manager import ServerManager


class CommandHandler:
    """Handles console commands for the pyOxide application."""

    def __init__(self, server_manager: ServerManager) -> None:
        """Initialize the command handler.

        Args:
            server_manager: The server manager instance to use.
        """
        self.server_manager = server_manager
        self.commands: Dict[str, Callable[[], None]] = {
            "help": self.show_help,
            "hello": self.show_hello,
            "info": self.show_info,
            "start": self.start_servers,
            "status": self.show_status,
            "stop": self.stop_servers,
        }

    def show_help(self) -> None:
        """Show available commands."""
        print("\nAvailable commands:")
        print("  help   - Show this help message")
        print("  hello  - Display greeting")
        print("  info   - Show project information")
        print("  start  - Start HTTP and TCP servers")
        print("  status - Check server status")
        print("  stop   - Stop all servers")
        print("  quit   - Exit the application")
        print()

    def show_hello(self) -> None:
        """Display greeting message."""
        print(self._hello_world())

    def show_info(self) -> None:
        """Show project information."""
        print("pyOxide - A modern Python project template")
        print("Version: 0.1.0")
        print("Built with love and modern Python practices! 🐍")

    def start_servers(self) -> None:
        """Start all servers."""
        self.server_manager.start_servers()

    def show_status(self) -> None:
        """Show server status."""
        status = self.server_manager.get_status()
        print("\nServer Status:")
        print(f"  Overall running: {status['running']}")
        http_status = status["http_server"]
        http_port = self.server_manager.http_port
        print(f"  HTTP server (port {http_port}): {http_status}")
        print("  TCP servers:")
        for port, running in status["tcp_servers"].items():
            print(f"    Port {port}: {running}")
        print()

    def stop_servers(self) -> None:
        """Stop all servers."""
        self.server_manager.stop_servers()

    def handle_command(self, command: str) -> bool:
        """Handle a user command.

        Args:
            command: The command string to handle.

        Returns:
            True if the command was handled, False if it should exit.
        """
        command = command.strip().lower()

        if command in ["quit", "exit", "q"]:
            print("Goodbye!")
            return False

        if command == "":
            return True  # Skip empty input

        if command in self.commands:
            self.commands[command]()
        else:
            msg = (
                f"Unknown command: '{command}'. " "Type 'help' for available commands."
            )
            print(msg)

        return True

    def get_hello_message(self) -> str:
        """Get the hello world message.

        Returns:
            str: A greeting message.
        """
        return self._hello_world()

    def _hello_world(self) -> str:
        """Return a greeting message.

        Returns:
            str: A greeting message.
        """
        return "Hello, World from pyOxide!"
