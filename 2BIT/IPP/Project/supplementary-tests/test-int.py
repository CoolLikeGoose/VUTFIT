#!/usr/bin/env python3
import os
import subprocess
from pathlib import Path

# Specify the path to the directory containing the files
directory_path = './interpret/'
php_script_path = '../interpret.php'

# ANSI escape code for green text
green_color_code = "\033[92m"
# ANSI escape code for red text
red_color_code = "\033[91m"
# ANSI escape code for yellow text
yellow_color_code = "\033[93m"
# Reset ANSI escape code
reset_color_code = "\033[0m"

def printRed(text):
    print(red_color_code + text + reset_color_code)

def printGreen(text):
    print(green_color_code + text + reset_color_code)

def printYellow(text):
    print(yellow_color_code + text + reset_color_code)

def ERROR(message):
    printRed(f"Error: {message}")

if not os.path.isfile(php_script_path):
    ERROR("interpret.php is missing; wrong path")
    exit(1)

def get_file_extension(filename):
    return Path(filename).suffix

def read_integer_from_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()
        number = int(content)

        return number

def group_files_by_name_without_extension(directory_path):
    files = os.listdir(directory_path)
    files_by_name = {}

    # Group files by filenames without extension
    for file in files:
        name_without_extension, _ = os.path.splitext(file)
        if name_without_extension not in files_by_name:
            files_by_name[name_without_extension] = []

        # Append the file to the list corresponding to its name without extension
        files_by_name[name_without_extension].append(file)

    return files_by_name

# Get the dictionary of files grouped by filenames without extension
files_by_name = group_files_by_name_without_extension(directory_path)

for file in files_by_name:
    # Run the command and capture the standard output
    params = ['php', php_script_path]
    outputFile = ""
    rc = 0

    for f in files_by_name[file]:
        suffix = get_file_extension(f)
        if suffix == ".src":
            params.append('--source='+directory_path+f)
        elif suffix == ".in":
            params.append('--input='+directory_path+f)
        elif suffix == ".out":
            with open(directory_path+f, 'r') as expectedOutputFile:
                # Read the content of the file into a string
                expectedOutput = expectedOutputFile.read()
        elif suffix == ".rc":
            rc = read_integer_from_file(directory_path+f)

    try:
        process = subprocess.run(params, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout = process.stdout.decode('UTF8')
        stderr = process.stderr.decode('UTF8')

        # Check if the command was successful (return code 0)
        if process.returncode != rc:
            printRed(f"{file} fail")
            printRed(f"Expected return code {rc} got {process.returncode}.")
            printYellow("Standard Output:")
            print(stdout)
            printYellow("Error:")
            print(stderr)
        else:
            if process.returncode == 0 and expectedOutput != stdout:
                printRed(f"{file} unexpected output")
                printYellow("Standard Output:")
                print(stdout)
                printYellow("expected:")
                print(expectedOutput)
            else:    
                printGreen(f"{file} ok")
        print("")

    except Exception as e:
        print(f"An error occurred: {e}")
