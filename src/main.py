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
    print()

    # Main loop - wait for user input
    try:
        while True:
            user_input = input(
                "Enter a command (type 'help' for options or 'quit' to exit): "
            )

            if not command_handler.handle_command(user_input):
                break

    except KeyboardInterrupt:
        print("\n\nReceived keyboard interrupt. Goodbye!")
    except EOFError:
        print("\n\nEnd of input reached. Goodbye!")


if __name__ == "__main__":
    main()
