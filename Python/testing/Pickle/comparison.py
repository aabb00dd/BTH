import json
import os
import sys
from collections import defaultdict

def load_json_files():
    """
    Load all JSON result files in the current directory.
    Returns a dictionary with filename as key and content as value.
    """
    json_files = [f for f in os.listdir(".") if os.path.isfile(f) and f.endswith(".json") 
                and f != "differences.json" and not f.startswith("comparison_")]
    
    results = {}
    for file in json_files:
        try:
            with open(file, 'r') as f:
                results[file] = json.load(f)
            print(f"Loaded: {file}")
        except Exception as e:
            print(f"Error loading {file}: {e}")
    
    return results

def extract_environment_info(filename):
    """
    Extract OS and Python version from the filename.
    Expected format: OS_<os>_PYTHON_<version>.json
    """
    parts = filename.split('_')
    if len(parts) >= 4 and parts[0] == "OS" and "PYTHON" in parts:
        python_index = parts.index("PYTHON")
        os_name = parts[1]
        python_version = parts[python_index + 1].replace(".json", "")
        return os_name, python_version
    return "unknown", "unknown"

def compare_results(results):
    """
    Compare results across different environments and protocols.
    Returns a structured analysis of differences.
    """
    # Initialize the structure to hold our comparison results
    comparison = {
        "by_test_case": {},
        "by_environment": {},
        "by_protocol": {},
        "summary": {
            "total_test_cases": 0,
            "consistent_across_all": 0,
            "differ_by_protocol": 0,
            "differ_by_environment": 0
        }
    }
    
    # Get all test cases from all files
    all_test_cases = set()
    for file_content in results.values():
        for protocol_data in file_content.values():
            all_test_cases.update(protocol_data.keys())
    
    comparison["summary"]["total_test_cases"] = len(all_test_cases)
    
    # Analyze each test case
    for test_case in all_test_cases:
        comparison["by_test_case"][test_case] = {
            "consistent": True,
            "protocol_differences": {},
            "environment_differences": {}
        }
        
        # Check for protocol differences within the same environment
        for filename, file_content in results.items():
            os_name, python_version = extract_environment_info(filename)
            env_key = f"{os_name}-{python_version}"
            
            # Get results for this test case across all protocols in this environment
            protocol_results = {}
            for protocol, protocol_data in file_content.items():
                if test_case in protocol_data:
                    protocol_results[protocol] = protocol_data[test_case]
            
            # If we have results for multiple protocols, check for differences
            if len(protocol_results) > 1:
                unique_values = set(protocol_results.values())
                if len(unique_values) > 1:
                    comparison["by_test_case"][test_case]["consistent"] = False
                    comparison["by_test_case"][test_case]["protocol_differences"][env_key] = protocol_results
                    comparison["summary"]["differ_by_protocol"] += 1
        
        # Check for environment differences for the same protocol
        for protocol in ['0', '1', '2', '3', '4', '5']:
            protocol_env_results = {}
            
            for filename, file_content in results.items():
                os_name, python_version = extract_environment_info(filename)
                env_key = f"{os_name}-{python_version}"
                
                if protocol in file_content and test_case in file_content[protocol]:
                    protocol_env_results[env_key] = file_content[protocol][test_case]
            
            # If we have results for multiple environments, check for differences
            if len(protocol_env_results) > 1:
                unique_values = set(protocol_env_results.values())
                if len(unique_values) > 1:
                    comparison["by_test_case"][test_case]["consistent"] = False
                    comparison["by_test_case"][test_case]["environment_differences"][protocol] = protocol_env_results
                    
                    # Only increment once per test case
                    if comparison["summary"]["differ_by_environment"] == comparison["summary"]["differ_by_protocol"]:
                        comparison["summary"]["differ_by_environment"] += 1
    
    # Count test cases that are consistent across all environments and protocols
    for test_case, data in comparison["by_test_case"].items():
        if data["consistent"]:
            comparison["summary"]["consistent_across_all"] += 1
    
    return comparison

