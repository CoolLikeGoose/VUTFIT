#!/usr/bin/env python3
import os
import subprocess
from pathlib import Path

# Specify the path to the directory containing the files
python_script_path = '../../parse.py'

# Set jexamxml path
jexamxml = "jexamxml/"

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

def fileWrite(text, filename):
    try:
        file = open(filename, "w")
        file.write(text)
    finally:
        if file is not None:
            file.close()

def fileRead(filename):
    file = None
    text = ""
    try:
        file = open(filename, "r")
        text = file.read()
    finally:
        if file is not None:
            file.close()
        return text

def OK(test):
    printGreen(f"{test} OK")

def ERROR(message):
    printRed(f"Error: {message}")

if not os.path.isfile(python_script_path):
    ERROR("parse.py is missing; wrong path")
    exit(1)

if not os.path.isfile(os.path.join(jexamxml, "jexamxml.jar")):
    ERROR(f"jexamxml.jar is missing; are you running on merlin? \n"
          f"(alternatively, you can install java and download the .jar file "
          f"from merlin on this path: {os.path.join(jexamxml, 'jexamxml.jar')})")
    exit(1)

for tSrc in os.listdir("./parse"):
    if os.path.isdir("./parse/" + tSrc):
        for src in os.listdir("./parse/" + tSrc):
            fullSrc = "./parse/" + tSrc + "/" + src
            if fullSrc.endswith(".src"):
                testName = os.path.splitext(src)[0]
                test = os.path.splitext(fullSrc)[0]

                # Run parse.py
                process = subprocess.run(["python3", python_script_path], input=open(fullSrc, "rb").read(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)

                stdout = process.stdout.decode('UTF8')
                fileWrite(stdout, f"{test}.your_out")
                stderr = process.stderr.decode('UTF8')
                rc = open(f'{test}.rc').read()

                if int(rc) != process.returncode:
                    printRed(f"{testName} fail")
                    printRed(f"Expected return code {rc} got {process.returncode}.")
                else:
                    if process.returncode == 0:
                        # Run jexamxml.jar
                        diffProcess = subprocess.run(["java", "-jar", "../../" + jexamxml + "jexamxml.jar", f"{testName}.out", f"{testName}.your_out",
                                        f"{testName}.xml", "-D", "../../" + jexamxml + "options"],
                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd="parse/" + tSrc + "/")

                        diffStdout = diffProcess.stdout.decode('UTF8')
                        diffStderr = diffProcess.stderr.decode('UTF8')

                        if diffProcess.returncode:
                            printRed(f"{testName} unexpected output")
                            # Print the result
                            toPrint = fileRead(f"{test}.xml")
                            print(toPrint)
                        else:
                            OK(testName)
                    else:
                        OK(testName)
