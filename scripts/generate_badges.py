#!/usr/bin/env python3
"""Generate status badges for README."""

import xml.etree.ElementTree as ET
import os

def generate_coverage_badge():
    """Generate a coverage badge URL."""
    coverage_file = "coverage.xml"
    
    if not os.path.exists(coverage_file):
        return "![Coverage](https://img.shields.io/badge/coverage-unknown-lightgrey)"
    
    try:
        tree = ET.parse(coverage_file)
        root = tree.getroot()
        line_rate = float(root.get('line-rate', 0))
        percentage = int(line_rate * 100)
        
        # Color based on coverage
        if percentage >= 90:
            color = "brightgreen"
        elif percentage >= 75:
            color = "green"
        elif percentage >= 60:
            color = "yellowgreen"
        elif percentage >= 50:
            color = "yellow"
        elif percentage >= 40:
            color = "orange"
        else:
            color = "red"
        
        return f"![Coverage](https://img.shields.io/badge/coverage-{percentage}%25-{color})"
    
    except Exception:
        return "![Coverage](https://img.shields.io/badge/coverage-error-red)"

def generate_badges():
    """Generate all status badges."""
    print("<!-- Status Badges -->")
    print("![Python](https://img.shields.io/badge/python-3.8%2B-blue)")
    print("![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)")
    print("![Type checking: mypy](https://img.shields.io/badge/type%20checking-mypy-blue)")
    print("![Testing: pytest](https://img.shields.io/badge/testing-pytest-green)")
    print(generate_coverage_badge())
    print("![Security: bandit](https://img.shields.io/badge/security-bandit-yellow)")
    print("![License: MIT](https://img.shields.io/badge/license-MIT-blue)")

if __name__ == "__main__":
    generate_badges()
