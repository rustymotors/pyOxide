"""Tests for the main module."""

from src.command_handler import CommandHandler
from src.server_manager import ServerManager


def test_hello_world() -> None:
    """Test the hello_world function through command handler."""
    server_manager = ServerManager()
    command_handler = CommandHandler(server_manager)
    result = command_handler.get_hello_message()
    assert result == "Hello, World from pyOxide!"
    assert isinstance(result, str)


def test_hello_world_not_empty() -> None:
    """Test that hello_world returns a non-empty string."""
    server_manager = ServerManager()
    command_handler = CommandHandler(server_manager)
    result = command_handler.get_hello_message()
    assert len(result) > 0
