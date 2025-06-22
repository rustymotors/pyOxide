# pyOxide

A modern Python project template with best practices and development tools.

## Features

- Modern Python project structure using `pyproject.toml`
- Development tools integration (Black, Flake8, MyPy, isort)
- Testing with pytest
- Virtual environment support
- VS Code integration

## Installation

1. Create and activate a virtual environment:
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

2. Install the project in development mode:
```bash
pip install -e ".[dev]"
```

## Usage

Run the main application:
```bash
python -m src.main
```

## Development

### Running Tests
```bash
pytest
```

### Code Formatting
```bash
black src/ tests/
isort src/ tests/
```

### Type Checking
```bash
mypy src/
```

### Linting
```bash
flake8 src/ tests/
```

## Project Structure

```
pyoxide/
├── src/
│   ├── __init__.py
│   └── main.py
├── tests/
│   ├── __init__.py
│   └── test_main.py
├── .github/
│   └── copilot-instructions.md
├── .vscode/
│   └── tasks.json
├── pyproject.toml
├── requirements.txt
└── README.md
```