"""
Disclaimer: Much of the html report generation is generated or partially generated using Claude 3.7 sonnet via github copilot in visual studio code.
This was seen as necessary as no one really knew HTML and we didnt want to just print it in the terminal, and this was just displaying our results anyway, which
does not take away from the rest of the project.
"""
def generate_html_report(comparison, results):
    """
    Generate an HTML report from the comparison results.
    """
    html = """
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Pickle Compatibility Report</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 20px; line-height: 1.6; }
            h1, h2, h3 { color: #333; }
            .summary { background-color: #f5f5f5; padding: 15px; border-radius: 5px; margin-bottom: 20px; }
            .inconsistent { background-color: #fff0f0; padding: 10px; margin: 10px 0; border-left: 4px solid #ff6b6b; }
            .test-case { margin-bottom: 20px; padding: 10px; background-color: #f9f9f9; border-radius: 5px; }
            table { border-collapse: collapse; width: 100%; }
            th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
            th { background-color: #f2f2f2; }
            .hash-cell { font-family: monospace; word-break: break-all; }
            .environment { display: inline-block; margin-right: 10px; padding: 3px 6px; background-color: #e9ecef; border-radius: 3px; }
        </style>
    </head>
    <body>
        <h1>Pickle Compatibility Report</h1>
    """
    
    # Add summary section
    summary = comparison["summary"]
    html += f"""
    <div class="summary">
        <h2>Summary</h2>
        <p>Total test cases: {summary["total_test_cases"]}</p>
        <p>Consistent across all environments and protocols: {summary["consistent_across_all"]}</p>
        <p>Test cases with protocol differences: {summary["differ_by_protocol"]}</p>
        <p>Test cases with environment differences: {summary["differ_by_environment"]}</p>
    </div>
    """
    
    # Add environments section
    html += "<h2>Tested Environments</h2><ul>"
    environments = set()
    for filename in results.keys():
        os_name, python_version = extract_environment_info(filename)
        environments.add(f"{os_name} - Python {python_version}")
    
    for env in sorted(environments):
        html += f"<li>{env}</li>"
    html += "</ul>"
    
    # Add inconsistent test cases
    html += "<h2>Inconsistent Test Cases</h2>"
    
    inconsistent_count = 0
    for test_case, data in comparison["by_test_case"].items():
        if not data["consistent"]:
            inconsistent_count += 1
            html += f"<div class='test-case'><h3>{test_case}</h3>"
            
            # Protocol differences
            if data["protocol_differences"]:
                html += "<h4>Protocol Differences</h4>"
                for env, protocol_results in data["protocol_differences"].items():
                    html += f"<p>Environment: {env}</p>"
                    html += "<table><tr><th>Protocol</th><th>Hash</th></tr>"
                    
                    # Group protocols by hash value for easier reading
                    hash_to_protocols = defaultdict(list)
                    for protocol, hash_val in protocol_results.items():
                        hash_to_protocols[hash_val].append(protocol)
                    
                    for hash_val, protocols in hash_to_protocols.items():
                        protocols_str = ", ".join(protocols)
                        html += f"<tr><td>{protocols_str}</td><td class='hash-cell'>{hash_val}</td></tr>"
                    
                    html += "</table>"
            
            # Environment differences
            if data["environment_differences"]:
                html += "<h4>Environment Differences</h4>"
                for protocol, env_results in data["environment_differences"].items():
                    html += f"<p>Protocol: {protocol}</p>"
                    html += "<table><tr><th>Environment</th><th>Hash</th></tr>"
                    
                    # Group environments by hash value for easier reading
                    hash_to_envs = defaultdict(list)
                    for env, hash_val in env_results.items():
                        hash_to_envs[hash_val].append(env)
                    
                    for hash_val, envs in hash_to_envs.items():
                        envs_html = "".join([f"<span class='environment'>{env}</span>" for env in envs])
                        html += f"<tr><td>{envs_html}</td><td class='hash-cell'>{hash_val}</td></tr>"
                    
                    html += "</table>"
            
            html += "</div>"
    
    if inconsistent_count == 0:
        html += "<p>All test cases are consistent across environments and protocols!</p>"
    
    html += """
    </body>
    </html>
    """
    
    return html

def run_comparison():
    print("Loading JSON result files...")
    results = load_json_files()
    
    if not results:
        print("No result files found. Run main.py first to generate results.")
        return
    
    print(f"Loaded {len(results)} result files.")
    print("Comparing results...")
    
    comparison = compare_results(results)
    
    # Save JSON comparison results
    with open("comparison_results.json", "w") as f:
        json.dump(comparison, f, indent=2)
    
    print("Generated comparison_results.json")
    
    # Generate and save HTML report
    html_report = generate_html_report(comparison, results)
    with open("comparison_report.html", "w") as f:
        f.write(html_report)
    
    print("Generated comparison_report.html")
    
    # Print summary to console
    summary = comparison["summary"]
    print("\nSummary:")
    print(f"Total test cases: {summary['total_test_cases']}")
    print(f"Consistent across all environments and protocols: {summary['consistent_across_all']}")
    print(f"Test cases with protocol differences: {summary['differ_by_protocol']}")
    print(f"Test cases with environment differences: {summary['differ_by_environment']}")

if __name__ == "__main__":
    run_comparison()
