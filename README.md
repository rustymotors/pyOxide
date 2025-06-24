# pyOxide

<!-- Status Badges -->
![Python](https://img.shields.io/badge/python-3.8%2B-blue)
![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)
![Type checking: mypy](https://img.shields.io/badge/type%20checking-mypy-blue)
![Testing: pytest](https://img.shields.io/badge/testing-pytest-green)
![Coverage](https://img.shields.io/badge/coverage-61%25-orange)
![Security: bandit](https://img.shields.io/badge/security-bandit-yellow)
![License: GPL v3](https://img.shields.io/badge/license-GPL%20v3-blue)

A modern Python HTTP server with embedded Django integration, featuring authentication systems, comprehensive testing, and professional development practices.

## 🚀 Quick Start

### Prerequisites
- Python 3.8+
- Git

### Installation

1. **Clone and setup:**
```bash
git clone <repository-url>
cd pyOxide
python -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
pip install -e ".[dev]"
```

## 🔒 Development Workflow Notice

**⚠️ Important**: This project uses **GitHub Flow** with branch protection (effective June 24, 2025):
- No direct commits to `main` branch
- All changes require feature branches and Pull Requests
- CI checks must pass before merge
- Code review required

**For contributors**: See [WORKFLOW_NOTICE.md](WORKFLOW_NOTICE.md) and [docs/development/DEVELOPMENT.md](docs/development/DEVELOPMENT.md) for full workflow details.

2. **Run the application:**
```bash
python -m src.main
```

3. **Test the application:**
   - Web interface: `http://localhost:3000/test`
   - Django admin: `http://localhost:3000/admin/` (admin/admin123)

## 🔥 Features

- **🌐 HTTP Server**: Custom multi-threaded server with Django integration
- **🔐 Authentication**: Secure user management with sessions and password hashing
- **📱 Web Interface**: Jinja2 templates with responsive design
- **🎨 Django Admin**: Full admin interface for user and system management
- **🧪 Testing**: Comprehensive test suite (52 tests, 61% coverage)
- **⚡ Development Tools**: Black, MyPy, Flake8, pre-commit hooks
- **📊 Monitoring**: Coverage reports, security scanning, CI/CD ready

## 📁 Project Structure

```
pyOxide/
├── src/                    # Main application code
│   ├── django_app/         # Django integration
│   ├── main.py            # Application entry point
│   └── *.py               # Core modules
├── tests/                  # Unit tests
│   └── integration/        # Integration tests
├── examples/               # Usage examples and demos
├── docs/                   # Documentation
├── templates/              # HTML templates
└── data/                   # Database and configuration
```

## 🛠️ Usage

### Web Interface
Visit `http://localhost:3000/test` for the test page with links to all features.

### Interactive CLI
```bash
python -m src.main
> help              # Show available commands
> adduser           # Create new user interactively
> exit              # Quit application
```

### Django Admin
- URL: `http://localhost:3000/admin/`
- Default credentials: admin/admin123
- Manage users, sessions, and system configuration

## 🧪 Testing

```bash
# Run all tests
python -m pytest

# Run with coverage
python -m pytest --cov=src --cov-report=html

# Integration tests only
python -m pytest tests/integration/
```

## 📚 Documentation

- **[Features](docs/FEATURES.md)** - Detailed feature documentation
- **[Development Guide](docs/development/DEVELOPMENT.md)** - Setup and development workflow
- **[Testing Guide](docs/development/TESTING.md)** - Comprehensive testing documentation
- **[Quick Reference](docs/QUICK_REFERENCE.md)** - Command and API reference
- **[Changelog](CHANGELOG.md)** - Version history and changes

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Install development dependencies: `pip install -e ".[dev]"`
4. Set up pre-commit: `pre-commit install`
5. Make your changes with tests
6. Submit a pull request

See [docs/development/DEVELOPMENT.md](docs/development/DEVELOPMENT.md) for detailed contribution guidelines.

## 📄 License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## 🔧 Development Status

- **Active Development**: Regular updates and improvements
- **Test Coverage**: 61% overall, 94% for authentication models
- **Code Quality**: Enforced via pre-commit hooks and CI/CD
- **Documentation**: Comprehensive guides and API documentation

---

For detailed documentation, visit the [docs/](docs/) directory.
