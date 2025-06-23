# Integration Tests

This directory contains integration tests that test the system as a whole or test interactions between components.

## Test Files

### Authentication Tests
- `test_auth_login.py` - Tests login functionality
- `test_auth_sessions.py` - Tests session management
- `test_auth_users.py` - Tests user management

### System Tests
- `test_django_login.py` - Tests Django integration
- `test_routes.py` - Tests HTTP routes and endpoints

## Running Integration Tests

```bash
# Run all integration tests
python -m pytest tests/integration/

# Run specific test file
python -m pytest tests/integration/test_auth_login.py

# Run with verbose output
python -m pytest tests/integration/ -v
```

## Test Organization

- **Unit Tests**: `tests/` (component-level tests)
- **Integration Tests**: `tests/integration/` (system-level tests)
- **Examples**: `examples/` (demonstration scripts)

## Adding New Integration Tests

1. Create test files with `test_*.py` naming convention
2. Focus on testing component interactions
3. Use appropriate fixtures for setup/teardown
4. Include GPL v3.0 license headers
5. Update this README with descriptions
