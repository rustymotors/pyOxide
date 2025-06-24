# Changelog

All notable changes to pyOxide will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **Motor City Online Shard Management System**
  - Added `/ShardList/` endpoint (case-sensitive, trailing slash required) for game client shard discovery
  - Created `Shard` dataclass with all Motor City Online required fields (name, description, id, server IPs/ports, status, population, etc.)
  - Implemented `ShardManager` class for dynamic shard management with methods to add, remove, and update shards
  - Added proper Motor City Online INI format output with exact field names and case inconsistencies as required by game client
  - Integrated shard manager into HTTP handler for real-time shard list generation
  - Default shard configuration with rusty-motors.com server endpoints and realistic population data
  - Type-safe shard management with full type hints and proper error handling
  - Removed static template dependency - shard data now generated programmatically
- **TCP Server Hex Dump Functionality**
  - Enhanced TCP server to dump all received packet data as hex strings for Motor City Online protocol debugging
  - Hex output format: uppercase, no spaces between bytes, leading zeros preserved (e.g., "000102FF")
  - Added comprehensive unit tests for hex formatting including edge cases and Motor City Online packet patterns
  - Verified correct formatting for all byte values (0x00-0xFF) with proper length validation
  - Includes ASCII representation alongside hex dump for debugging convenience

### Changed
- **Development Workflow - GitHub Flow Implementation**
  - Implemented branch protection rules for main branch (effective June 24, 2025)
  - All future changes require feature branches and Pull Requests with CI approval
  - No direct commits to main branch allowed - ensures code quality and review process
  - Updated development documentation with GitHub Flow workflow and branch naming conventions

*No other changes yet.*

## [0.3.0] - 2025-06-23

### Added
- **CI/CD Pipeline Fixes and Python 3.8 Compatibility**
  - Added pytest-django dependency for proper Django test database setup
  - Fixed Python 3.8 type annotation compatibility in admin.py (replaced `tuple[str, ...]` with `Any`)
  - Created comprehensive pytest configuration with Django settings integration
  - Added `@pytest.mark.django_db` markers to all integration tests for proper database access
  - Fixed Django app setup in integration tests to use pytest-django fixtures instead of manual setup
  - Enhanced test configuration in pyproject.toml with Django settings module specification
  - All integration tests now pass with proper database table creation and migrations
  - Improved test isolation and transaction handling for database tests
- **Project Structure Reorganization**
  - Created logical directory structure for better maintainability and navigation
  - `examples/` directory for demo scripts and usage examples (moved demo_adduser.py, test_adduser.py)
  - `reports/` directory for generated reports (coverage, security scans, moved htmlcov, coverage.xml, bandit reports)
  - `tests/integration/` directory for integration tests (moved auth, Django, and route tests from root)
  - `data/` directory consolidation for database and data files (moved db.sqlite3)
  - Added comprehensive README.md files for each new directory with usage guidelines
  - Updated all configuration files (pyproject.toml, .vscode/tasks.json, .gitignore) for new paths
  - Enhanced project structure documentation in FEATURES.md with organization principles
  - Fixed example scripts to work from new locations with proper Python path handling
- **Interactive User Creation (`adduser` CLI Command)**
  - New `adduser` command in the interactive CLI for creating authentication users
  - Interactive prompts for username, password, and optional email
  - Input validation with clear error messages for empty fields and duplicate usernames
  - Automatic user activation and immediate success feedback with user details
  - Integration with existing `AuthUsers.create_user()` class method
  - Enhanced CLI help text to include the new command
  - Support for multiple user creation methods (CLI, Django admin, Django shell)
- **Custom Authentication System**
  - `AuthUsers` Django model with secure password handling
  - Customer ID primary key, username, email, and status fields
  - Secure password hashing using Django's `make_password`/`check_password`
  - User creation, authentication, and password management methods
  - Django admin integration for user management
  - Database table: `auth_users` with proper indexing and constraints
- **Session Management System**
  - `AuthSessions` Django model for user session tracking
  - Foreign key relationship to `AuthUsers` via `customer_id`
  - Unique 40-character hex ticket generation using `secrets.token_hex()`
  - Session expiration tracking with `expires_at` field
  - Session refresh capability with configurable duration
  - Automatic session cleanup via CASCADE delete
  - Django admin integration with session management interface
  - **Session update behavior**: If a session exists for a user, update it with a new ticket instead of creating duplicates
