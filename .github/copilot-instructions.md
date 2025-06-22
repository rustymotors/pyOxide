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
