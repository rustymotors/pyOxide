# Integration Tests

Integration tests verify that different components of pyOxide work together correctly.

## Current Status

✅ **14 passing tests** - Core authentication and model integration
⏭️ **2 skipped tests** - Require running server

## Running Tests

```bash
# Run all integration tests
python -m pytest tests/integration/

# Run specific test
python -m pytest tests/integration/test_auth_login.py -v

# Run without server-dependent tests
python -m pytest tests/integration/ -k "not (django_login or routes)"
```

## Test Coverage

- **Authentication**: User creation, login, password verification
- **Sessions**: Session creation, expiration, refresh
- **Django Integration**: Admin interface, model operations

## Documentation

For detailed testing documentation, see [docs/development/TESTING.md](../../docs/development/TESTING.md).
