# Development Tools - pyOxide

## 🎯 Single Source of Truth: pyproject.toml + Hatch

This project uses **Hatch** as the modern Python project manager with **`pyproject.toml`** as the single source of truth for all configuration. This eliminates configuration drift and reduces maintenance overhead.

### Why Hatch?

- **Zero Configuration Drift**: All commands defined in `pyproject.toml`
- **Environment Isolation**: Automatic virtual environment management
- **Modern Python Standards**: Built around PEP 517/518/621
- **Cross-Platform**: Works identically on Linux, macOS, and Windows
- **CI/CD Ready**: Same commands work in GitHub Actions

### Quick Start

```bash
# Install Hatch (one-time setup)
pip install hatch

# All development commands use 'hatch run'
hatch run test                  # Run tests
hatch run test-cov             # Run tests with coverage
hatch run check                # Run all quality checks
hatch run format               # Format code
hatch run lint                 # Lint code
hatch run type-check           # Type checking
hatch run security             # Security scanning
```

### Available Commands

All commands are defined in `[tool.hatch.envs.default.scripts]` in `pyproject.toml`:

#### Testing
- `hatch run test` - Run tests with pytest
- `hatch run test-cov` - Run tests with coverage reports (HTML + XML)
- `hatch run test-verbose` - Run tests with verbose output

#### Code Quality
- `hatch run format` - Format code with Black and isort
- `hatch run format-check` - Check code formatting (CI-friendly)
- `hatch run lint` - Lint with flake8
- `hatch run type-check` - Type checking with MyPy
- `hatch run security` - Security scanning with Bandit

#### Combined
- `hatch run check` - Run all quality checks (format-check + lint + type-check + security)
- `hatch run ci` - Full CI pipeline (check + test-cov)

#### Development
- `hatch run server` - Start the development server
- `hatch run coverage-report` - Generate coverage status report
- `hatch run install-hooks` - Install pre-commit hooks
- `hatch run run-hooks` - Run pre-commit on all files

### Configuration Consolidation

All tool configurations are in `pyproject.toml`:

```toml
[tool.black]          # Code formatting
[tool.isort]          # Import sorting
[tool.mypy]           # Type checking
[tool.flake8]         # Linting
[tool.pytest.ini_options]  # Testing
[tool.coverage.run]   # Coverage settings
[tool.bandit]         # Security scanning
[tool.hatch.envs.default.scripts]  # All commands
```

### CI/CD Integration

GitHub Actions workflows use the same Hatch commands:

```yaml
- name: Install dependencies
  run: |
    python -m pip install --upgrade pip
    pip install hatch

- name: Run code quality checks
  run: hatch run check

- name: Run tests with coverage
  run: hatch run test-verbose --junitxml=pytest-results.xml
```

### Deprecated Files

The following files have been **removed** in favor of the unified `pyproject.toml + hatch` approach:

- ✅ ~~`Makefile`~~ - Replaced by Hatch scripts
- ✅ ~~`justfile`~~ - Replaced by Hatch scripts
- ✅ ~~`Taskfile.yml`~~ - Replaced by Hatch scripts
- ✅ ~~`requirements.txt`~~ - Replaced by `pyproject.toml` dependencies
- ✅ ~~`.flake8`~~ - Replaced by `[tool.flake8]` in `pyproject.toml`
- ✅ ~~`scripts/ci-commands.sh`~~ - Replaced by Hatch scripts
- ✅ ~~`docs/CI_BEST_PRACTICES.md`~~ - Superseded by this document
- ✅ Duplicate workflow files - Consolidated into single `ci-cd.yml`
- ✅ Pre-commit hooks - Updated to use Hatch commands for consistency

### Migration Guide

**Old way (multiple tools):**
```bash
make test
just lint
task format
pip install -r requirements.txt
```

**New way (unified with Hatch):**
```bash
hatch run test
hatch run lint
hatch run format
# Dependencies auto-managed by Hatch
```

### Benefits

1. **Consistency**: Same commands work locally and in CI/CD
2. **Simplicity**: One tool, one config file, one command pattern
3. **Maintainability**: Changes in one place (`pyproject.toml`)
4. **Modern**: Follows latest Python packaging standards
5. **Reliability**: Automatic environment isolation prevents conflicts

### Troubleshooting

**Environment Issues:**
```bash
# Reset Hatch environment
hatch env prune
hatch run test  # Recreates environment automatically
```

**Dependency Updates:**
```bash
# Update dependencies in pyproject.toml, then:
hatch env prune  # Clear cached environment
hatch run test   # Reinstalls with new dependencies
```

**Command Not Found:**
```bash
# Install Hatch globally
pip install --user hatch
# Or in project venv
pip install hatch
```

### Development Workflow

1. **Setup**: `pip install hatch` (one-time)
2. **Code**: Make your changes
3. **Check**: `hatch run check` (format, lint, type-check, security)
4. **Test**: `hatch run test-cov`
5. **Commit**: Pre-commit hooks run automatically (using Hatch commands)
6. **Push**: CI/CD runs same Hatch commands

This unified approach eliminates tool confusion and ensures everyone uses the same commands with the same results.

### File Locations

All generated files follow consistent patterns:

- **Coverage Reports**: `reports/coverage.xml`, `reports/htmlcov/`
- **Test Results**: `pytest-results.xml` (root)
- **Security Reports**: `bandit-report.json` (root)
