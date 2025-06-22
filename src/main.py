"""Main module for pyOxide application."""


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
                print("  help  - Show this help message")
                print("  hello - Display greeting")
                print("  info  - Show project information")
                print("  quit  - Exit the application")
                print()
            elif user_input == "hello":
                print(hello_world())
            elif user_input == "info":
                print("pyOxide - A modern Python project template")
                print("Version: 0.1.0")
                print("Built with love and modern Python practices! 🐍")
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
