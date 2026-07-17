#!/usr/bin/env python3
"""Link the relevant contents of the LUNA project into the SOL project"""

import argparse
import os
import sys

# Folders to be linked from the luna project
FOLDERS = ("gradescope", "solution", "template")

from quest import PROJECT_ROOT

def main():
    parser = argparse.ArgumentParser(description="Symlink gradescope, solution, and template folders from a luna project directory.")
    parser.add_argument("source_dir", help="Path to the luna project directory")
    args = parser.parse_args()

    source = os.path.abspath(args.source_dir)

    if not os.path.isdir(source):
        print(f"Error: '{source}' is not a directory or does not exist.", file=sys.stderr)
        sys.exit(1)

    for folder in FOLDERS:
        target = os.path.join(source, folder)
        if not os.path.exists(target):
            print(f"Error: '{folder}' folder not found in '{source}'.", file=sys.stderr)
            sys.exit(1)

    for folder in FOLDERS:
        link_name = os.path.join(PROJECT_ROOT, folder)
        if os.path.exists(link_name) and not os.path.islink(link_name):
            print(f"Error: '{link_name}' exists and is not a symlink. Remove it manually.", file=sys.stderr)
            sys.exit(1)

    for folder in FOLDERS:
        target = os.path.join(source, folder)
        link_name = os.path.join(PROJECT_ROOT, folder)
        if os.path.islink(link_name):
            os.remove(link_name)
            print(f"Removed existing symlink: {link_name}")
        os.symlink(target, link_name)
        print(f"Linked: {link_name} -> {target}")

if __name__ == "__main__":
    main()