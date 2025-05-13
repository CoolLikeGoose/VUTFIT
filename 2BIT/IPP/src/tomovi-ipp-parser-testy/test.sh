#!/bin/bash

# Navigate to the tests directory
cd $(dirname $0)

# Define ANSI color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Path to the JExamXML tool
JEXAMXML_PATH="../../../../../../Code/Apps/jexamxml/"

# Check if a specific test name was provided as an argument
if [ "$#" -eq 1 ]; then
  test_files=("$1.src")
else
  # If no specific test was provided, run all .src files
  test_files=(*.src)
fi

# Function to run a single test
run_test() {
  local src_file=$1
  # Extract the base name without the extension
  local base_name=$(basename "$src_file" .src)

  # Execute the python script and capture both exit code and output
  local output
  output=$(python3.10 ../parse.py <"$src_file" 2>&1) # Capture stderr as well, if needed
  local exit_code=$?

  # Read the expected exit code from the .rc file
  local expected_exit_code
  expected_exit_code=$(cat "${base_name}.rc")

  # Initialize test result message
  local result_message="${base_name}: Test"

  if [ "$expected_exit_code" -ne 0 ]; then
    if [ "$exit_code" -eq "$expected_exit_code" ]; then
      echo -e "${GREEN}${result_message} successful${NC}"
    else
      echo -e "${RED}${result_message} not successful${NC}"
      echo "Expected exit code: $expected_exit_code, Got: $exit_code"
    fi
  else
    # Use JExamXML for comparing XML outputs if the expected exit code is 0
    echo "$output" >"${base_name}_actual.out"
    local expected_output="${base_name}.out"
    jexamxml_output=$(java -jar "$JEXAMXML_PATH" "$expected_output" "${base_name}_actual.out" delta.xml 2>&1)
    local jexamxml_exit_code=$?
    if [ "$exit_code" -eq "$expected_exit_code" ]; then
      if [ "$jexamxml_exit_code" -eq 0 ] || [ ! -f delta.xml ]; then
        echo -e "${GREEN}${result_message} successful${NC}"
      else
        echo -e "${RED}${result_message} not successful${NC}"
        echo "Expected exit code: $expected_exit_code, Got: $exit_code"
        echo "Output files do not match. Differences:"
        cat delta.xml
      fi
    else
      echo -e "${RED}${result_message} not successful${NC}"
      echo "Expected exit code: $expected_exit_code, Got: $exit_code"
      if [ -f delta.xml ]; then
        echo "Output files do not match. Differences:"
        cat delta.xml
      fi
    fi
    # Clean up
    rm "${base_name}_actual.out"
    [ -f delta.xml ] && rm delta.xml # Ensure removal only if it exists
  fi
}

# Iterate over the specified test files
for src_file in "${test_files[@]}"; do
  run_test "$src_file"
done
