"""Main module for pyOxide application."""

from .server_manager import ServerManager

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
                print("\nServer Status:")
                print(f"  Overall running: {status['running']}")
                http_status = status["http_server"]
                print(f"  HTTP server (port {server_manager.http_port}): {http_status}")
                print("  TCP servers:")
                for port, running in status["tcp_servers"].items():
                    print(f"    Port {port}: {running}")
                print()
            elif user_input == "stop":
                server_manager.stop_servers()
            elif user_input == "":
                continue  # Skip empty input
            else:
                msg = (
                    f"Unknown command: '{user_input}'. "
                    "Type 'help' for available commands."
                )
                print(msg)

    except KeyboardInterrupt:
        print("\n\nReceived keyboard interrupt. Goodbye!")
    except EOFError:
        print("\n\nEnd of input reached. Goodbye!")


if __name__ == "__main__":
    main()
