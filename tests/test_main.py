"""Tests for the main module."""

from src.main import hello_world


def test_hello_world():
    """Test the hello_world function."""
    result = hello_world()
    assert result == "Hello, World from pyOxide!"
    assert isinstance(result, str)


def test_hello_world_not_empty():
    """Test that hello_world returns a non-empty string."""
    result = hello_world()
    assert len(result) > 0
