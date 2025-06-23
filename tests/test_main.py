"""Tests for the main module."""

from src._version import __version__
from src.command_handler import CommandHandler
from src.server_manager import ServerManager


def test_hello_world() -> None:
    """Test the hello_world function through command handler."""
    server_manager = ServerManager()
    command_handler = CommandHandler(server_manager)
    result = command_handler.get_hello_message()

    # Check that the message contains the expected elements
    assert f"pyOxide v{__version__}" in result
    assert "A Python HTTP Server with Django Integration" in result
    assert "Server ready!" in result
    assert "http://localhost:3000/test" in result
    assert isinstance(result, str)


def test_hello_world_not_empty() -> None:
    """Test that hello_world returns a non-empty string."""
    server_manager = ServerManager()
    command_handler = CommandHandler(server_manager)
    result = command_handler.get_hello_message()
    assert len(result) > 0
