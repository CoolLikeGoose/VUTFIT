#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

for test_case in test_cases/*.txt; do

    if [[ "$test_case" == *_output.txt ]]; then
        continue
    fi

    test_name=$(basename "$test_case" .txt)
    echo "TEST started: $test_name"
    
    # inter code
    ASSEMBLER_CODE=$(./../IFJ2023_Swift/cmake-build-debug/IFJ2023_Swift < "$test_case")
    # echo "ASS code $ASSEMBLER_CODE"

    TEMP_ASM_FILE="temp.ifjcode"
    echo "$ASSEMBLER_CODE" > "$TEMP_ASM_FILE"
    # asm out
    output=$(./IFJ_intercode/ic23int.exe "$TEMP_ASM_FILE")
    # exp out
    expected_output=$(cat "${test_case%.txt}_output.txt")

    echo " "
    if [ "$output" == "$expected_output" ]; then
        echo "-----------------------------"
        echo "Programm output: $output"
        echo "-----------------------------"
        echo -e "${GREEN}TEST passed: $test_name ${NC}"
    else
        echo "-----------------------------"
        echo "Programm output: $output"
        echo "-----------------------------"
        echo "Expected output: $expected_output"
        echo "-----------------------------"
        echo -e "${RED}TEST failed: $test_name ${NC}"
    fi
done