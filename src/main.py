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
    print("  ✓ Testing with pytest")
    print("  ✓ VS Code integration")


if __name__ == "__main__":
    main()
