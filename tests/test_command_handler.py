"""Tests for the CommandHandler class."""

from io import StringIO
from unittest.mock import MagicMock, patch

from src._version import __version__
from src.command_handler import CommandHandler


def test_command_handler_initialization() -> None:
    """Test CommandHandler initialization."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    assert handler.server_manager is server_manager
    assert "help" in handler.commands
    assert "hello" in handler.commands
    assert "info" in handler.commands
    assert "start" in handler.commands
    assert "status" in handler.commands
    assert "stop" in handler.commands


def test_get_hello_message() -> None:
    """Test getting hello message."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    message = handler.get_hello_message()
    assert f"pyOxide v{__version__}" in message
    assert "🚀 Server ready!" in message
    assert "http://localhost:3000/test" in message


@patch("sys.stdout", new_callable=StringIO)
def test_show_help(mock_stdout: StringIO) -> None:
    """Test showing help message."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.show_help()
    output = mock_stdout.getvalue()

    assert "Available commands:" in output
    assert "help    - Show this help message" in output
    assert "hello   - Display greeting" in output
    assert "license - Show license information" in output
    assert "quit    - Exit the application" in output


@patch("sys.stdout", new_callable=StringIO)
def test_show_hello(mock_stdout: StringIO) -> None:
    """Test showing hello message."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.show_hello()
    output = mock_stdout.getvalue()

    assert f"pyOxide v{__version__}" in output
    assert "Copyright (C) 2025 rustymotors" in output
    assert "ABSOLUTELY NO WARRANTY" in output
    assert "Type 'license' for details" in output


@patch("sys.stdout", new_callable=StringIO)
def test_show_info(mock_stdout: StringIO) -> None:
    """Test showing project info."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.show_info()
    output = mock_stdout.getvalue()

    assert "pyOxide - A modern Python HTTP server with Django integration" in output
    assert "Version: 0.2.0" in output
    assert "License: GPL v3.0" in output
    assert "Built with love and modern Python practices! 🐍" in output


@patch("sys.stdout", new_callable=StringIO)
def test_show_license(mock_stdout: StringIO) -> None:
    """Test showing license information."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.show_license()
    output = mock_stdout.getvalue()

    assert "LICENSE INFORMATION" in output
    assert "GNU General Public License" in output
    assert "Copyright (C) 2025 rustymotors" in output
    assert "https://www.gnu.org/licenses/" in output
    assert "https://github.com/rustymotors/pyOxide" in output


def test_start_servers() -> None:
    """Test starting servers command."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.start_servers()
    server_manager.start_servers.assert_called_once()


def test_stop_servers() -> None:
    """Test stopping servers command."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    handler.stop_servers()
    server_manager.stop_servers.assert_called_once()


@patch("sys.stdout", new_callable=StringIO)
def test_show_status(mock_stdout: StringIO) -> None:
    """Test showing server status."""
    server_manager = MagicMock()
    server_manager.http_port = 3000
    server_manager.get_status.return_value = {
        "running": True,
        "http_server": True,
        "tcp_servers": {8226: True, 8228: False},
    }

    handler = CommandHandler(server_manager)
    handler.show_status()
    output = mock_stdout.getvalue()

    assert "Server Status:" in output
    assert "Overall running: True" in output
    assert "HTTP server (port 3000): True" in output
    assert "Port 8226: True" in output
    assert "Port 8228: False" in output


def test_handle_command_quit() -> None:
    """Test handling quit commands."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    # Test different quit variations
    assert handler.handle_command("quit") is False
    assert handler.handle_command("exit") is False
    assert handler.handle_command("q") is False
    assert handler.handle_command("QUIT") is False  # Case insensitive


def test_handle_command_empty() -> None:
    """Test handling empty command."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    result = handler.handle_command("")
    assert result is True
    result = handler.handle_command("   ")
    assert result is True


@patch("sys.stdout", new_callable=StringIO)
def test_handle_command_valid(mock_stdout: StringIO) -> None:
    """Test handling valid commands."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    result = handler.handle_command("hello")
    assert result is True
    output = mock_stdout.getvalue()
    assert f"pyOxide v{__version__}" in output
    assert "Copyright (C) 2025 rustymotors" in output

    handler.handle_command("start")
    server_manager.start_servers.assert_called_once()


@patch("sys.stdout", new_callable=StringIO)
def test_handle_command_unknown(mock_stdout: StringIO) -> None:
    """Test handling unknown command."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    result = handler.handle_command("unknown")
    output = mock_stdout.getvalue()

    assert result is True
    assert "Unknown command: 'unknown'" in output
    assert "Type 'help' for available commands" in output


def test_handle_command_case_insensitive() -> None:
    """Test that command handling is case insensitive."""
    server_manager = MagicMock()
    handler = CommandHandler(server_manager)

    result = handler.handle_command("HELP")
    assert result is True

    result = handler.handle_command("Start")
    assert result is True
    server_manager.start_servers.assert_called_once()
