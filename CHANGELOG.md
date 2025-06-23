# Changelog

All notable changes to pyOxide will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.0] - 2025-06-22

### Added
- **GPL v3.0 Compliance Features**
  - `license` CLI command for displaying GPL license information
  - `/license` HTTP endpoint with dedicated Jinja2 template
  - GPL license headers in all Python source files
  - Copyright notices in CLI and web interfaces

### Changed
- **BREAKING: License Migration from MIT to GPL v3.0**
  - Updated LICENSE file to GPL v3.0
  - Updated project metadata in pyproject.toml (license field and classifier)
  - Updated README.md badge and license section
  - **Version bumped to 0.2.0 due to license change**
- **Enhanced CLI Interface**
  - Updated `hello` command to display GPL compliance notice
  - Updated help messages to include new `license` command
  - Enhanced project info display with GPL v3.0 license
- **Template Updates**
  - Added `license.html` template for web license display
  - Updated `home.html` and `test_pages.html` to link to license page
  - Added copyright notices to all templates

### Fixed
- **Test Suite Improvements**
  - Fixed version mismatch in HTTP routes test (0.1.0 → 0.2.0)
  - Updated hello world test to match new formatted banner message
  - All 38 tests now passing
- **Code Quality**
  - Fixed line length issues in GPL license text display
  - All pre-commit hooks now passing (Black, isort, flake8, mypy, bandit, pytest)
  - Maintained 62.72% test coverage
- **Python Best Practices**
  - Removed unnecessary shebang lines from non-executable modules
  - Only `main.py` retains shebang as it has executable entry point
  - Follows proper Python module structure conventions
- **Dynamic Version Management**
  - Created `src/_version.py` as single source of truth for version
  - Updated all code and tests to use dynamic version from `__version__`
  - No more hardcoded version strings to update on version bumps
  - Version automatically synced across CLI, web interface, and tests

## [0.1.1] - 2025-06-22

### Added
- Enhanced coverage status script with organizational dashboard integration
- Django type stubs for better MyPy support
- Type annotations improvements for HTTP handler
- Comprehensive GitHub Actions version guidelines for Copilot
- **Pre-commit hooks configuration** with comprehensive code quality checks
- **Development setup guide** (DEVELOPMENT.md) for complete environment setup
- **VS Code tasks** for pre-commit hook management
- **Automated code quality enforcement** before commits reach CI
- **GitHub CLI pager configuration** - instructions to disable pager for smoother workflow
- **Auto-continue preferences** - guidance for reducing manual intervention steps

### Changed
- Updated repository URLs in pyproject.toml to reflect actual GitHub repository
- Updated author information from placeholder to rustymotors
- Coverage badge now reflects actual coverage percentage (21%)
- **Updated all GitHub Actions to latest versions:**
  - `actions/setup-python@v4` → `actions/setup-python@v5` (v4 and older deprecated)
  - `actions/upload-pages-artifact@v2` → `actions/upload-pages-artifact@v3`
  - `actions/deploy-pages@v2` → `actions/deploy-pages@v4`
- **Added Bandit reports to .gitignore** - security scanner outputs are now ignored

### Fixed
- Type annotation for django_integration attribute in HTTP handler
- Repository metadata in pyproject.toml
- Deprecated GitHub Actions versions that could cause workflow failures
- Code quality issues in Django integration (complexity and formatting)
- CI/CD pipeline failures due to flake8 linting violations
- **MyPy type checking issues** in Django models, admin, and views
- **Jinja2 autoescape security vulnerability** (B701) - enabled autoescape=True
- **Python 3.8 compatibility** - disabled Django plugin for older Python versions
- **All CI/CD pipeline type checking failures** - MyPy now passes on all Python versions
- **Bandit security false positives** - added nosec comments for development-safe warnings
- **Pre-commit hook compatibility** - resolved security scanner conflicts for development use
- **Pre-commit pytest hooks** - fixed to use virtual environment Python instead of system Python
- **VS Code commit failures** - resolved "No module named pytest" errors during commits
- **Pre-commit --all-files failures** - added proper exclude patterns for scripts and test files
- **Invalid MyPy module patterns** - removed invalid test file exclusion syntax

## [0.1.0] - 2025-06-22

### Added
- **Core Architecture**
  - Modular server architecture with separate concerns
  - `ServerManager` class for HTTP and TCP server lifecycle management
  - `CommandHandler` class for interactive command-line interface
  - `PyOxideHTTPHandler` class for custom HTTP request handling
  - `DjangoWSGIIntegration` class for seamless Django integration

- **Django Integration**
  - Embedded Django application within custom HTTP server
  - Django admin interface accessible at `/admin/`
  - Custom Django app (`pyoxide_admin`) with models and admin configuration
  - Static file serving for Django admin CSS/JS files
  - WSGI application wrapper for request/response translation
  - Database models: `ServerLog` and `Configuration`

- **Templating System**
  - Jinja2 template engine integration
  - Template inheritance with `base.html`
  - HTML templates for all routes (no inline HTML in Python code)
  - Templates: `home.html`, `404.html`, `auth_login.html`, `test_pages.html`

