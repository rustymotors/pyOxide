#!/usr/bin/env python3
"""Simple script to display current coverage percentage."""

import json
import os
import xml.etree.ElementTree as ET
from datetime import datetime


def get_coverage_percentage():
    """Extract coverage percentage from coverage.xml file."""
    coverage_file = "coverage.xml"

    if not os.path.exists(coverage_file):
        print("❌ coverage.xml not found. Run tests with coverage first:")
        print("   pytest --cov=src --cov-report=xml")
        return None

    try:
        tree = ET.parse(coverage_file)
        root = tree.getroot()

        line_rate = float(root.get("line-rate", 0))
        branch_rate = float(root.get("branch-rate", 0))

        line_percentage = line_rate * 100
        branch_percentage = branch_rate * 100

        # Color coding based on coverage
        if line_percentage >= 90:
            color = "🟢"  # Green
        elif line_percentage >= 75:
            color = "🟡"  # Yellow
        elif line_percentage >= 50:
            color = "🟠"  # Orange
        else:
            color = "🔴"  # Red

        print(f"{color} Coverage Report")
        print(f"  Line Coverage:   {line_percentage:.1f}%")
        print(f"  Branch Coverage: {branch_percentage:.1f}%")
        print(f"  HTML Report:     htmlcov/index.html")
        print(f"  XML Report:      coverage.xml")

        return line_percentage

    except Exception as e:
        print(f"❌ Error reading coverage.xml: {e}")
        return None


def generate_coverage_json():
    """Generate JSON report for organizational dashboards."""
    coverage_file = "coverage.xml"

    if not os.path.exists(coverage_file):
        return None

    try:
        tree = ET.parse(coverage_file)
        root = tree.getroot()

        line_rate = float(root.get("line-rate", 0))
        branch_rate = float(root.get("branch-rate", 0))

        # Get detailed package information
        packages = []
        for package in root.findall(".//package"):
            pkg_name = package.get("name", "unknown")
            pkg_line_rate = float(package.get("line-rate", 0))
            pkg_branch_rate = float(package.get("branch-rate", 0))

            packages.append(
                {
                    "name": pkg_name,
                    "line_coverage": pkg_line_rate * 100,
                    "branch_coverage": pkg_branch_rate * 100,
                }
            )

        coverage_data = {
            "project": "pyOxide",
            "timestamp": datetime.now().isoformat(),
            "overall": {
                "line_coverage": line_rate * 100,
                "branch_coverage": branch_rate * 100,
            },
            "packages": packages,
            "thresholds": {"excellent": 90, "good": 75, "fair": 50},
            "status": get_coverage_status(line_rate * 100),
            "reports": {"html": "htmlcov/index.html", "xml": "coverage.xml"},
        }

        # Save JSON report
        with open("coverage-report.json", "w") as f:
            json.dump(coverage_data, f, indent=2)

        return coverage_data

    except Exception as e:
        print(f"❌ Error generating coverage JSON: {e}")
        return None


def get_coverage_status(percentage: float) -> str:
    """Get coverage status badge text."""
    if percentage >= 90:
        return "excellent"
    elif percentage >= 75:
        return "good"
    elif percentage >= 50:
        return "fair"
    else:
        return "poor"


def generate_org_summary():
    """Generate organizational summary for dashboard integration."""
    coverage_data = generate_coverage_json()
    if not coverage_data:
        return

    print("\n📊 Organizational Coverage Summary")
    print("=" * 50)
    print(f"Project: {coverage_data['project']}")
    print(f"Status: {coverage_data['status'].upper()}")
    print(f"Line Coverage: {coverage_data['overall']['line_coverage']:.1f}%")
    print(f"Branch Coverage: {coverage_data['overall']['branch_coverage']:.1f}%")
    print(f"Generated: {coverage_data['timestamp']}")

    if coverage_data["packages"]:
        print(f"\n📦 Package Breakdown ({len(coverage_data['packages'])} packages):")
        for pkg in coverage_data["packages"]:
            status_icon = (
                "🟢"
                if pkg["line_coverage"] >= 75
                else "🟡" if pkg["line_coverage"] >= 50 else "🔴"
            )
            print(f"  {status_icon} {pkg['name']}: {pkg['line_coverage']:.1f}%")


if __name__ == "__main__":
    coverage = get_coverage_percentage()
    if coverage is not None:
        generate_org_summary()
