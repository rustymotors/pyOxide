# Development Setup Guide

This guide will help you set up a complete development environment for pyOxide with pre-commit hooks to catch issues before CI.

## 🚀 Quick Setup

### 1. Initial Environment Setup

```bash
# Create and activate virtual environment
python -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate

# Install project with development dependencies
pip install -e ".[dev]"
```

### 2. Set Up Pre-commit Hooks

```bash
# Install pre-commit hooks (one-time setup)
pre-commit install

# Optional: Run on all files to test setup
pre-commit run --all-files
```

### 3. VS Code Tasks Setup

Use **Ctrl+Shift+P** → "Tasks: Run Task" → "Setup Pre-commit Hooks" (one-time)

## 🔧 Pre-commit Hooks

The project includes comprehensive pre-commit hooks that automatically run before each commit:

### **Automatic Checks:**
- ✅ **Trailing whitespace removal**
- ✅ **End-of-file fixing**
- ✅ **YAML/JSON/TOML validation**
- ✅ **Large file detection**
- ✅ **Merge conflict detection**
- ✅ **Debug statement detection**

### **Code Quality:**
- ✅ **Black** - Code formatting (88 char line length)
- ✅ **isort** - Import sorting (Black profile)
- ✅ **Flake8** - Linting (max complexity 10)
- ✅ **MyPy** - Static type checking
- ✅ **Bandit** - Security scanning

### **Testing:**
- ✅ **pytest** - Run test suite
- ✅ **Coverage** - Ensure minimum 20% coverage

## 📋 Development Workflow

### **Recommended Git Workflow:**

```bash
# 1. Make your changes
git add .

# 2. Commit (pre-commit hooks run automatically)
git commit -m "Your commit message"

# 3. Push (CI will pass because pre-commit caught issues)
git push
```

### **If Pre-commit Hooks Fail:**

Pre-commit hooks will **automatically fix** most issues:

```bash
# After hooks fix issues, just add and commit again
git add .
git commit -m "Your commit message"
```

### **Manual Code Quality Checks:**

Use VS Code tasks for manual verification:

- **Ctrl+Shift+P** → "Tasks: Run Task"
  - `Format Code` - Black + isort formatting
  - `Lint Code` - Flake8 linting
  - `Type Check` - MyPy type checking
  - `Run Tests` - pytest execution
  - `Run Pre-commit on All Files` - Full pre-commit check

## 🛠️ VS Code Integration

### **Required VS Code Tasks:**

1. **Setup Pre-commit Hooks** (one-time)
2. **Run Pre-commit on All Files** (manual check)
3. **Format Code** (manual formatting)
4. **Lint Code** (manual linting)
5. **Type Check** (manual type checking)
6. **Run Tests** (manual testing)

### **Recommended Extensions:**

- **Python** (Microsoft)
- **Black Formatter** (Microsoft)
- **Pylance** (Microsoft)
- **GitLens** (GitKraken)

## 🔍 Troubleshooting

### **Pre-commit Installation Issues:**

```bash
# Reinstall pre-commit
pip uninstall pre-commit
pip install pre-commit
pre-commit install
```

### **Hook Failures:**

```bash
# Skip hooks for emergency commits (NOT RECOMMENDED)
git commit --no-verify -m "Emergency commit"

# Run specific hook manually
pre-commit run black --all-files
pre-commit run flake8 --all-files
```

### **MyPy Issues:**

```bash
# Install type stubs
pip install django-stubs types-requests

# Clear MyPy cache
mypy --clear-cache
```

## 📊 Coverage Requirements

- **Minimum coverage**: 20% (enforced by pre-commit)
- **Target coverage**: 80%+ for new features
- **Coverage reports**: Generated in `htmlcov/index.html`

## 🚨 Code Quality Standards

### **Automatic Enforcement:**

- **Line length**: 88 characters (Black standard)
- **Import sorting**: Black profile with isort
- **Complexity**: Maximum 10 (Flake8)
- **Type hints**: Required for all functions
- **Security**: Bandit scanning for vulnerabilities

### **Manual Guidelines:**

- Write tests for new functionality
- Use Google-style docstrings
- Follow PEP 8 conventions
- Use Jinja2 templates (no inline HTML)
- Add type annotations

## 🔄 CI/CD Integration

With pre-commit hooks properly set up:

✅ **Code Quality workflow** will pass
✅ **Test Coverage workflow** will pass
✅ **CI/CD Pipeline** will complete successfully
✅ **No more CI failures** due to formatting issues

## 🎯 Benefits

- **Faster development**: Catch issues locally, not in CI
- **Consistent code quality**: Automated formatting and linting
- **Reduced CI time**: No more failed builds due to formatting
- **Better collaboration**: Standardized code style across team
- **Security**: Automatic vulnerability scanning

---

**Next Steps:** Run `pre-commit install` and start developing with confidence! 🚀
