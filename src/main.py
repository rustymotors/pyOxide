"""Main module for pyOxide application."""

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
