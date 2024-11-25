import json
import subprocess
import os

# Load the JSON file
file_path = 'modules.json'
try:
    with open(file_path, 'r') as file:
        data = json.load(file)
except FileNotFoundError:
    print(f"Error: {file_path} not found.")
    exit(1)
except json.JSONDecodeError as e:
    print(f"Error parsing {file_path}: {e}")
    exit(1)

# Ensure the JSON structure is as expected
if not isinstance(data, dict):
    print(f"Error: {file_path} must contain a dictionary.")
    exit(1)

# Clone each repository
for module_name, repo_url in data.items():
    print(f"Cloning {module_name} from {repo_url}...")
    try:
        # Run the git clone command
        subprocess.run(["git", "clone", repo_url, module_name], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error cloning {module_name}: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

print("Cloning complete!")
