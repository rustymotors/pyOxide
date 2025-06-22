#!/usr/bin/env python3
"""Simple script to display current coverage percentage."""

import xml.etree.ElementTree as ET
import os


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
        
        line_rate = float(root.get('line-rate', 0))
        branch_rate = float(root.get('branch-rate', 0))
        
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


if __name__ == "__main__":
    get_coverage_percentage()
