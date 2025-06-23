# pyOxide Quick Reference

## 🚀 Quick Start

```bash
# Start the server
python -m src.main

# Open test page
firefox http://localhost:3000/test &
```

## 📍 Key Endpoints

| Endpoint | Type | Description |
|----------|------|-------------|
| `/test` | **Testing** | **Interactive test page for all routes** |
| `/` | pyOxide | Home page with endpoint listing |
| `/admin/` | Django | Django admin interface |
| `/dashboard/` | Django | Custom admin dashboard |
| `/status` | pyOxide | Server status (JSON) |

## 🔐 Login Credentials

- **Username:** `admin`
- **Password:** `admin123`

## 🧪 Testing Methods

### 🥇 Primary: Interactive Test Page
```bash
firefox http://localhost:3000/test &
```

### 🥈 Text Browser (Quick)
```bash
w3m http://localhost:3000/test
```

### 🥉 Command Line (API)
```bash
curl -s http://localhost:3000/status | jq
```

## 🛠️ Development Commands

```bash
# Run tests
pytest

# Run tests with coverage
pytest --cov=src --cov-report=html --cov-report=xml --cov-report=term-missing

# Generate coverage report only
coverage report

# Open HTML coverage report
open htmlcov/index.html

# Format code
black src/ tests/

# Type check
mypy src/

# Lint code
flake8 src/ tests/

# Run with VS Code tasks
# Ctrl+Shift+P → "Tasks: Run Task"
```

## 🚨 Common Issues

### Django Pages Blank
- ✅ Use Firefox/Chrome
- ❌ Don't use VS Code Simple Browser

### Static Files Not Loading
```bash
curl -I http://localhost:3000/static/admin/css/base.css
# Should return: HTTP/1.0 200 OK
```

### Port Already in Use
```bash
pkill -f "python -m src.main"
```

## 📁 Project Structure

```
src/
├── main.py              # Entry point
├── http_handler.py      # HTTP routes
├── django_integration.py # Django WSGI
└── django_app/          # Django app

templates/               # Jinja2 templates
├── base.html           # Base template
├── test_pages.html     # Test interface
└── ...

tests/                  # Unit tests
```

## 🔄 Request Flow

```
Browser → HTTP Handler → Route Check → Django/pyOxide → Template → Response
```

## 💡 Pro Tips

1. **Always use the test page** at `/test` for manual testing
2. **Templates only** - never mix HTML in Python code
3. **Type hints** required for all functions
4. **Background testing** is prohibited - tests must be synchronous
5. **Firefox/Chrome** for full Django functionality
6. **w3m** for quick text-based testing

## 🔄 CI/CD Pipeline

### GitHub Actions Workflows
- **Code Quality**: Black, isort, flake8, mypy, bandit, safety
- **Test Coverage**: pytest across Python 3.8-3.12, coverage reports
- **Artifacts**: HTML/XML coverage, security reports, test results
- **Integration**: Codecov, PR comments, automated badges

### Status Badges
```bash
python scripts/generate_badges.py
```

### Dependencies
- **Dependabot**: Weekly Python deps, monthly GitHub Actions
- **Security**: Automated vulnerability scanning
- **Quality**: Multi-version testing matrix
