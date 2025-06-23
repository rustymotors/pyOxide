# Testing Guide

This document provides comprehensive information about the testing setup, organization, and best practices for pyOxide development.

## Test Structure

```
tests/
├── __init__.py                    # Unit tests package
├── test_*.py                      # Component-level unit tests
└── integration/                   # Integration tests
    ├── README.md                  # Integration tests overview
    ├── test_auth_*.py            # Authentication system tests
    ├── test_django_*.py          # Django integration tests
    └── test_routes.py            # HTTP endpoint tests
```

## Current Test Status

- **52 total tests**
  - **38 unit tests** - Component-level testing
  - **14 integration tests** - System-level testing
- **2 skipped tests** - Server-dependent integration tests
- **Coverage**: ~61% overall, 94% for auth models

## Running Tests

### All Tests
```bash
# Run complete test suite
python -m pytest

# Run with coverage report
python -m pytest --cov=src --cov-report=html --cov-report=term-missing
```

### Unit Tests Only
```bash
# Run unit tests (exclude integration)
python -m pytest --ignore=tests/integration

# Run specific unit test file
python -m pytest tests/test_command_handler.py -v
```

### Integration Tests Only
```bash
# Run all integration tests
python -m pytest tests/integration/

# Run without server-dependent tests
python -m pytest tests/integration/ -k "not (django_login or routes)"

# Run specific integration test
python -m pytest tests/integration/test_auth_login.py -v
```

### Server-Dependent Tests
```bash
# Start server in another terminal
python -m src.main

# Run server-dependent tests
python -m pytest tests/integration/test_django_login.py::test_django_admin -s
python -m pytest tests/integration/test_routes.py::test_routes_with_server -s
```

## Test Categories

### Unit Tests (`tests/`)
- **Component isolation**: Test individual modules/classes
- **Fast execution**: No external dependencies
- **High coverage**: Focus on edge cases and error conditions
- **Mocking**: Use mocks for external dependencies

#### Files:
- `test_command_handler.py` - CLI command processing
- `test_http_handler.py` - HTTP request handling
- `test_http_routes.py` - Route mapping and responses
- `test_main.py` - Application entry point
- `test_server_manager.py` - Server lifecycle management

### Integration Tests (`tests/integration/`)
- **System interactions**: Test component integration
- **Django setup**: Use real Django models and database
- **UUID isolation**: Unique identifiers prevent test conflicts
- **Cleanup**: Tests clean up after themselves

#### Files:
- `test_auth_login.py` - Authentication flow testing ✅
- `test_auth_sessions.py` - Session management testing ✅
- `test_auth_users.py` - User CRUD operations testing ✅
- `test_django_login.py` - Django admin interface ⏭️ (requires server)
- `test_routes.py` - HTTP endpoint testing ⏭️ (requires server)

## Writing Tests

### Test Design Principles
1. **Isolation**: Each test should be independent
2. **Repeatability**: Tests should produce consistent results
3. **Clear naming**: Test names should describe what they test
4. **Proper cleanup**: Clean up resources after test completion
5. **Type annotations**: Use proper type hints for better IDE support

### Unit Test Example
```python
"""Test module example."""
import pytest
from src.module import MyClass

class TestMyClass:
    """Test suite for MyClass."""

    def test_method_with_valid_input(self) -> None:
        """Test method with valid input parameters."""
        instance = MyClass()
        result = instance.method("valid_input")
        assert result == "expected_output"

    def test_method_with_invalid_input(self) -> None:
        """Test method error handling with invalid input."""
        instance = MyClass()
        with pytest.raises(ValueError):
            instance.method("invalid_input")
```

### Integration Test Example
```python
"""Integration test example."""
import os
from uuid import uuid4
import django
import pytest

# Setup Django
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "src.django_app.settings")
django.setup()

from src.django_app.pyoxide_admin.models import AuthUsers

class TestAuthUsers:
    """Test suite for AuthUsers integration."""

    @pytest.fixture
    def test_user(self) -> AuthUsers:
        """Create test user with unique identifier."""
        username = f"testuser_{str(uuid4())[:8]}"
        return AuthUsers.create_user(
            username=username,
            password="testpass123",
            email=f"{username}@example.com"
        )

    def test_user_creation(self, test_user: AuthUsers) -> None:
        """Test user creation through Django ORM."""
        assert test_user.username.startswith("testuser_")
        assert test_user.is_active is True
        assert test_user.check_password("testpass123") is True
```

## Test Configuration

### pytest Configuration (`pyproject.toml`)
```toml
[tool.pytest.ini_options]
testpaths = ["tests"]
python_files = ["test_*.py"]
python_classes = ["Test*"]
python_functions = ["test_*"]
addopts = [
    "--strict-markers",
    "--strict-config",
    "--cov=src",
    "--cov-report=term-missing",
    "--cov-report=html:reports/htmlcov",
    "--cov-report=xml:reports/coverage.xml"
]
```

### Coverage Configuration
- **Target**: Maintain >90% coverage for new code
- **Reports**: HTML (reports/htmlcov/), XML (reports/coverage.xml), terminal
- **Exclusions**: Test files, migrations, __init__.py files

## Pre-commit Testing

All tests are run automatically via pre-commit hooks:
```bash
# Install pre-commit hooks
pre-commit install

# Run all hooks manually
pre-commit run --all-files
```

## CI/CD Integration

Tests are run in GitHub Actions on:
- Pull requests to main branch
- Pushes to main branch
- Manual workflow dispatch

## Troubleshooting

### Common Issues

#### Django Setup Errors
```bash
# Ensure Django is properly configured
export DJANGO_SETTINGS_MODULE=src.django_app.settings
python -c "import django; django.setup(); print('Django OK')"
```

#### Database Conflicts
```bash
# Reset test database
rm data/db.sqlite3
python -m src.django_app.manage migrate
```

#### Import Path Issues
```bash
# Verify Python path includes src/
python -c "import sys; print('src' in str(sys.path))"
```

### Performance Tips
- Use `pytest-xdist` for parallel test execution
- Skip slow tests during development with `@pytest.mark.slow`
- Use fixtures for expensive setup operations
- Mock external services and databases when possible

## VS Code Integration

The project includes VS Code tasks for testing:
- **Run Tests**: Full test suite execution
- **Run Tests with Coverage**: Generate coverage reports
- **Coverage Report Only**: View existing coverage data
- **Type Check**: Run mypy type checking
- **Lint Code**: Run flake8 linting

Access via `Ctrl+Shift+P` → "Tasks: Run Task"
