"""Tests for the ServerManager class."""

from typing import Any, Dict
from unittest.mock import MagicMock, patch

from src.server_manager import ServerManager


def test_server_manager_initialization() -> None:
    """Test ServerManager initialization."""
    manager = ServerManager()
    assert manager.running is False
    assert manager.http_server is None
    assert len(manager.tcp_servers) == 0
    assert len(manager.server_threads) == 0
    assert manager.http_port == 3000
    assert manager.tcp_ports == [8226, 8228, 7003, 43300]


def test_start_servers_when_not_running() -> None:
    """Test starting servers when not already running."""
    manager = ServerManager()

    # Mock the threading to avoid actually starting servers
    with patch("src.server_manager.threading.Thread") as mock_thread:
        mock_thread_instance = MagicMock()
        mock_thread.return_value = mock_thread_instance

        result = manager.start_servers()

        assert result is True
        assert manager.running is True
        # Should create 5 threads (1 HTTP + 4 TCP)
        assert mock_thread.call_count == 5
        mock_thread_instance.start.assert_called()


def test_start_servers_when_already_running() -> None:
    """Test starting servers when already running."""
    manager = ServerManager()
    manager.running = True

    result = manager.start_servers()

    assert result is False
    assert manager.running is True


def test_stop_servers_when_running() -> None:
    """Test stopping servers when running."""
    manager = ServerManager()
    manager.running = True

    # Mock HTTP server
    mock_http_server = MagicMock()
    manager.http_server = mock_http_server

    # Mock TCP servers
    mock_tcp_server = MagicMock()
    manager.tcp_servers = [mock_tcp_server]

    # Mock threads
    mock_thread = MagicMock()
    manager.server_threads = [mock_thread]

    result = manager.stop_servers()

    assert result is True
    assert manager.running is False
    mock_http_server.shutdown.assert_called_once()
    mock_tcp_server.close.assert_called_once()
    mock_thread.join.assert_called_once_with(timeout=2.0)
    assert len(manager.tcp_servers) == 0
    assert len(manager.server_threads) == 0


def test_stop_servers_when_not_running() -> None:
    """Test stopping servers when not running."""
    manager = ServerManager()
    manager.running = False

    result = manager.stop_servers()

    assert result is False
    assert manager.running is False


def test_get_status_when_not_running() -> None:
    """Test getting status when servers are not running."""
    manager = ServerManager()
    manager.running = False

    status = manager.get_status()

    expected: Dict[str, Any] = {
        "running": False,
        "http_server": False,
        "tcp_servers": {},
    }
    assert status == expected


@patch("src.server_manager.socket.socket")
def test_get_status_when_running(mock_socket: MagicMock) -> None:
    """Test getting status when servers are running."""
    manager = ServerManager()
    manager.running = True

    # Mock socket behavior for successful connections
    mock_socket_instance = MagicMock()
    mock_socket.return_value = mock_socket_instance
    mock_socket_instance.connect_ex.return_value = 0  # Success

    status = manager.get_status()

    assert status["running"] is True
    assert status["http_server"] is True
    assert len(status["tcp_servers"]) == 4
    for port in manager.tcp_ports:
        assert status["tcp_servers"][port] is True
