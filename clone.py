import os
import shutil
import sys
import glob

def cloneFile(sourceFile: str, targetFile: str):
    os.makedirs(os.path.dirname(targetFile), exist_ok=True)
    shutil.copy2(sourceFile, targetFile)  # Using shutil.copy2 to preserve metadata

Filters = [
    "**/*.h",
    "**/*.hpp",
]

def main():
    if len(sys.argv) < 3:
        print("Usage: clone.py <source> <target>")
        return

    source = sys.argv[1]
    target = sys.argv[2]

    for filter in Filters:
        for file in glob.iglob(os.path.join(source, filter), recursive=True):
            target_file = os.path.join(target, os.path.relpath(file, source))
            cloneFile(file, target_file)

if __name__ == "__main__":
    main()