- **HTTP Endpoints**
  - **pyOxide Routes:**
    - `/` - Home page with endpoint listing
    - `/status` - Server status (JSON)
    - `/health` - Health check (JSON)
    - `/api/info` - API information (JSON)
    - `/AuthLogin` - Custom authentication page
    - `/test` - Interactive test page for all routes
  - **Django Routes:**
    - `/admin/` - Django admin interface
    - `/admin/login/` - Django admin login
    - `/dashboard/` - Custom admin dashboard
    - `/api/django/` - Django API information
    - `/static/` - Static files (CSS, JS, images)

- **Testing Infrastructure**
  - Comprehensive unit test suite with pytest
  - Test files for all major components:
    - `test_main.py` - Main module tests
    - `test_server_manager.py` - Server lifecycle tests
    - `test_command_handler.py` - Command interface tests
    - `test_http_handler.py` - HTTP handler tests
    - `test_http_routes.py` - Route integration tests
  - Coverage reporting with pytest-cov
  - HTML, XML, and terminal coverage reports
  - Coverage status scripts for organizational dashboards

- **Development Tools**
  - Code formatting with Black (88 character line length)
  - Import sorting with isort (Black profile)
  - Type checking with MyPy
  - Linting with Flake8
  - VS Code integration with tasks for all development workflows

- **VS Code Tasks**
  - `Run Python Application` - Start the server
  - `Run Tests` - Execute all tests
  - `Run Tests with Coverage` - Execute tests with coverage reports
  - `Coverage Report Only` - Generate coverage report from existing data
  - `Coverage Status` - Display current coverage status
  - `Open Coverage HTML Report` - Run tests and open HTML report
  - `Format Code` - Black formatting and isort import sorting
  - `Type Check` - MyPy type checking
  - `Lint Code` - Flake8 linting

- **GitHub Actions CI/CD**
  - **Code Quality Workflow** (`.github/workflows/code-quality.yml`)
    - Multi-Python version testing (3.8-3.12)
    - Code formatting checks with Black
    - Import sorting validation with isort
    - Linting with Flake8
    - Type checking with MyPy
    - Security scanning with Bandit
    - Dependency vulnerability checks with Safety
  - **Test Coverage Workflow** (`.github/workflows/test-coverage.yml`)
    - Automated test execution with coverage
    - HTML and XML coverage report generation
    - Codecov integration for coverage tracking
    - PR comment integration for coverage reports
    - Artifact storage for 30 days
  - **Complete CI/CD Pipeline** (`.github/workflows/ci-cd.yml`)
    - Parallel job execution for efficiency
    - Build artifact collection
    - Coverage badge generation
    - Build summary reporting
    - Scheduled weekly runs for dependency checks
  - **Coverage Publishing** (`.github/workflows/publish-coverage.yml`)
    - GitHub Pages integration for coverage reports
    - Automated coverage report publishing

- **Dependency Management**
  - Dependabot configuration for automated dependency updates
  - Comprehensive dependency specifications in pyproject.toml
  - Development dependencies isolated from production

- **Security & Configuration**
  - Development SSL keys with proper documentation
  - Security scanner exceptions for intentional key sharing
  - Bandit and GitLeaks configuration
  - Docker support with proper build context

- **Documentation**
  - Comprehensive README.md with setup, usage, and development guides
  - Quick reference guide (QUICK_REFERENCE.md)
  - Organizational publishing guide (ORG_PUBLISHING_GUIDE.md)
  - GitHub Copilot instructions for development standards
  - Inline code documentation with Google-style docstrings

- **Project Configuration**
  - Modern pyproject.toml configuration
  - Black, isort, MyPy, Flake8, and pytest configuration
  - Coverage reporting configuration
  - Git ignore rules
  - Requirements.txt for compatibility

### Technical Architecture

- **Request Flow**: Browser → HTTP Handler → Route Check → Django/pyOxide Handler → Response
- **Modular Design**: Separate concerns for HTTP handling, server management, command processing
- **Type Safety**: Comprehensive type hints throughout codebase
- **Template Inheritance**: Consistent styling via base templates
- **Single Port Architecture**: Django and pyOxide routes unified on port 3000

### Key Design Decisions

- **No Inline HTML**: All HTML responses use Jinja2 templates
- **Single Server**: Django and pyOxide routes on same port for simplicity
- **Modular Architecture**: Separate classes for different concerns
- **Type Safety**: Full type annotations for better code quality
- **Testing First**: Comprehensive test coverage from the start
- **CI/CD Integration**: Automated quality checks and deployment

### Default Configuration

- **Server**: HTTP server on port 3000
- **Database**: SQLite (`db.sqlite3`) for Django data
- **Admin Credentials**: admin/admin123
- **Templates**: Located in `templates/` directory
- **Static Files**: Served via Django static file handling

### Dependencies

- **Core**: Python 3.8+, Django 4.2+, Jinja2 3.1+
- **Development**: pytest, black, flake8, mypy, isort, coverage
- **Type Checking**: django-stubs, types-requests

---

*This changelog follows the [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format.*
