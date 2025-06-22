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

## HTML/CSS Templates
- **NEVER mix HTML/CSS directly in Python code**
- Always use Jinja2 templates for HTML responses
- Create template files in the `templates/` directory
- Use template inheritance (extend `base.html`) for consistent styling
- Keep Python route handlers clean and focused on logic, not presentation
- Pass dynamic data to templates via `template.render()` parameters

## Testing
- **NEVER use background processes for testing** (e.g., isBackground=true in run_in_terminal)
- All tests must run synchronously and complete before returning
- Use the existing pytest configuration and test structure
- Tests should be deterministic and not rely on external services or timing

## Manual Testing and Verification
- **Primary method**: Use the integrated test page at `http://localhost:3000/test`
- **Browser testing**: Use Firefox/Chrome for full functionality (`firefox http://localhost:3000/test &`)
- **Text browser**: Use w3m for quick testing (`w3m http://localhost:3000/test`)
- **Command line**: Use curl for API testing (`curl -s http://localhost:3000/status | jq`)
- **Static files**: Verify CSS/JS loading (`curl -I http://localhost:3000/static/admin/css/base.css`)
- **Django admin**: Test with credentials admin/admin123
- **NEVER rely on VS Code Simple Browser** for Django pages (limited functionality)
