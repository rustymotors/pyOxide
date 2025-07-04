# Development Setup Guide

This guide will help you set up a complete development environment for pyOxide with pre-commit hooks to catch issues before CI.

## � Development Documentation

- **[FEATURES.md](./FEATURES.md)** - Comprehensive feature specification and capabilities registry
- **[CHANGELOG.md](./CHANGELOG.md)** - Version history and change documentation
- **[README.md](./README.md)** - Project overview and user guide
- **[QUICK_REFERENCE.md](./QUICK_REFERENCE.md)** - Command and endpoint reference

## 🔒 GitHub Flow & Branch Protection

**⚠️ IMPORTANT**: As of June 24, 2025, this project uses **GitHub Flow** with branch protection rules:

### **Workflow Requirements:**
- 🚫 **No direct commits to `main`** - All changes must go through Pull Requests
- 🌿 **Feature branches required** - Create a new branch for each feature/fix
- ✅ **CI checks must pass** - All tests, linting, and quality checks must pass
- 👥 **Code review required** - At least one approval needed before merge
- 🔄 **Branch must be up-to-date** - Merge latest `main` before PR approval

### **Development Process:**
1. **Create feature branch**: `git checkout -b feature/motor-city-parser`
2. **Make changes**: Implement features with tests and documentation
3. **Push branch**: `git push origin feature/motor-city-parser`
4. **Open Pull Request**: Use GitHub UI to create PR to `main`
5. **Pass CI checks**: Ensure all workflows pass (tests, linting, type checking)
6. **Code review**: Request review and address feedback
7. **Merge**: Squash and merge after approval

### **Branch Naming Convention:**
- `feature/description` - New features (e.g., `feature/motor-city-parser`)
- `fix/description` - Bug fixes (e.g., `fix/tcp-connection-leak`)
- `docs/description` - Documentation updates (e.g., `docs/api-reference`)
- `refactor/description` - Code refactoring (e.g., `refactor/shard-manager`)

**Why Branch Protection?**
- Prevents accidental direct pushes to main
- Ensures all code passes CI before merging
- Maintains code quality through review process
- Provides audit trail of all changes
- Enables safe rollbacks if needed

## �🚀 Quick Setup

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

### **Feature Development Workflow:**

For any new features or significant changes:

1. **Update FEATURES.md** - Document the new capability in the feature specification
2. **Update CHANGELOG.md** - Add entry to the [Unreleased] section
3. **Implement Changes** - Follow GPL compliance and coding standards
4. **Write Tests** - Add comprehensive test coverage
5. **Update Documentation** - Update README.md if user-facing changes
6. **Run Quality Checks** - Ensure all pre-commit hooks pass

**Required Documentation Updates:**
- ✅ **FEATURES.md** - ALWAYS update for new features, endpoints, models, or CLI commands
- ✅ **CHANGELOG.md** - ALWAYS update for any code changes
- ✅ **README.md** - Update for user-facing changes
- ✅ **GPL Headers** - Include in all new source files

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

## 🚀 Feature Development Workflow

When adding new features to pyOxide, follow this systematic approach:

### **1. Feature Planning & Documentation**
```bash
# 1. Update FEATURES.md with new feature specification
# 2. Document expected endpoints, models, or CLI commands
# 3. Update feature capability tables
# 4. Plan implementation approach
```

### **2. Implementation Standards**
- ✅ **GPL v3.0 Compliance** - Add license headers to all new Python files
- ✅ **Modular Architecture** - Follow existing patterns (separate concerns)
- ✅ **Type Safety** - Comprehensive type hints for all functions/methods
- ✅ **Template System** - Use Jinja2 templates for HTML (no inline HTML in Python)
- ✅ **Django Integration** - Work within embedded Django context

### **3. Testing Requirements**
```bash
# Create comprehensive tests for new functionality
# Naming: test_<feature>.py for new features
# Ensure all existing tests continue to pass
pytest tests/ -v
pytest test_<new_feature>.py -v
```

### **4. Documentation Updates**
- [ ] Update `FEATURES.md` with implementation details
- [ ] Update `CHANGELOG.md` with detailed changes (under `[Unreleased]`)
- [ ] Update `README.md` if user-facing changes
- [ ] Add VS Code tasks if applicable (in `.vscode/tasks.json`)

### **5. Quality Assurance Checklist**
```bash
# Run full quality checks before commit
pre-commit run --all-files
pytest tests/ --cov=src --cov-report=term-missing
mypy src/
```

### **6. Commit Standards**
```bash
# Use descriptive commit messages with feature context
git commit -m "feat: Add <feature> with <key capabilities>

- Implementation details
- Integration notes
- Testing verification
- Documentation updates

All tests passing, GPL compliant, embedded Django working."
```

## 🎯 Benefits

- **Faster development**: Catch issues locally, not in CI
- **Consistent code quality**: Automated formatting and linting
- **Reduced CI time**: No more failed builds due to formatting
- **Better collaboration**: Standardized code style across team
- **Security**: Automatic vulnerability scanning

## 🔧 GitHub CLI Configuration

### **Disable Pager (Recommended):**

```bash
# Option 1: Global configuration (recommended)
gh config set pager ""

# Option 2: Environment variable per session
export GH_PAGER=""

# Option 3: Inline for single commands
GH_PAGER="" gh run list --limit 5
GH_PAGER="" gh run view <run-id>
```

### **Common Commands:**

```bash
# Check CI status without pager
GH_PAGER="" gh run list --limit 5

# View failed run logs
GH_PAGER="" gh run view <run-id> --log-failed

# Check workflow status
GH_PAGER="" gh workflow list

# Alternative: Pipe to cat
gh run list --limit 5 | cat
```

## 🏷️ Version Management & Release Process

### **Version Bumping Process**

1. **Update Version**: Edit `src/_version.py` to new version number
2. **Update CHANGELOG**: Move `[Unreleased]` to `[X.Y.Z] - YYYY-MM-DD` and create new `[Unreleased]` section
3. **Test Updates**: Ensure tests use dynamic `__version__` imports (not hardcoded strings)
4. **Commit Changes**: Stage and commit all version-related changes

### **Git Tag Creation**

**ALWAYS create annotated tags** after version bumps:

```bash
# Create annotated tag with consistent format
git tag -a v0.X.Y -m "Release v0.X.Y - Brief description of major changes"

# Push all tags to GitHub
git push --tags
```

### **Tag Format Requirements**

- **Format**: `"Release v0.X.Y - Description"`
- **Use dashes, not colons**: `"Release v0.3.0 - Authentication System & CI/CD Fixes"` ✅
- **Consistent prefix**: Always start with "Release" ✅
- **Brief description**: Highlight major changes or focus area ✅

### **Version Bump Criteria**

- **Patch (0.0.X)**: Bug fixes, small improvements, CI/CD fixes
- **Minor (0.X.0)**: New features, significant enhancements, new endpoints/commands
- **Major (X.0.0)**: Breaking changes, major architecture changes, license changes

### **Complete Release Checklist**

1. ✅ Update `src/_version.py`
2. ✅ Update `CHANGELOG.md` (move [Unreleased] to [X.Y.Z])
3. ✅ Create new `[Unreleased]` section with "*No changes yet.*"
4. ✅ Verify tests use dynamic `__version__` imports
5. ✅ Commit all changes
6. ✅ Create annotated tag: `git tag -a v0.X.Y -m "Release v0.X.Y - Description"`
7. ✅ Push changes and tags: `git push && git push --tags`
8. ✅ Verify GitHub shows the new release tag

---

**Next Steps:** Run `pre-commit install` and start developing with confidence! 🚀
