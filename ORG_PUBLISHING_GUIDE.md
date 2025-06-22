# README Template for Organizational Publishing

## GitHub Pages Setup Instructions

To publish your coverage reports and make them publicly accessible through your GitHub organization:

### 1. Enable GitHub Pages

1. Go to your repository settings on GitHub
2. Navigate to "Pages" in the left sidebar
3. Under "Source", select "GitHub Actions"
4. The `publish-coverage.yml` workflow will automatically deploy to Pages

### 2. Update Repository Settings

Replace `YOUR_ORG` in the following files with your actual organization name:

- `.github/workflows/publish-coverage.yml` (line 87)
- `README.md` badges section

### 3. Add Organization Badges

Add these badges to your main README.md:

```markdown
# pyOxide

[![Coverage](https://img.shields.io/badge/coverage-check%20pages-blue?logo=github)](https://YOUR_ORG.github.io/pyOxide)
[![Tests](https://github.com/YOUR_ORG/pyOxide/actions/workflows/test-coverage.yml/badge.svg)](https://github.com/YOUR_ORG/pyOxide/actions/workflows/test-coverage.yml)
[![Code Quality](https://github.com/YOUR_ORG/pyOxide/actions/workflows/code-quality.yml/badge.svg)](https://github.com/YOUR_ORG/pyOxide/actions/workflows/code-quality.yml)
[![CI/CD](https://github.com/YOUR_ORG/pyOxide/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/YOUR_ORG/pyOxide/actions/workflows/ci-cd.yml)

Modern Python HTTP Server with Django Integration

📊 **[Live Coverage Report](https://YOUR_ORG.github.io/pyOxide)**
```

### 4. Organization Dashboard Integration

The workflows generate JSON reports that can be consumed by organizational dashboards:

#### Available Endpoints (after Pages deployment):
- **Coverage Dashboard**: `https://YOUR_ORG.github.io/pyOxide`
- **Coverage JSON API**: `https://YOUR_ORG.github.io/pyOxide/coverage-report.json`
- **Raw XML Report**: `https://YOUR_ORG.github.io/pyOxide/coverage.xml`

#### JSON Report Structure:
```json
{
  "project": "pyOxide",
  "timestamp": "2025-06-22T12:00:00.000000",
  "overall": {
    "line_coverage": 85.5,
    "branch_coverage": 78.2
  },
  "packages": [
    {
      "name": "src",
      "line_coverage": 85.5,
      "branch_coverage": 78.2
    }
  ],
  "status": "good",
  "thresholds": {
    "excellent": 90,
    "good": 75,
    "fair": 50
  }
}
```

### 5. Codecov Integration (Optional)

For additional organizational reporting, the workflows also integrate with Codecov:

1. Sign up at [codecov.io](https://codecov.io) with your GitHub organization
2. Add your repository to Codecov
3. The workflows will automatically upload coverage data

### 6. Third-Party Dashboard Integration

The JSON reports can be consumed by various organizational tools:

#### Grafana Dashboard
```json
{
  "dashboard": {
    "title": "Code Coverage Dashboard",
    "panels": [
      {
        "title": "Coverage by Project",
        "type": "stat",
        "targets": [
          {
            "url": "https://YOUR_ORG.github.io/pyOxide/coverage-report.json",
            "jsonPath": "$.overall.line_coverage"
          }
        ]
      }
    ]
  }
}
```

#### Custom Webhook Integration
```python
import requests
import json

def update_org_dashboard():
    """Update organizational dashboard with latest coverage."""
    coverage_url = "https://YOUR_ORG.github.io/pyOxide/coverage-report.json"
    dashboard_api = "https://your-dashboard.com/api/metrics"
    
    response = requests.get(coverage_url)
    coverage_data = response.json()
    
    payload = {
        "project": coverage_data["project"],
        "coverage": coverage_data["overall"]["line_coverage"],
        "status": coverage_data["status"],
        "timestamp": coverage_data["timestamp"]
    }
    
    requests.post(dashboard_api, json=payload)
```

### 7. Security and Access Control

The GitHub Pages site will be public by default. For private organizational dashboards:

1. Use GitHub API with authentication to fetch artifacts
2. Set up internal mirror/proxy for the JSON data
3. Use GitHub Apps for automated dashboard updates

### 8. Automated Reporting

The workflows include:
- **Daily/Weekly Reports**: Via scheduled runs
- **PR Comments**: Automatic coverage reporting on pull requests
- **Slack/Teams Integration**: Via webhook notifications (add to workflows)
- **Email Notifications**: Via GitHub Actions marketplace actions

### 9. Repository Settings for Organizations

Ensure these settings are configured:

1. **Actions Permissions**: Allow GitHub Actions
2. **Pages Settings**: Enable Pages with GitHub Actions source
3. **Branch Protection**: Require status checks for coverage thresholds
4. **Required Reviewers**: Ensure code quality standards

### 10. Monitoring and Alerts

Add monitoring for coverage regression:

```yaml
# Add to existing workflows
- name: Coverage Regression Check
  run: |
    CURRENT_COVERAGE=$(python scripts/coverage_status.py | grep "Line Coverage" | grep -o "[0-9.]*")
    if (( $(echo "$CURRENT_COVERAGE < 75" | bc -l) )); then
      echo "::error::Coverage dropped below 75%: $CURRENT_COVERAGE%"
      exit 1
    fi
```

This setup provides comprehensive organizational reporting and dashboard integration for your coverage data.
