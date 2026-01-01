import tests_file as tsts
import os
import sys
import json

from comparison import run_comparison

pickle_protocols = [0, 1, 2, 3, 4, 5]


def write_results_to_file(results, filename):
    """
    Write the results to a JSON file.
    """
    with open(filename, "w") as file:
        json.dump(results, file)


def main():
    current_os = sys.platform
    current_python_version = (
        f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}"
    )

    protocol_results = {"0": {}, "1": {}, "2": {}, "3": {}, "4": {}, "5": {}}

    # Run tests for each protocol
    for protocol in pickle_protocols:
        protocol_results[str(protocol)] = tsts.pickle_and_store()

    # Generate dynamic filename
    filename = f"OS_{current_os}_PYTHON_{current_python_version}.json"

    # Write results to file
    write_results_to_file(protocol_results, filename)



if __name__ == "__main__":
    # Run the main function
    main()
