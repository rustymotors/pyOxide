<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

# Copilot Instructions for pyOxide

This is a Python project using modern development practices. When working on this project:

## Code Style
- Follow PEP 8 guidelines
- Use type hints for all function parameters and return values
- Use Black for code formatting (line length: 88 characters)
- Use isort for import sorting with Black profile
- Write docstrings for all public functions and classes using Google style

## Project Structure
- Source code goes in the `src/` directory
- Tests go in the `tests/` directory
- Use `pyproject.toml` for project configuration
- Follow the existing module structure

## License Compliance (GPL v3.0)
- **This project is licensed under GPL v3.0** - ensure all contributions comply
- **Add GPL license headers** to all new Python source files using this template:
  ```python
  # Copyright (C) 2025 rustymotors
  #
  # This program is free software: you can redistribute it and/or modify
  # it under the terms of the GNU General Public License as published by
  # the Free Software Foundation, either version 3 of the License, or
  # (at your option) any later version.
  #
  # This program is distributed in the hope that it will be useful,
  # but WITHOUT ANY WARRANTY; without even the implied warranty of
  # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  # GNU General Public License for more details.
  #
  # You should have received a copy of the GNU General Public License
  # along with this program.  If not, see <https://www.gnu.org/licenses/>.
  ```
- **Update project metadata** when license-related changes are made
- **Document license changes** in CHANGELOG.md under appropriate sections
- **Ensure compatibility** with GPL v3.0 for any new dependencies

## Development Tools
- Use pytest for testing
- Use mypy for type checking
- Use flake8 for linting
- All development dependencies are specified in pyproject.toml

## Best Practices
- Write tests for new functionality
- Keep functions small and focused
- Use meaningful variable and function names
- Add appropriate error handling
- Document complex logic with comments
- **Include GPL license header** in all new Python source files (see License Compliance section)
- **Update copyright years** when making substantial changes to existing files
- **Maintain GPL compliance** when adding new dependencies or external code
- **ALWAYS update CHANGELOG.md** for any code changes (see Documentation and Changelog section)

## HTML/CSS Templates
- **NEVER mix HTML/CSS directly in Python code**
- Always use Jinja2 templates for HTML responses
- Create template files in the `templates/` directory
- Use template inheritance (extend `base.html`) for consistent styling
- Keep Python route handlers clean and focused on logic, not presentation
- Pass dynamic data to templates via `template.render()` parameters

## GitHub Actions Workflows
- **ALWAYS use the latest versions of official GitHub Actions**
- **NEVER use actions/setup-python@v3 or older** - it is deprecated and causes workflow failures
- **Current required versions (as of June 2025):**
  - `actions/checkout@v4` ✅
  - `actions/setup-python@v5` ✅ (latest, v4 and older are deprecated)
  - `actions/cache@v4` ✅
  - `actions/upload-artifact@v4` ✅
  - `actions/download-artifact@v4` ✅
  - `actions/configure-pages@v4` ✅
  - `actions/upload-pages-artifact@v3` ✅ (latest available)
  - `actions/deploy-pages@v4` ✅
- **Check GitHub Marketplace for latest versions** before using any action
- **Update action versions immediately** if workflows fail due to deprecated versions

## GitHub CLI and Automation
- **ALWAYS use `GH_PAGER=""` environment variable** to disable pager for GitHub CLI commands
- **Use `GH_PAGER="" gh run list --limit 5`** instead of `gh run list --limit 5`
- **Use `GH_PAGER="" gh run view <id>`** to avoid pressing 'q' to quit
- **Use `| cat` for other commands** that might use pagers (git, less, man, etc.)
- **Batch operations when possible**: Combine multiple steps into single requests
- **Use specific, actionable requests**: "Fix all CI issues and commit" vs. step-by-step
- **Prefer automated workflows**: Let tools handle repetitive tasks without manual intervention

