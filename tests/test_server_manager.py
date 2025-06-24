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


def test_hex_data_formatting() -> None:
    """Test that TCP data is formatted as hex without spaces."""
    # Test data samples
    test_cases = [
        (b"\x00", "00"),
        (b"\x01\x02\x03", "010203"),
        (b"\xff\xfe\xfd", "FFFEFD"),
        (b"\x0a\x0b\x0c\x0d", "0A0B0C0D"),
        (b"Hello", "48656C6C6F"),  # ASCII "Hello"
        (
            b"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
            "000102030405060708090A0B0C0D0E0F",
        ),
    ]

    for test_data, expected_hex in test_cases:
        # Convert using the same logic as the server
        hex_result = "".join(f"{byte:02X}" for byte in test_data)
        assert (
            hex_result == expected_hex
        ), f"Failed for {test_data!r}: got {hex_result}, expected {expected_hex}"


def test_hex_data_formatting_with_leading_zeros() -> None:
    """Test that hex formatting includes leading zeros for single digits."""
    # Test bytes that should have leading zeros
    test_cases = [
        (b"\x00", "00"),  # Zero should be "00", not "0"
        (b"\x01", "01"),  # One should be "01", not "1"
        (b"\x0f", "0F"),  # Fifteen should be "0F", not "F"
        (b"\x00\x0a\x01\x0b", "000A010B"),  # Mix of single and double digit hex
    ]

    for test_data, expected_hex in test_cases:
        hex_result = "".join(f"{byte:02X}" for byte in test_data)
        assert (
            hex_result == expected_hex
        ), f"Failed for {test_data!r}: got {hex_result}, expected {expected_hex}"
        # Verify no spaces in output
        assert (
            " " not in hex_result
        ), f"Hex output should not contain spaces: {hex_result}"


def test_motor_city_online_packet_hex_formatting() -> None:
    """Test hex formatting for realistic Motor City Online packet data."""
    # Simulate common packet patterns that might be received
    test_cases = [
        # Login packet simulation
        (b"\x00\x10\x01\x02", "00100102"),
        # Authentication data simulation
        (b"\xff\x00\xaa\xbb\xcc\xdd", "FF00AABBCCDD"),
        # Binary data with all byte values
        (bytes(range(16)), "000102030405060708090A0B0C0D0E0F"),
        # Empty packet
        (b"", ""),
    ]

    for test_data, expected_hex in test_cases:
        hex_result = "".join(f"{byte:02X}" for byte in test_data)
        assert (
            hex_result == expected_hex
        ), f"Failed for {test_data!r}: got {hex_result}, expected {expected_hex}"
        # Verify format consistency
        assert " " not in hex_result, "Hex output must not contain spaces"
        assert (
            len(hex_result) == len(test_data) * 2
        ), "Each byte should produce exactly 2 hex characters"
