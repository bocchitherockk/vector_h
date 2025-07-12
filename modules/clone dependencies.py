import json
import subprocess
import os

# print(dir(os))
# os.mkdir('yassine')
print(os.listdir())

# Load the JSON file
file_path = 'modules.json'
with open(file_path, 'r') as f:
    data = json.load(f)
    # Clone each repository
    for module_name, repo_url in data.items():
        if module_name in os.listdir():
            # git fetch && git pull
            print(f"Fetching changes into {module_name} from {repo_url}...")
            subprocess.run(['git', '-C', module_name, 'fetch'], check=True)
            subprocess.run(['git', '-C', module_name, 'pull' ], check=True)
            print("Fetching complete!")
        else:
            # git clone
            print(f"Cloning {module_name} from {repo_url}...")
            subprocess.run(['git', 'clone', repo_url, module_name], check=True)
            print("Cloning complete!")
