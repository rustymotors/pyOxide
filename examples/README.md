# Examples

This directory contains example scripts and demonstration code for pyOxide.

## Files

### `demo_adduser.py`
- Comprehensive demonstration of the `adduser` CLI command
- Shows features, usage examples, and error handling
- Includes live testing of user creation functionality

### `test_adduser.py`
- Programmatic test script for user creation
- Demonstrates `AuthUsers.create_user()` usage
- Validates authentication and cleanup

## Usage

Run any script directly:
```bash
python examples/demo_adduser.py
python examples/test_adduser.py
```

## Adding New Examples

When adding new example scripts:
1. Follow the existing naming convention (`demo_*.py` or `example_*.py`)
2. Include comprehensive docstrings and comments
3. Add GPL v3.0 license headers
4. Update this README with descriptions
5. Test thoroughly before committing