## Command Execution Preferences
- **Always set isBackground=false** for terminal commands unless explicitly starting servers
- **Use descriptive explanation parameters** for all terminal commands
- **Chain related commands** with `&&` when logical sequence is important
- **Avoid interactive prompts**: Use command flags to bypass confirmations when safe
- **Auto-fix when possible**: Let tools like pre-commit auto-correct issues before manual review

## Testing
- **NEVER use background processes for testing** (e.g., isBackground=true in run_in_terminal)
- All tests must run synchronously and complete before returning
- Use the existing pytest configuration and test structure
- Tests should be deterministic and not rely on external services or timing

## Pre-commit Hooks and Code Quality
- **ALWAYS set up pre-commit hooks** for new developers: `pre-commit install`
- **Run pre-commit before committing**: Hooks automatically catch formatting, linting, and type issues
- **Never commit without running code quality checks** - use VS Code tasks or pre-commit
- **Pre-commit catches issues locally** instead of failing in CI
- **Required tools**: Black, isort, Flake8, MyPy, pytest, Bandit
- **If pre-commit fails**: Let it auto-fix issues, then `git add .` and commit again

## Manual Testing and Verification
- **Primary method**: Use the integrated test page at `http://localhost:3000/test`
- **Browser testing**: Use Firefox/Chrome for full functionality (`firefox http://localhost:3000/test &`)
- **Text browser**: Use w3m for quick testing (`w3m http://localhost:3000/test`)
- **Command line**: Use curl for API testing (`curl -s http://localhost:3000/status | jq`)
- **Static files**: Verify CSS/JS loading (`curl -I http://localhost:3000/static/admin/css/base.css`)
- **Django admin**: Test with credentials admin/admin123
- **NEVER rely on VS Code Simple Browser** for Django pages (limited functionality)

## Documentation and Changelog
- **ALWAYS update CHANGELOG.md** when making changes to the codebase
- **ALWAYS update FEATURES.md** when adding new features or capabilities
- **Follow Keep a Changelog format** with proper sections (Added, Changed, Fixed, etc.)
- **Document all user-facing changes** including new features, bug fixes, and breaking changes
- **Update feature specification** with new endpoints, models, CLI commands, or capabilities
- **Update changelog entries** in the `[Unreleased]` section during development
- **Include both technical and user-impact descriptions** in changelog entries
- **Group related changes** under clear headings (e.g., "GPL v3.0 Compliance Features")
- **Reference specific files/components** when documenting technical changes
- **Update copyright years** in changelog when making substantial changes

## Version Management and Release Process
- **Version updates**: Update version in `src/_version.py` as single source of truth
- **CHANGELOG management**: Move `[Unreleased]` section to `[X.Y.Z] - YYYY-MM-DD` when releasing
- **Create new `[Unreleased]` section** with "*No changes yet.*" placeholder after version release
- **Git tag creation**: ALWAYS create annotated tags after version bumps using consistent format:
  ```bash
  git tag -a v0.X.Y -m "Release v0.X.Y - Brief description of major changes"
  ```
- **Tag format consistency**: All tags must follow format: `"Release v0.X.Y - Description"`
  - ✅ Good: `"Release v0.3.0 - Authentication System & CI/CD Fixes"`
  - ❌ Bad: `"pyOxide v0.3.0 - Authentication System"` (inconsistent prefix)
  - ❌ Bad: `"Release v0.1.1: Development improvements"` (colon instead of dash)
- **Push tags to remote**: Use `git push --tags` to ensure GitHub shows all release tags
- **Version bump criteria**:
  - **Patch (0.0.X)**: Bug fixes, small improvements, CI/CD fixes
  - **Minor (0.X.0)**: New features, significant enhancements, new endpoints/commands
  - **Major (X.0.0)**: Breaking changes, major architecture changes, license changes
- **Test updates**: Ensure tests use dynamic `__version__` imports, not hardcoded version strings
- **Complete process**: Update version → Update changelog → Create tag → Push all changes and tags
