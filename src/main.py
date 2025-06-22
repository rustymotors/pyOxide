#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
pyOxide - A Python HTTP server with Django integration.

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

from .command_handler import CommandHandler
from .server_manager import ServerManager

# Global instances
server_manager = ServerManager()
command_handler = CommandHandler(server_manager)


def main() -> None:
    """Main entry point of the application."""
    print(command_handler.get_hello_message())
    print("Welcome to your new Python project!")
    print("This is a modern Python project template with:")
    print("  ✓ Type hints")
    print("  ✓ Code formatting with Black")
    print("  ✓ Linting with Flake8")
    print("  ✓ Type checking with MyPy")
    print("  ✓ testing with pytest")
    print("  ✓ VS Code integration")
    print("  ✓ Jinja2 templating")
    print()

    # Auto-start servers
    print("🚀 Starting servers automatically...")
    server_manager.start_servers()

    # Show server status
    command_handler.show_status()

    # Main loop - wait for user input
    try:
        while True:
            user_input = input(
                "Enter a command (type 'help' for options or 'quit' to exit): "
            )

            if not command_handler.handle_command(user_input):
                break

    except KeyboardInterrupt:
        print("\n\nReceived keyboard interrupt. Stopping servers...")
        server_manager.stop_servers()
        print("Goodbye!")
    except EOFError:
        print("\n\nEnd of input reached. Stopping servers...")
        server_manager.stop_servers()
        print("Goodbye!")


if __name__ == "__main__":
    main()
