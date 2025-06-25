"""Tests for the HTTP handler class."""

from unittest.mock import patch

from src._version import __version__
from src.http_handler import PyOxideHTTPHandler


def test_version_string() -> None:
    """Test version string method."""
    # Create a minimal mock handler just to test this method
    with patch.object(PyOxideHTTPHandler, "__init__", return_value=None):
        handler = PyOxideHTTPHandler.__new__(PyOxideHTTPHandler)
        version = handler.version_string()
        assert f"pyOxide/{__version__}" == version


def test_http_handler_class_exists() -> None:
    """Test that the HTTP handler class exists and can be imported."""
    assert PyOxideHTTPHandler is not None
    assert hasattr(PyOxideHTTPHandler, "do_GET")
    assert hasattr(PyOxideHTTPHandler, "do_POST")
    assert hasattr(PyOxideHTTPHandler, "version_string")
