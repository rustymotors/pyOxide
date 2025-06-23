# Reports

This directory contains generated reports and analysis outputs.

## Contents

### Code Coverage
- `coverage.xml` - XML format coverage report for CI/CD
- `htmlcov/` - HTML coverage report directory
  - Open `htmlcov/index.html` in browser for interactive coverage report

### Security Analysis
- `bandit-report*.json` - Security analysis reports from Bandit
- Multiple reports for different test scenarios

## Viewing Reports

### Coverage Report
```bash
# Generate new coverage report
python -m pytest --cov=src --cov-report=html --cov-report=xml

# View HTML report
open reports/htmlcov/index.html
```

### Security Report
```bash
# Generate new security report
bandit -r src/ -f json -o reports/bandit-report.json

# View report
cat reports/bandit-report.json | jq
```

## Cleanup

These files are automatically generated and can be safely deleted:
```bash
rm -rf reports/htmlcov reports/coverage.xml reports/bandit-report*.json
```

They will be regenerated when running tests or security scans.
