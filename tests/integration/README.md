# Integration Tests

This directory contains integration tests that test the system as a whole or test interactions between components.

## Current Status

✅ **14 passing tests** - Core authentication and model integration tests
⏭️ **2 skipped tests** - Server-dependent tests that require a running pyOxide server

All tests use unique identifiers (UUID) to avoid conflicts between test runs.

## Test Files

### Authentication Tests
- `test_auth_login.py` - Tests login functionality ✅
- `test_auth_sessions.py` - Tests session management ✅
- `test_auth_users.py` - Tests user management ✅

### System Tests
- `test_django_login.py` - Tests Django integration ⏭️ (requires server)
- `test_routes.py` - Tests HTTP routes and endpoints ⏭️ (requires server)

## Running Integration Tests

```bash
# Run all integration tests
python -m pytest tests/integration/

# Run specific test file
python -m pytest tests/integration/test_auth_login.py

# Run with verbose output
python -m pytest tests/integration/ -v

# Run only tests that don't require a server
python -m pytest tests/integration/ -k "not (django_login or routes)"
```

## Test Organization

- **Unit Tests**: `tests/` (component-level tests)
- **Integration Tests**: `tests/integration/` (system-level tests)

## Server-Dependent Tests

Some tests require a running pyOxide server:

```bash
# Start the server in another terminal
python -m src.main

# Then run server-dependent tests
python -m pytest tests/integration/test_django_login.py::test_django_admin -s
python -m pytest tests/integration/test_routes.py::test_routes_with_server -s
```

## Test Design Principles

- **Isolation**: Each test uses unique identifiers to avoid conflicts
- **Reliability**: Tests clean up after themselves and handle missing dependencies gracefully
- **Documentation**: Each test file includes clear descriptions of what it tests
- **Maintainability**: Tests follow project coding standards with type hints and docstrings
- **Examples**: `examples/` (demonstration scripts)

## Adding New Integration Tests

1. Create test files with `test_*.py` naming convention
2. Focus on testing component interactions
3. Use appropriate fixtures for setup/teardown
4. Include GPL v3.0 license headers
5. Update this README with descriptions
