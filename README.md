# pyOxide

<!-- Status Badges -->
![Python](https://img.shields.io/badge/python-3.8%2B-blue)
![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)
![Type checking: mypy](https://img.shields.io/badge/type%20checking-mypy-blue)
![Testing: pytest](https://img.shields.io/badge/testing-pytest-green)
![Coverage](https://img.shields.io/badge/coverage-21%25-red)
![Security: bandit](https://img.shields.io/badge/security-bandit-yellow)
![License: GPL v3](https://img.shields.io/badge/license-GPL%20v3-blue)

A modern Python HTTP server with embedded Django integration, featuring modular architecture, comprehensive testing, and best practices.

## Features

- **Modern Python Architecture**: Modular design with separate concerns for HTTP handling, server management, and command processing
- **Embedded Django Integration**: Full Django admin interface and API endpoints integrated into the custom HTTP server
- **Jinja2 Templating**: All HTML responses use templates (no inline HTML in Python code)
- **Development Tools**: Black, Flake8, MyPy, isort, pytest integration
- **VS Code Integration**: Tasks, debugging, and extension support
- **Multiple Server Support**: HTTP server with multiple TCP servers
- **Static File Serving**: Django admin CSS/JS files served correctly
- **Comprehensive Testing**: Unit tests and integration testing

## Quick Start

1. **Create and activate a virtual environment:**
```bash
python -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
```

2. **Install the project in development mode:**
```bash
pip install -e ".[dev]"
```

3. **Set up pre-commit hooks (recommended):**
```bash
pre-commit install
```

4. **Run the application:**
```bash
python -m src.main
```

4. **Test the application:**
   - Open your browser to: `http://localhost:3000/test`
   - Or use the terminal: `firefox http://localhost:3000/test &`

## Usage

### Starting the Server

```bash
python -m src.main
```

This starts:
- HTTP server on port 3000
- Multiple TCP servers on various ports
- Embedded Django application
- Command-line interface

### Available Endpoints

#### pyOxide Routes
- `/` - Home page with endpoint listing
- `/status` - Server status (JSON)
- `/health` - Health check (JSON)
- `/api/info` - API information (JSON)
- `/AuthLogin` - Custom authentication page
- `/test` - **Interactive test page for all routes**

#### Django Routes
- `/admin/` - Django admin interface
- `/admin/login/` - Django admin login
- `/dashboard/` - Custom admin dashboard
- `/api/django/` - Django API information
- `/static/` - Static files (CSS, JS, images)

### Testing the Application

#### 🎯 **Primary Method: Interactive Test Page**
```bash
# Start the server
python -m src.main

# Open the test page in your browser
firefox http://localhost:3000/test &
```

The test page (`/test`) provides:
- ✅ Visual status indicators
- 🔗 Working links to all endpoints
- 🔑 Login credentials
- 💡 Alternative testing methods
- 🎨 Beautiful, responsive design

#### 🌐 **Browser Testing**
```bash
# Firefox (recommended)
firefox http://localhost:3000/admin/ &

# Chrome
google-chrome http://localhost:3000/admin/ &

# VS Code Simple Browser
# Navigate to: http://localhost:3000/test
```

#### 📝 **Text Browser Testing**
```bash
# Install w3m (if not already installed)
sudo apt install w3m

# Interactive browsing
w3m http://localhost:3000/test

# Quick content check
w3m -dump http://localhost:3000/admin/login/
```

#### 🔧 **Command Line Testing**
```bash
# Check Django admin login page
curl -s http://localhost:3000/admin/login/ | grep -o '<title>.*</title>'

# Test static files
curl -I http://localhost:3000/static/admin/css/base.css

# Test API endpoints
curl -s http://localhost:3000/api/django/ | jq

# Test server status
curl -s http://localhost:3000/status | jq
```

### Django Admin Access

**Default Credentials:**
- **Username:** `admin`
- **Password:** `admin123`

**Creating Additional Users:**
```bash
# In the pyOxide directory
python -c "
import os, django
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'src.django_app.settings')
django.setup()
from django.contrib.auth.models import User
User.objects.create_superuser('newuser', 'email@example.com', 'password123')
"
```

## Development

### Pre-commit Hooks (Recommended)

**Set up pre-commit hooks to catch issues before CI:**

```bash
# Install pre-commit hooks (one-time setup)
pre-commit install

# Optional: Run on all files to test
pre-commit run --all-files
```

The pre-commit hooks automatically run:
- **Black** code formatting
- **isort** import sorting
- **Flake8** linting
- **MyPy** type checking
- **pytest** test execution
- **Security scanning** with Bandit

**📖 See [DEVELOPMENT.md](DEVELOPMENT.md) for complete setup guide**

### GitHub Actions CI/CD

The project includes comprehensive GitHub Actions workflows:

#### **Code Quality Workflow** (`.github/workflows/code-quality.yml`)
- ✅ **Multi-Python testing**: Python 3.8-3.12
- ✅ **Code formatting**: Black format checking
- ✅ **Import sorting**: isort validation
- ✅ **Linting**: Flake8 code quality checks
- ✅ **Type checking**: MyPy static analysis
- ✅ **Security scanning**: Bandit security analysis
- ✅ **Dependency checks**: Safety vulnerability scanning

#### **Test Coverage Workflow** (`.github/workflows/test-coverage.yml`)
- ✅ **Test execution**: pytest with coverage across Python versions
- ✅ **Coverage reports**: HTML and XML formats
- ✅ **Codecov integration**: Automatic coverage reporting
- ✅ **PR comments**: Coverage reports on pull requests
- ✅ **Artifact storage**: Coverage reports saved for 30 days

#### **Complete CI/CD Pipeline** (`.github/workflows/ci-cd.yml`)
- ✅ **Parallel execution**: Code quality + test matrix
- ✅ **Build artifacts**: Coverage reports, test results, security scans
- ✅ **Coverage badges**: Automated coverage status
- ✅ **Build summaries**: Comprehensive job status reporting
- ✅ **Scheduled runs**: Weekly dependency/security checks

### Running Tests
```bash
# Run all tests
pytest

# Run with coverage (HTML + XML + terminal output)
pytest --cov=src --cov-report=html --cov-report=xml --cov-report=term-missing

# Run specific test file
pytest tests/test_http_handler.py

# Run with verbose output
pytest -v

# Run coverage on existing .coverage data
coverage report

# Generate HTML coverage report only
coverage html

# Generate XML coverage report only
coverage xml
```

### Code Quality

#### **Code Formatting**
```bash
# Format code
black src/ tests/

# Sort imports
isort src/ tests/

# Format and sort (combined)
python -m black src/ tests/ && python -m isort src/ tests/
```

#### **Type Checking**
```bash
# Check types
mypy src/

# Check specific file
mypy src/http_handler.py
```

#### **Linting**
```bash
# Lint code
flake8 src/ tests/

# Lint with specific rules
flake8 --max-line-length=88 src/ tests/
```

### VS Code Tasks

Use the integrated VS Code tasks for development:

- **Ctrl+Shift+P** → "Tasks: Run Task"
  - `Run Python Application` - Start the server
  - `Run Tests` - Execute all tests (no coverage)
  - `Run Tests with Coverage` - Execute tests with HTML/XML coverage reports
  - `Coverage Report Only` - Generate coverage report from existing data
  - `Open Coverage HTML Report` - Run tests with coverage and open HTML report
  - `Format Code` - Black formatting
  - `Type Check` - MyPy type checking
  - `Lint Code` - Flake8 linting

### Debugging

#### **Server Logs**
The server provides detailed logging:
```bash
python -m src.main
# Watch for HTTP request logs: [timestamp] HTTP: "METHOD /path HTTP/1.1" status -
```

#### **Django Debug Mode**
Django debug mode is enabled by default in development:
- Detailed error pages
- Debug toolbar available
- SQL query logging

#### **Common Issues**

**Static Files Not Loading:**
```bash
# Check static file serving
curl -I http://localhost:3000/static/admin/css/base.css
# Should return: HTTP/1.0 200 OK
```

**Django Admin Blank Pages:**
- Use Firefox/Chrome instead of VS Code Simple Browser
- Check browser console for JavaScript errors
- Verify static files are loading (see above)

**Port Already in Use:**
```bash
# Kill existing processes
pkill -f "python -m src.main"

# Check what's using port 3000
lsof -i :3000
```

## Project Structure

```
pyOxide/
├── src/                          # Source code
│   ├── __init__.py
│   ├── main.py                   # Application entry point
│   ├── server_manager.py         # Server lifecycle management
│   ├── command_handler.py        # Command-line interface
│   ├── http_handler.py           # HTTP request handling
│   ├── django_integration.py     # Django WSGI integration
│   └── django_app/               # Embedded Django application
│       ├── settings.py           # Django configuration
│       ├── urls.py               # URL routing
│       ├── views.py              # Django views
│       ├── wsgi.py               # WSGI application
│       └── pyoxide_admin/        # Django app for admin
│           ├── __init__.py
│           ├── admin.py          # Admin interface config
│           ├── apps.py           # App configuration
│           ├── models.py         # Data models
│           └── migrations/       # Database migrations
│               └── 0001_initial.py
├── templates/                    # Jinja2 templates
│   ├── base.html                # Base template
│   ├── home.html                # Home page
│   ├── 404.html                 # Error page
│   ├── auth_login.html          # Custom login
│   └── test_pages.html          # Testing interface
├── tests/                       # Test suite
│   ├── __init__.py
│   ├── test_main.py             # Main module tests
│   ├── test_server_manager.py   # Server tests
│   ├── test_command_handler.py  # Command tests
│   ├── test_http_handler.py     # HTTP handler tests
│   └── test_http_routes.py      # Route integration tests
├── .github/
│   └── copilot-instructions.md  # AI coding guidelines
├── .vscode/
│   └── tasks.json               # VS Code tasks
├── .pre-commit-config.yaml      # Pre-commit hooks configuration
├── .gitignore                   # Git ignore rules
├── pyproject.toml               # Project configuration
├── requirements.txt             # Dependencies
├── CHANGELOG.md                 # Version history and changes
├── DEVELOPMENT.md               # Development setup guide
├── db.sqlite3                   # Django database
└── README.md                    # This file
```

## Architecture

### Core Components

1. **ServerManager** (`server_manager.py`)
   - Manages HTTP and TCP server lifecycles
   - Handles graceful shutdown
   - Thread-safe server state management

2. **PyOxideHTTPHandler** (`http_handler.py`)
   - Custom HTTP request handler
   - Route mapping and dispatching
   - Jinja2 template rendering
   - Django integration routing

3. **DjangoWSGIIntegration** (`django_integration.py`)
   - WSGI application wrapper
   - Request/response translation
   - Static file serving
   - Path routing logic

4. **CommandHandler** (`command_handler.py`)
   - Interactive command-line interface
   - Server control commands
   - Status reporting

### Request Flow

```
Browser Request → HTTP Handler → Route Check → Django/pyOxide Handler → Response
                                     ↓
                              Static Files / Templates
```

1. **HTTP Handler** receives request
2. **Route Check** determines handler (Django vs pyOxide)
3. **Django Integration** handles Django routes via WSGI
4. **pyOxide Routes** use Jinja2 templates
5. **Static Files** served directly by Django
6. **Response** sent back to browser

### Key Design Decisions

- **No inline HTML**: All HTML uses Jinja2 templates
- **Single server**: Django and pyOxide routes on same port (3000)
- **Modular architecture**: Separate concerns for maintainability
- **Type safety**: Full type hints throughout codebase
- **Template inheritance**: Consistent styling via base templates

## Contributing

1. **Code Style**: Follow PEP 8 and use the provided tools
2. **Testing**: Write tests for new functionality
3. **Documentation**: Update README and docstrings
4. **Templates**: Use Jinja2 templates, never inline HTML
5. **Type Hints**: Add type annotations to all functions

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for a detailed history of changes, new features, and improvements.

## License

This project is licensed under the GPL v3 License - see the [LICENSE](LICENSE) file for details.