- **Comprehensive Feature Specification (FEATURES.md)**
  - Complete documentation of all pyOxide capabilities and features
  - Detailed HTTP endpoint specifications with request/response formats
  - Database model documentation with field descriptions and relationships
  - CLI command reference with usage examples
  - Django admin interface documentation with access credentials
  - Authentication flow documentation with security specifications
  - Development workflow guidelines and quality assurance requirements
  - Template system documentation with Jinja2 architecture details
  - License compliance documentation with GPL v3.0 requirements
  - Technical specifications including dependencies and architecture
  - Feature development checklist for adding new capabilities
  - Integration with changelog requirements for documentation updates

### Fixed
- **CI/CD Pipeline Fixes and Python Cross-Version Compatibility**
  - Fixed Django database errors in CI tests (`django.db.utils.OperationalError: no such table: auth_users`)
  - Resolved Python 3.8 type annotation compatibility issues (`'type' object is not subscriptable`)
  - Fixed Django app registry conflicts (`RuntimeError: populate() isn't reentrant`) in test collection
  - Added proper pytest-django integration with database fixture configuration
  - Fixed integration test setup to use pytest-django fixtures instead of manual Django setup
  - Resolved Django test environment setup conflicts between pytest-django and manual setup
  - All tests now pass across Python 3.8, 3.9, and 3.11 in CI environments
  - Fixed transaction management issues in integration tests with proper database markers
- **CI/CD Pipeline Fixes**
  - Fixed integration tests failing in CI with `ModuleNotFoundError: No module named 'src'`
  - Added missing `requests` dependency to pyproject.toml for integration tests
  - Updated all GitHub Actions workflows to properly set PYTHONPATH environment variable
  - Fixed test execution in CI environments by ensuring Python can find the `src` module
  - Resolved import path issues in ci-cd.yml, test-coverage.yml, and publish-coverage.yml
  - Fixed coverage artifact paths in all workflows to use `reports/htmlcov/` and `reports/coverage.xml`
  - Fixed coverage publishing workflow failures due to path mismatches after project reorganization
  - Removed `reports/` directory from version control (contains generated files like coverage reports)
  - Updated .gitignore to ignore entire `reports/` directory instead of individual files
  - All CI/CD workflows now run successfully with proper module discovery and coverage reporting

### Enhanced
- **Django Models Enhancement**
  - Improved type annotations and code clarity in Django models
  - Added comprehensive docstrings following Google style
  - Enhanced model methods with proper return types
  - Better integration with embedded Django admin interface
- **Authentication Endpoint Enhancement**
  - Modified `/AuthLogin` endpoint to accept GET query parameters (`username`, `password`)
  - Returns specific plain text responses for success/failure scenarios
  - Success response: `Valid=TRUE\nTicket=${ticket}` with session ticket
  - Failure response: `reasoncode=${code}\nreasontext=${text}\nreasonurl=${url}`
  - Integrates with AuthUsers and AuthSessions models for secure authentication

### Changed
- **Model Organization Refactor**
  - Split monolithic `models.py` into focused modules:
    - `models/auth.py` - Authentication models (`AuthUsers`)
    - `models/config.py` - Configuration models (`Configuration`)
    - `models/logging.py` - Logging models (`ServerStatus`, `APIRequest`)
    - `models/__init__.py` - Imports all models for compatibility
  - Maintained backward compatibility through `__init__.py` imports
  - Improved code organization for future model additions
  - All existing imports continue to work without changes
- **Development Workflow Integration**
  - Updated DEVELOPMENT.md to require FEATURES.md updates for all new features
  - Enhanced Copilot instructions to mandate feature specification updates
  - Added FEATURES.md to required documentation updates for any code changes
  - Integrated feature specification into VS Code development workflow
  - Created documentation requirements for endpoints, models, and CLI commands
- **Documentation Organization**
  - Moved all developer documentation to `docs/development/` directory for better organization
  - Created comprehensive documentation index at `docs/README.md` with clear navigation
  - Separated user-facing documentation (FEATURES.md, QUICK_REFERENCE.md) from developer guides
  - Streamlined integration test README to focus on user information only
  - Created dedicated testing documentation at `docs/development/TESTING.md`
  - Updated all cross-references and links for new documentation structure
  - Removed duplicate files and organized remaining demo scripts in examples directory

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
